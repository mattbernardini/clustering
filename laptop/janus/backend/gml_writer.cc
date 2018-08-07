/*
 * gml_writer.cpp
 *
 *  Created on: Apr 2, 2018
 *      Author: matt
 */

#include "gml_writer.h"
#include <stdlib.h>
namespace AI {

GmlWriter::GmlWriter(char * outputFileName)
{
	this->outputFile = fopen(outputFileName, "w");
	this->startFile();
    this->startedEdges = false;
}

GmlWriter::~GmlWriter()
{
	fprintf(this->outputFile, "]\n");
	fflush(this->outputFile);
	fclose(this->outputFile);
}
void GmlWriter::writeEnd()
{
	fprintf(this->outputFile, "]\n");
	fflush(this->outputFile);
}
void GmlWriter::startFile()
{
    fprintf(this->outputFile, "\ngraph\n[\n");
	fflush(this->outputFile);
}
void GmlWriter::writeNode(int index)
{
    if (this->startedEdges)
    {
        // TODO: Handle error
        exit(-1);
    }
	fprintf(this->outputFile, "\tnode \n\t[\n\tid %d \n\t]\n", index);
	fflush(this->outputFile);
}
void GmlWriter::writeEdge(int source, int target, double weight)
{
    this->startedEdges = true;
	fprintf(this->outputFile, 
            "\tedge\n\t[\n\tsource %d\n\ttarget %d\n\tweight %f\n\t]\n", 
            source, 
            target, 
            weight);
	fflush(this->outputFile);
}
} /* namespace AI */
