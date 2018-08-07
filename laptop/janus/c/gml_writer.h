/*
 * gml_writer.h
 *
 *  Created on: Apr 2, 2018
 *      Author: matt
 */
#include <stdio.h>

#ifndef BFS_GML_WRITER_H_
#define BFS_GML_WRITER_H_

namespace AI {

/*
 *
 */
class GmlWriter {
	FILE * outputFile;
	void startFile();
    bool startedEdges;
public:
	GmlWriter(char * outputFileName);
	~GmlWriter();

	void writeNode(int index);
	void writeEdge(int source, int target, double weight);
	void writeEnd();
};

} /* namespace AI */

#endif /* BFS_GML_WRITER_H_ */
