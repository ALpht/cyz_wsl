/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <vector>
#define N 1000

using namespace std;

float float_to_fixed (float fp, int INTW, int fracBits)
{
	float upperBound = pow(2.0, INTW - 1) - pow(2.0, -fracBits);
	float lowerBound = -pow(2.0, INTW - 1);
	float scaleFactor = pow(2.0, fracBits);
	
	float temp = fp * scaleFactor;
	float result = floor(fp * scaleFactor) / scaleFactor;

	if(result > upperBound)
		result = upperBound;
	else if(result < lowerBound)
		result = lowerBound;
	else
		result = result;
	
	return result;
}

float *convolution (float filterCoefficient[], float inputs[], int coeLen, int inputLen)
{
  float delayElements[coeLen - 1] = { 0 };
  float *ptr = (float *) malloc (inputLen * sizeof (float));

  for (int i = 0; i < inputLen; i++)
    {
      ptr[i] = delayElements[0] + filterCoefficient[0] * inputs[i];
      for (int j = 0; j < coeLen - 1; j++)
	{
	  delayElements[j] =
	    filterCoefficient[j + 1] * inputs[i] + delayElements[j + 1];
	}
      delayElements[coeLen - 2] = filterCoefficient[coeLen - 1] * inputs[i];
    }

  return ptr;
}

float *read_file(char fileName[])
{
	ifstream inFile;
	inFile.open(fileName);
	float temp;
	static float result[1000];
	float *ptr = &result[0];
	int i = 0;
	
	while(!inFile.eof())
	{
		inFile >> temp;
		if(!inFile.fail())
		{
			result[i] = temp;			
			i++;
		}
	}
	
	return ptr;
}	

void write_file(char fileName[])
{
	std::ofstream ofs;
	
	ofs.open(fileName);
	if(!ofs.is_open())
	{
		cout << "Wirte file fail\n";
	}
	else
	{
		
	}
}

int main ()
{
  float FILTER_COE[23] =
    { 0.013239156, 0.0043691504, -0.022531772, -0.036399439, -0.0046777544,
0.039288234, 0.020806413, -0.059205871, -0.079080485, 0.068135582, 0.30790523, 0.42687139,
0.30790523, 0.068135582, -0.079080485, -0.059205871, 0.020806413, 0.039288234, -0.0046777544,
-0.036399439, -0.022531772, 0.0043691504, 0.013239156 };
  float FIXED_FILTER_COE[23];
  float fixedInputs[N];
  float *floatOutputs = NULL, *fixedOutputs = NULL, *floatInputs =  NULL;
  float outputSNR[15] = { 0 };
  char fileName[] = "rand_data.txt";

  floatInputs = read_file(fileName);

  for (int j = 1; j < 16; j++)
  {
    float powerOfFp = 0, powerOfDiff = 0;

    for (int i = 0; i < N; i++)
	{
	  fixedInputs[i] = float_to_fixed (floatInputs[i], 1, j);
	  //cout << fixedInputs[i] << endl;
	}
	
    for (int i = 0; i < 23; i++)
	{
	  FIXED_FILTER_COE[i] = float_to_fixed (FILTER_COE[i], 1, j);
	}

      floatOutputs = convolution (FILTER_COE, floatInputs, 23, N);
      fixedOutputs = convolution (FILTER_COE, fixedInputs, 23, N);

	  for (int i = 0; i < N; i++)
	{
	  // cout<<ptr1[i]<<", "<<ptr2[i]<<endl;
	  powerOfFp += floatOutputs[i] * floatOutputs[i];
	  powerOfDiff += (floatOutputs[i] - fixedOutputs[i]) * (floatOutputs[i] - fixedOutputs[i]);
	}

      outputSNR[j] = 10 * log10 (powerOfFp / powerOfDiff);
      cout << outputSNR[j] << endl;
    }

  return 0;
}
