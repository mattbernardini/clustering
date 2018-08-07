#include "Network.h"
#include "base_logger.h"
#include <string>
static BaseLogger * logger;
using namespace AI;
int main (int argc, char ** argv)
{
	if (argc != 2)
		exit(-1);
	logger = new BaseLogger("master.log");
	Network * n = new Network((char *)argv[1], logger);
	return 0;
}

