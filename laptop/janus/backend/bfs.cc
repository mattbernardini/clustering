#include "network.h"
#include "base_logger.h"
#include <string>
static BaseLogger * logger;
using namespace AI;
int main (int argc, char ** argv)
{
	if (argc != 2)
		exit(-1);
	Network * n;
	logger = new BaseLogger("master.log");
	n = new Network((char *)argv[1], logger);
	n->readGmlFile();
	n->bfs();
	n->findClusters();
	n->printFinalClusters();
	return 0;
}

