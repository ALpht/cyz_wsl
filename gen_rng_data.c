#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#define N 1000

int main()
{
	srand(time(NULL));
	float data[N] = {0};
	float count = 0;
	int i = 0;
	FILE  *fp;

	fp = fopen("rand_data.txt", "w");

	for(i = 0; i < N; i++)
	{
		//data[i] = (float) rand() / (RAND_MAX + 1.0);
		data[i] = ((float) rand() / RAND_MAX) * 2.0 - 1.0;
		count += data[i] * data[i];
	}
	

	if(fp == NULL)
	{
		printf("Open file fail\n");
		return 1;
	}
	else
	{
		for(i = 0; i < N; i++)
		{
			data[i] = data[i] / sqrt(count / N);
			fprintf(fp, "%f ", data[i]);
		}
	}

	fclose(fp);
	return 0;
}

