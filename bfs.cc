#include "network.h"
#include "base_logger.h"
#include <string>
#include <mpi.h>
static BaseLogger * logger;
using namespace AI;
int main (int argc, char ** argv)
{
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	fprintf(stderr, "Rank: %i\n", world_rank);
	if (argc != 2)
		exit(-1);
	if (world_rank == 0)
	{
		logger = new BaseLogger("master.log");
		Network * n = new Network((char *)argv[1], logger);
		n->readGmlFile();
		n->bfs();
		n->findClusters();
		n->printFinalClusters();
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}

