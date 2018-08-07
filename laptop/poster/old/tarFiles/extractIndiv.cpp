/*   
 *
 *   extractIndiv.cpp 
 *
 *   Program to extract genotype data for a list of individuals
 *
 *   Written by Sharlee Climer
 *   July 2010
 *
 *   Revised to allow different columns hold search symbol using
 *   SYMBOL_COL in header file
 *   December 2011
 *
 *   Revised to print unmatched items to 'unmatched.txt' when PRINT_UNMATCHED 
 *   is set to one.
 *   August 2013
 *
 *   Fixed bug - extraction of rows with a period instead of the label.
 *   November 2014
 *
 *   Moved number of header rows to command line and added error checking.
 *   January 2016
 * 
*/

#include"extractIndiv.h"

int main(int argc, char ** argv)
{
  if (argc != 8)
    fatal("Usage:\n  extractRows listOfIDs.txt numInList original.txt numHeaderRows numDataRows numCols output.txt"); 

  timer t;
  t.start("Timer started");  


  FILE *indiv; // contains the list of individuals to output genotypes for
  FILE *input; // contains all genotypes
  FILE *output; // will hold genotype data for list of individuals
  FILE *unmatched; // will hold unmatched items if PRINT_UNMATCHED is set to 1

  std::cout << "Command line arguments: \n\t";
  for (int i = 0; i < argc; i++)
	std::cout << argv[i] << " ";
  std::cout << "\n" << std::endl;

  // check to be sure all files are available
  if ((indiv = fopen(argv[1], "r")) == NULL)
    fatal("List of individuals could not be opened");
  if (((input = fopen(argv[3], "r")) == NULL) || ((output = fopen(argv[7], "w")) == NULL))
    fatal("File could not be opened.\n");

  if (PRINT_UNMATCHED)
   if ((unmatched = fopen("unmatched.txt", "w")) == NULL)
     fatal("Unable to open 'unmatched.txt' for writing");

  int numInd = atoi(argv[2]); // number of individuals in list
  int numCols = atoi(argv[6]); // number of columns
  int numHeadRows = atoi(argv[4]); // number of header rows
  int numRows = atoi(argv[5]); // number of data rows

  char strng[1000]; // temporary storage of strings

  std::cout << numCols << " columns in data file." << std::endl;
  std::cout << "Extracting data for " << numInd << " rows listed in '" << argv[1] << "' from '" << argv[3] << "'...\n" << std::endl;
  std::cout << "Assuming original data file has " << numHeadRows << " header rows, " << numRows << " data rows, and " << numCols << " total columns.\n" << std::endl;

  if(SYMBOL_COL != 1)
    std::cout << "***Important: String in column " << SYMBOL_COL << " of the input data file will be used for matching.***\n" << std::endl;

  // allocate memory for list of individuals
  char **ind;
  if((ind = new char*[numInd]) == NULL)
    fatal("memory not allocated");
  for (int i = 0; i < numInd; i++) {
    if ((ind[i] = new char[1000]) == NULL)
      fatal("memory not allocated");
  }

  // allocate memory for lengths of each individuals label
  int *length;
  if((length = new int[numInd]) == NULL)
    fatal("memory not allocated");

  // allocate memory for checking which items are found
  int *found;
  if((found = new int[numInd]) == NULL)
    fatal("memory not allocated");

  for (int i = 0; i < numInd; i++)
    found[i] = 0; // initialize to not found

  // read in and store list of individuals
  for (int i = 0; i < numInd; i++) 
    fscanf(indiv,"%s",ind[i]);

  fclose(indiv);

  if (VERBOSE) {
    std::cout << "Row IDs to extract: " << std::endl;
    for (int i = 0; i < numInd; i++)
      std::cout << ind[i] << std::endl;
  }
  
  // record length of each string

  int num; // count number of characters
  for (int i = 0; i < numInd; i++) {
    num = 0;
    while (ind[i][num] != '\0')
      num++;
    if (num >= 1000)
      fatal("String lengths too long.");
    length[i] = num; // number of chars in string
  }

  if(VERBOSE) {
    std::cout << "Rows to extract:" << std::endl;
    for (int i = 0; i < numInd; i++)
      std::cout << ind[i] << std::endl;
    std::cout << std::endl;
  }

  // read in and write out header rows
  if(numHeadRows > 0) 
    for (int i = 0; i < numHeadRows; i++) {
      for (int j = 0; j < numCols; j++) {
	fscanf(input, "%s", strng);
	fprintf(output, "%s ", strng);
      }

      fprintf(output, "\n");
    }

  // read in and write out data
  char **currentRow; // hold current row of strings
  int numTotal = 0; // count total number of individuals
  int check = 0; // count number written out (to check validity)
  int inList; // flag individuals that are in the list

  // allocate memory
  if((currentRow = new char*[numCols]) == NULL)
    fatal("memory not allocated");
  for (int i = 0; i < numCols; i++) {
    if ((currentRow[i] = new char[1000]) == NULL)
      fatal("memory not allocated");
  }

  if (VERBOSE)
    std::cout << "Individuals in input file: " << std::endl;

  while (!feof(input)) { // read until reach end of file
    inList = 0; // initialize flag

    for (int i = 0; i < numCols; i++) {
      if(feof(input))
	 break;
      fscanf(input,"%s",currentRow[i]); // read in strings in current row
    }

    if (feof(input))
      break;

    // check to see if read in white space at end of file
    if(currentRow[0][1] == '\0')  // only one char in string
      if(((int)currentRow[0][0] < 48) || ((int)currentRow[0][0] > 57)) { // not just a number for the label
	fscanf(input,"%s",strng); // see if at end of file
	if(!feof(input))
	  warning("Unexpected individual label with one character");
	else
	  break; // at end of file
      }

    numTotal++; // one more individual read in from input file
    if (numTotal > numRows) { // error
      std::cout << "Read " << numTotal << " data rows and only expected " << numRows << " rows.\n" << std::endl;
      fatal("More input data than expected.");
    }

    if (VERBOSE) 
      std::cout << currentRow[SYMBOL_COL-1] << " "; // symbol to use for this row
    
    num = 0; // initialize number of chars
    while (currentRow[SYMBOL_COL-1][num] != '\0')
      num++;
    if (num >= 1000)
      fatal("String length too long.");

    if (num < MIN_CHARS_ID) { // too few characters, continue to next row
      if (!QUIET)
	std::cout << "Skipped row with " << currentRow[SYMBOL_COL-1] << " identifier." << std::endl;

      continue;
    }

    //std::cout << currentRow[SYMBOL_COL-1] << " with " << num << " chars" << std::endl;
    
    // see if this individual is on the list
    for (int i = 0; i < numInd; i++) {
      if ((num == length[i]) && (strncmp(currentRow[SYMBOL_COL-1], ind[i], length[i]) == 0)) {
	inList = 1; // set flag as this individual is in the list
	check++; // found another that is in list

	if((found[i] > 0) && !QUIET)
	  std::cout << "Item " << ind[i] << " appears more than once in data." << std::endl;

	found[i]++; // this item has been found

	//std::cout << currentRow[SYMBOL_COL-1] << " and check = " << check << std::endl;

	break;
      }
    }

    for (int i = 0; i < numCols; i++) { // write out row, if matched
      if(inList)
	fprintf(output, "%s ", currentRow[i]); // write out data
    }

    if (inList)
      fprintf(output,"\n");
  }

  if (VERBOSE)
    std::cout << "\n" << std::endl;

    if(PRINT_UNMATCHED)
      for (int i = 0; i < numInd; i++)
	if (found[i] == 0)
	  fprintf(unmatched, "%s\n", ind[i]);

  // check validity of number of individuals
  if(check != numInd) {
    std::cout << check << " not equal to " << numInd << std::endl;
    warning("Didn't find all that are listed in input file");
  }

  /*
  int check2 = 0;
  for (int i = 0; i < numInd; i++)
    if (found[i] > 0)
      check2++;
  std::cout << check2 << " items found.\n" << std::endl;
  */
  
  if (numTotal != numRows) { // error
    std::cout << "Read " << numTotal << " data rows and expected " << numRows << " rows.\n" << std::endl;
    fatal("Unexpected amount of input data.");
  }
    
  std::cout << "Data for " << check << " selected rows has been written to '" << argv[7] << "'.\n" << std::endl;

  fclose(input);
  fclose(output);

  if (PRINT_UNMATCHED)
    fclose(unmatched);

  for (int i = 0; i < numInd; i++)
    delete [] ind[i];
  delete [] ind;
  delete [] length;


  t.stop("Timer stopped");
  std::cout << t << " seconds\n" << std::endl;

  return 1;
}


