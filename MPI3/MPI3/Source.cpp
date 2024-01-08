#include <mpi.h>
#include <iostream>
int main(int argc, char* argv[]) {
	int num_elements;
	int num_trials = atoi(argv[1]);
	
	MPI_Init(&argc, &argv);
	int world_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &num_elements);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	double total_mpi_bcast_time = 0.0;
	int i;
	int* senddata = new int[num_elements];
	int* recvdata = new int[num_elements];

	for (i = 0; i < num_trials; i++)
	{
		// Time MPI_Bcast
		if (world_rank == 0)
			senddata[i] = i;
		MPI_Barrier(MPI_COMM_WORLD);
		total_mpi_bcast_time -= MPI_Wtime();
		MPI_Scatter(senddata + i, 1, MPI_INT, recvdata + i , 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Gather(recvdata + i, 1, MPI_INT, senddata+i, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		total_mpi_bcast_time += MPI_Wtime();
	}

	// Print off timing information
	if (world_rank == 0)
	{
		for (auto i = 0u; i < num_elements; i++)
			std::cout << recvdata[i]*num_elements << " ";
		std::cout << "\nAvg MPI_Bcast time = " << total_mpi_bcast_time / num_trials << std::endl;
	}

	int status = MPI_Finalize();
	delete[] senddata;
	delete[] recvdata;
	return 0;
}