#include <stdio.h>
#include <omp.h>
#include <locale.h>
#include <time.h>

const int NMAX = 500;
const int LIMIT = 10;
void main()
{
	setlocale(LC_ALL, "Rus");

	double t1, t2, dt;
	t1 = omp_get_wtime();

	int i, j;
	float sum;
	float a[NMAX][NMAX];
	for (i = 0; i < NMAX; i++)
		for (j = 0; j < NMAX; j++)
			a[i][j] = i + j;
#pragma omp parallel shared(a) if (NMAX>LIMIT)
	{
#pragma omp for private(i,j,sum)
		for (i = 0; i < NMAX; i++)
		{
			sum = 0;
			for (j = 0; j < NMAX; j++)
				sum += a[i][j];
			printf("Сумма элементов строки %d равна %f\n", i, sum);
		}
	}

	t2 = omp_get_wtime();
	dt = t2 - t1;
	printf("Программа работала %f секунд.\n", dt);
}
