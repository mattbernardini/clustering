/*
 * gene_data_prep.h
 *
 *  Created on: Apr 2, 2018
 *      Author: matt
 */
#include <stdio.h>
#include <map>
#ifndef BFS_GENE_DATA_PREP_H_
#define BFS_GENE_DATA_PREP_H_

namespace AI {
const int MAX_VAL = 100000000;
const int MISSING_SYMBOL = 1;
const int BUF_SIZE = 512;
/*
 *
 */
class GeneDataPrep {
private:
	float ** data;
	short ** snpData;
	FILE * inputFile;
	std::map<char, int> * alleleMap;
	long int numMissing;
	bool space;
	bool slash;
	bool format;
	bool rowColFlipped;
	char buffer[BUF_SIZE];
	char ** allele;
	int numRows;
	int numCols;
	int numHeaderRows;
	int numHeaderCols;
	void readNextString();
	void permuteGeneData();
	void computePearsonCorrolation();
	void readHeaderRows();
	void createSnpData();
	void determineFormat();
	void determineAlleleTypes();
	void loadSnpData();
	void computeMissing();
	bool isValidNum (float num);
	bool isValidChar (int c);
	bool isMissingChar (int c);
public:
	GeneDataPrep(
			int numHeaderRows,
			int numHeaderCols,
			int numRows,
			int numCols,
			char * inputFileName);
	virtual ~GeneDataPrep();
	void parseInputFile();
};

} /* namespace AI */

#endif /* BFS_GENE_DATA_PREP_H_ */
