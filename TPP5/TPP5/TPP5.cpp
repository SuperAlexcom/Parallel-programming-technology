#include <stdio.h>
#include <omp.h>
#include <iostream>
int* init_fill_array(int N) // Инициализация
{
	int* arr = new int[N];
	for (int i = 0; i < N; i++)
	{
		arr[i] = rand() % 10; // рандом от 0 до 10
	}
	return (arr);
}
void func_2_sections(int* A, int* B, int* C, int N, int LIMIT)
{
	double start_time, end_time; // Время
	int total = 0;
	start_time = omp_get_wtime(); // Начало времени
#pragma omp parallel shared(A, B, C, N, total) if(N > LIMIT)
	{
		// Если (N>LIMIT) не выполнится, то
		// будет обычный последовательный код
#pragma omp sections reduction(+: total) // 2 секции
		{
#pragma omp section // 1 секция: с 0 по N / 2
			{
				for (int i = 0; i < N / 2; i++)
				{
					if (A[i] % 2 == 0)
					{
						total += B[i] + C[i];
					}
					else
					{
						if (B[i] - A[i] != 1)
						{
							total += B[i] - A[i];
						}
					}
				}
			}
#pragma omp section // 2 секция: с N / 2 по N
			{
				for (int i = N / 2; i < N; i++)
				{
					if (A[i] % 2 == 0)
					{
						total += B[i] + C[i];
					}
					else
					{
						if (B[i] - A[i] != 1)
						{
							total += B[i] - A[i];
						}
					}
				}
			}
		}
	}
	end_time = omp_get_wtime(); // Конец
	printf("2 sect\n");
	printf("sum of values - %d\n", total);
	printf("N - %d\nLIMIT - %d\n", N, LIMIT);
	printf("time - %lf\n\n", end_time - start_time);
}
void func_4_sections(int* A, int* B, int* C, int N, int LIMIT)
{
	double start_time, end_time; // Время
	int total = 0;
	start_time = omp_get_wtime(); // Начало времени
#pragma omp parallel shared(A, B, C, N, total) if(N > LIMIT)
	{
		// Если (N>LIMIT) не выполнится, то
		// будет обычный последовательный код
#pragma omp sections reduction(+: total) // 4 секции
		{
#pragma omp section // 1 секция: с 0 по N / 4
			{
				for (int i = 0; i < N / 4; i++)
				{
					if (A[i] % 2 == 0)
					{
						total += B[i] + C[i];
					}
					else
					{
						if (B[i] - A[i] != 1)
						{
							total += B[i] - A[i];
						}
					}
				}
			}
#pragma omp section // 2 секция: с N / 4 по N * 2 / 4
			{
				for (int i = N / 4; i < N / 2; i++)
				{
					if (A[i] % 2 == 0)
					{
						total += B[i] + C[i];
					}
					else
					{
						if (B[i] - A[i] != 1)
						{
							total += B[i] - A[i];
						}
					}
				}
			}
#pragma omp section // 3 секция: с N / 2 по N * 3 / 4
			{
				for (int i = N / 2; i < N * 3 / 4; i++)
				{
					if (A[i] % 2 == 0)
					{
						total += B[i] + C[i];
					}
					else
					{
						if (B[i] - A[i] != 1)
						{
							total += B[i] - A[i];
						}
					}
				}
			}
#pragma omp section // 4 секция: с N * 3 / 4 по N
			{
				for (int i = N * 3 / 4; i < N; i++)
				{
					if (A[i] % 2 == 0)
					{
						total += B[i] + C[i];
					}
					else
					{
						if (B[i] - A[i] != 1)
						{
							total += B[i] - A[i];
						}
					}
				}
			}
		}
	}
	end_time = omp_get_wtime(); // Конец
	printf("4 sect\n");
	printf("sum of values - %d\n", total);
	printf("N - %d\nLIMIT - %d\n", N, LIMIT);
	printf("time - %lf\n\n", end_time - start_time);
}
void timing(int N, int step_N)
{
	for (int i = step_N; i < N; i += step_N)
	{
		int* A = init_fill_array(i);
		int* B = init_fill_array(i);
		int* C = init_fill_array(i);
		func_2_sections(A, B, C, i, 1); // параллельно
		func_2_sections(A, B, C, i, i + 1); // последовательно
		func_4_sections(A, B, C, i, 1); // параллельно
		func_4_sections(A, B, C, i, i + 1); // последовательно
		delete[] A;
		delete[] B;
		delete[] C;
	}
}
int main(int argc, char* argv[])
{
	timing(100001, 1000);
	// 1000 <= N < 10001
}
