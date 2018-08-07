/*
 * gene_data_prep.cpp
 *
 *  Created on: Apr 2, 2018
 *      Author: matt
 */

#include "gene_data_prep.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
namespace AI {

GeneDataPrep::GeneDataPrep(
		int numHeaderRows,
		int numHeaderCols,
		int numRows,
		int numCols,
		char * inputFileName)
{
	/*
	 * Set object variables up
	 */
	this->numHeaderRows = numHeaderRows;
	this->numHeaderCols = numHeaderCols;
	this->numRows = numRows > 0 ? numRows : 1;
	this->numCols = numCols;
	this->numMissing = 0;
	this->space = false;
	this->slash = false;
	this->format = false;
	this->rowColFlipped = false;
	memset(this->buffer, '\0', sizeof(char) * BUF_SIZE);
	this->alleleMap = new std::map<char, int>();
    /*
	this->alleleMap['A'] = 0;
	this->alleleMap['C'] = 0;
	this->alleleMap['G'] = 0;
	this->alleleMap['T'] = 0;
	this->alleleMap['M'] = 0;
	*
	 * Allocate memory for array
	 */
	this->data = new float*[numRows];
	this->snpData = new short*[numRows];
	this->allele = new char*[numRows];
	for (int i = 0; i < numRows; i++)
		this->data[i] = new float[numCols],
		    this->snpData[i] = new short[numCols],
			this->allele[i] = new char[2];
	/*
	 * Open file and clear out junk
	 */
	this->inputFile = fopen(inputFileName, "r");
	this->readHeaderRows();
	this->determineFormat();
}
bool GeneDataPrep::isValidChar (int c)
{
    if ((c != 65) && (c != 67) && 	// not 'A' or 'C'
    	(c != 71) && (c != 84) &&  	// not 'G' or 'T'
		(c != 73) && (c != 68) &&  	// not 'I' or 'D'
    	(c != 48) && (c != 78) && 	// not 'N' or '0'
		(c != 63) && (c != 88) && 	// not '?' or 'X'
    	c != MISSING_SYMBOL) 		// not customized missing symbol
    		return false;
    return true;
}
bool GeneDataPrep::isMissingChar (int c)
{
	if ((c == 48) && (c == 78) && 	// not 'N' or '0'
		(c == 63) && (c == 88) && 	// not '?' or 'X'
		c == MISSING_SYMBOL) 		// not customized missing symbol
		return true;
	return false;
}
void GeneDataPrep::determineAlleleTypes()
{
	int ascii1, ascii2;
	this->readHeaderRows();
	for (int i = 0; i < this->numRows; i++)
	{
		/*
		 * Clear out headercols
		 */
		for (int j = 0; j < this->numHeaderCols; j++)
			fscanf(this->inputFile, "%s", this->buffer);

		/*
		 * Parse data
		 */
		for (int j = 0; j < this->numCols; j++)
		{
			/*
			 * Get first allele
			 */
			fscanf(this->inputFile, "%s", this->buffer);
			ascii1 = this->buffer[0];

			/*
			 * Get second allele
			 */
			if (this->space)
			{
				fscanf(this->inputFile, "%s", this->buffer);
				ascii2 = this->buffer[0];
			}
			else
			{
				if (this->slash)
					ascii2 = this->buffer[2];
				else
					ascii2 = this->buffer[1];
			}

			if (!this->isValidChar(ascii1) || !this->isValidChar(ascii2))
				exit(-1);	// TODO: handle error better

			if (!this->isMissingChar(ascii1))
			{
				if (allele[i][0] == '0')
					allele[i][0] = (char)ascii1;

				if (allele[i][1] == '0')
				{
					if (ascii1 != this->allele[i][0])
					{
						allele[i][1] = (char)ascii1;
					}
					else if (ascii2 != this->allele[i][0])
					{
						allele[i][1] = (char)ascii2;
					}
				}
			}
			else
			{
			}
		}
		/*
		 * Flip the alleles into alphabetical order
		 */
		if (allele[i][1] > allele[i][0])
		{
			int temp = allele[i][0];
			allele[i][0] = allele[i][1];
			allele[i][1] = temp;
		}
	}
}
void GeneDataPrep::createSnpData()
{
	int ascii1, ascii2, oneAllele;
	char ** data1, data2;
	/*
	 * Determine allele types
	 */
	this->determineAlleleTypes();
	/*
	 * Rewind file and clear out junk
	 */
	this->readHeaderRows();
	/*
	 * Create data
	 */
	for (int i = 0; i < this->numRows; i++)
	{
		/*
		 * Clear out headercols
		 */
		for (int j = 0; j < this->numHeaderCols; j++)
			fscanf(this->inputFile, "%s", this->buffer);

		/*
		 * Parse data
		 */
		for (int j = 0; j < this->numCols; j++)
		{
            /*
             * Get first allele
             */
			fscanf(this->inputFile, "%s", this->buffer);
			ascii1 = this->buffer[0];

			/*
			 * Get second allele
			 */
			if (this->space)
			{
				fscanf(this->inputFile, "%s", this->buffer);
				ascii2 = this->buffer[0];
			}
			else
			{
				if (this->slash)
					ascii2 = this->buffer[2];
				else
					ascii2 = this->buffer[1];
			}
			if (!this->isValidChar(ascii1) ||
					ascii1 != (int)this->allele[i][0] ||
					ascii1 != (int)this->allele[i][1])
			{
				ascii1 = 48;
			}
			if (!this->isValidChar(ascii2) ||
					ascii2 != (int)this->allele[i][0] ||
					ascii2 != (int)this->allele[i][1])
			{
				ascii2 = 48;
			}
            /*
             * Assign to snpData
             */
            this->snpData[i][0] = ascii1,
                this->snpData[i][1] = ascii2;
		}
	}
}
void GeneDataPrep::determineFormat()
{

	int ascii1;
	// read in data
	for (int i = 0; i < this->numRows; i++)
	{
		if (format)
			break; // already determined format

		for (int j = 0; j < this->numHeaderCols; j++)
			fscanf(this->inputFile, "%s", this->buffer); // read in and disregard header columns

		for (int j = 0; j < this->numCols; j++)
		{
			if(feof(this->inputFile))
				exit(-1);

			fscanf(this->inputFile, "%s", this->buffer);

			ascii1 = this->buffer[0]; // find ascii value of first char

			if ((ascii1 != 48) && (ascii1 != 78)) // not 'N' or '0'
            {
				if(ascii1 != MISSING_SYMBOL) // not customized missing symbol
                {
					if ((ascii1 != 63) && (ascii1 != 88)) // not '?' or 'X'
					{

						if ((int)this->buffer[1] == 47) // second char is a '/'
							slash = 1;
						else if ((int)this->buffer[1] < 65) // second char is not a letter
							space = 1; // space between chars

						format = 1;
						break; // determined format
                	}
                }
            }
		}
	}
}

void GeneDataPrep::readHeaderRows()
{
	rewind(this->inputFile);
	for (int i = 0; i < this->numHeaderRows; i++)
		for (int j = 0; j < this->numHeaderCols + this->numCols; j++)
			fscanf(this->inputFile, "%s", this->buffer);
}
void GeneDataPrep::parseInputFile()
{
	for (int i = 0; i < this->numRows; i++)
	{
		/*
		 * Clean out header columns
		 */
		for (int j = 0; j < this->numHeaderCols; j++)
			fscanf(this->inputFile, "%s", this->buffer);
		/*
		 * Read in actual data
		 */
		for (int j = 0; j < this->numCols; j++)
		{
			fscanf(this->inputFile, "%s", this->buffer);
			int ascii = (int) this->buffer[0];
			if (isalpha(this->buffer[0]))
			{
				if ((int)this->buffer[0] == 78)
				{
					if ((strncmp(this->buffer, "NA", 2) == 0) || (strncmp(this->buffer, "NN", 2)))
					{
						this->numMissing++;
						this->data[i][j] = MAX_VAL + 1;
					}
					// TODO: Add valadation
				}
			}

		}
	}
}
GeneDataPrep::~GeneDataPrep() {
	// TODO Auto-generated destructor stub
}
void GeneDataPrep::computePearsonCorrolation()
{
	for (int x = 0; x < this->numRows - 1; x++)
	{
		for (int y = x + 1; y < this->numRows; y++)
		{
			float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0, sumY2 = 0, N = 0, num = 0, den = 0;
			for (int i = 0; i < this->numCols; i++)
			{
				if (this->isValidNum(this->data[x][i]) && this->isValidNum(this->data[y][i]))
				{
					N++;
					sumX += this->data[x][i];
					sumY += this->data[y][i];
					sumXY += this->data[x][i] * this->data[y][i];
					sumX2 += this->data[x][i] * this->data[x][i];
					sumY2 += this->data[y][i] * this->data[y][i];
				}
			}

			num = sumXY;

			if (N > 0)
				num -= (sumX * sumY) / N;

			if ((num < 0.00001) && (num > -0.00001))
				num = 0;
			else
				den = sqrt((sumX2 - (sumX*sumX/N)) * (sumY2 - (sumY*sumY/N)));

			num /= den;
            this->data[x][y] = num;
		}
	}
}
void GeneDataPrep::computeMissing()
{
	int rowMissing[this->numRows],
	colMissing[this->numCols];

	for (int i = 0; i < this->numRows; i++)
	{
		for (int j = 0; j < this->numCols; j++)
		{
			if (this->isMissingChar(this->allele[i][j][0]))
			{
				rowMissing[i]++;
				colMissing[j]++;
			}
		}
	}
}
} /* namespace AI */
