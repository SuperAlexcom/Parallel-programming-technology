#include "mpi.h"
#include <stdio.h> 
#include <time.h>
#include <random>
#include <iostream>
#define N 1
using namespace std;

int main(int* argc, char** argv)
{
	int rank, size, dims[N];
	int periods[N], source, dest;
	int reorder = 0;

	MPI_Comm comm_cart;
	MPI_Status status;
	MPI_Init(argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm comm, comm1, comm2, StarComm;
	MPI_Group group, group1, group2;
	int size1 = 7; //кол-во процессов

	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	MPI_Comm_group(comm, &group);

	int* ranks = new int[7]; // для кольца 
	for (int i = 0; i < 7; i++) {
		ranks[i] = i;
	}
	MPI_Group_incl(group, 7, ranks, &group1); // создание новой группы group1 из существующей группы global
	MPI_Comm_create(comm, group1, &comm1);
	int* arr1, * array;
	int* arr, * arr_res;
	int rank1[1], rank2[6], rank0[6];
	int n = 5;

	arr1 = (int*)malloc(size1 * sizeof(int));
	for (int i = 0; i < size1; i++)
	{
		arr1[i] = 0; // создали массив, заполняем нулями 
	}

	for (int i = 0; i < N; i++) {
		dims[i] = 0;
		periods[i] = 1;
	}

	if (rank < 7)
	{
		// dims размер решетки единичная 
		MPI_Dims_create(size1, N, dims); //создание кольца 
		MPI_Cart_create(comm1, N, dims, periods, reorder, &comm_cart);
		// Каждая ветвь находит своих соседей вдоль кольца, в направлении больших значений рангов 
		MPI_Cart_shift(comm_cart, 0, 1, &source, &dest); //инитит для каждого процесса 
		if (rank == 0)
		{
			MPI_Send(&size1, 1, MPI_INT, dest, 1, comm_cart);
			MPI_Send(arr1, size1, MPI_INT, dest, 1, comm_cart);

			MPI_Recv(&size, 1, MPI_INT, source, 1, comm_cart, &status);
			array = (int*)malloc(size1 * sizeof(int));
			MPI_Recv(array, size1, MPI_INT, source, 1, comm_cart, &status);
			cout << "(circle) \nArray: \n";
			for (int i = 0; i < size1; i++) {
				cout << array[i] << " -> ";
			}
			cout << array[0] << endl << endl;
		}
		else {
			MPI_Recv(&size1, 1, MPI_INT, source, 1, comm_cart, &status);
			MPI_Recv(arr1, size1, MPI_INT, source, 1, comm_cart, &status);
			for (int i = 0; i < size1; i++)
			{
				if (i == rank)
					arr1[i] = rank;
			}
			MPI_Send(&size1, 1, MPI_INT, dest, 1, comm_cart);
			MPI_Send(arr1, size1, MPI_INT, dest, 1, comm_cart);
		}
	}

	int* ranks1 = new int[6];  // для звезды 
	for (int i = 0; i < 6; i++) {
		ranks1[i] = 7 + i;
	}
	MPI_Group_incl(group, 6, ranks1, &group2); // создание новой группы group2 из существующей группы global
	MPI_Comm_create(comm, group2, &comm2);

	int index[] = { 1, 2, 3, 4, 5, 10 };  // кол-во ребер у вершин 

	int edges[] = { 5, 5, 5, 5, 5, 0, 1, 2, 3, 4 }; // вершины соединены  5-0, 5-1, 5-2, 5-3, 5-4


	if (rank >= 7)
	{
		rank = rank - 7;
		MPI_Graph_create(comm2, 6, index, edges, 1, &StarComm); // создаю коммутатор comm2
		MPI_Graphdims_get(StarComm, index, edges);
		MPI_Graph_neighbors(StarComm, rank, 5, rank2);
		if (rank == 5) { // это если 5 он главный 
			arr = (int*)malloc(n * sizeof(int));
			for (int i = 0; i < n; i++) // создаем массив заполняем 5 
			{
				arr[i] = 5;
			}
			MPI_Graph_neighbors(StarComm, rank, 5, rank0);
			for (int i = 0; i < 5; i++) // рассылаем этот массив 
			{
				MPI_Send(arr, n, MPI_INT, rank0[i], 1, StarComm);
			}
			arr_res = (int*)malloc(n * sizeof(int));
			cout << "(star) \nResult: \n\n";
			for (int i = 0; i < 5; i++)
			{
				cout << "Process " << rank0[i] << " \n " << endl;
				MPI_Recv(arr_res, n, MPI_INT, rank0[i], 1, StarComm, &status);
				for (int i = 0; i < n; i++)
				{
					cout << arr_res[i] << " ";
				}
				cout << "\n " << endl;
			}
		}
		else {
			arr = (int*)malloc(n * sizeof(int));
			MPI_Graph_neighbors(StarComm, rank, 1, rank1); // находит ранги процессов, являющихся соседями указанного процесса (массив целых чисел)
			MPI_Recv(arr, n, MPI_INT, rank1[0], 1, StarComm, &status);

			for (int i = 0; i < n; i++)
			{
				if (i == rank)
					arr[i] = rank;

			}
			MPI_Send(arr, 5, MPI_INT, rank1[0], 1, StarComm);
		}
	}

	MPI_Comm_free(&comm);
	MPI_Finalize();
	return 0;
}