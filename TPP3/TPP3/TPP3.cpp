#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <clocale>

const int NMAX = 10;

using namespace std;

void main()
{
	setlocale(LC_ALL, "rus");
	int i, j;
	long long int total = 0;
	long long int sum = 0;
	int** A = new int* [NMAX];
	for (i = 0; i < NMAX; ++i) {
		A[i] = new int[NMAX];
	}

	int** B = new int* [NMAX];
	for (i = 0; i < NMAX; ++i) {
		B[i] = new int[NMAX];
	}

	int** C = new int* [NMAX];
	for (i = 0; i < NMAX; ++i) {
		C[i] = new int[NMAX];
	}

	for (i = 0; i < NMAX; i++)
		for (j = 0; j < NMAX; j++)
		{
			A[i][j] = (i + j) % 10;
			B[i][j] = (i + j + 1) % 10;
			C[i][j] = (i + j + 2) % 10;
		}
	double start_time = omp_get_wtime();
#pragma omp parallel shared(A,B,C, total) 
	{
#pragma omp for  lastprivate(i, j, sum)
		for (i = 0; i < NMAX; i++)
		{
			sum = 0;
			for (j = 0; j < NMAX; j++)
			{
				if (A[i][j] % 2 == 0) sum += B[i][j] + C[i][j];
				else sum += B[i][j] - A[i][j];
			}
			//printf("Сумма элементов %d строки равна: %d\n", i, sum);
#pragma omp critical
			total += sum;
		}
	}
	printf("\nsum: %d", total);
	printf("\nВремя: %lf\n", omp_get_wtime() - start_time);

	total = 0;
	start_time = omp_get_wtime();
#pragma omp parallel shared(A,B,C, total) 
	{
#pragma omp for  lastprivate(i, j, sum)
		for (i = 0; i < NMAX; i++)
		{
			sum = 0;
			for (j = 0; j < NMAX; j++)
			{
				if (A[i][j] % 2 == 0) sum += B[i][j] + C[i][j];
				else sum += B[i][j] - A[i][j];
			}
			//printf("Сумма элементов %d строки равна: %d\n", i, sum);
#pragma omp atomic
			total += sum;
		}
	}
	printf("\nsum: %d", total);
	printf("\nВремя: %lf", omp_get_wtime() - start_time);

	for (int i = 0; i < NMAX; ++i) delete[] A[i], B[i], C[i];
	delete[] A, B, C;
}
