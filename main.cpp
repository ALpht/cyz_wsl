/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <math.h>
#include <time.h>
#define N 10

using namespace std;

float
float_to_fixed (float fp, int fracBits)
{
  float temp = fp * pow (2, fracBits);
  int intTemp = (int) temp;
  float result = intTemp / pow (2, fracBits);
  return result;
}

float *
convolution (float filterCoefficient[], float inputs[], int coeLen,
	     int inputLen)
{
  float delayElements[coeLen - 1] = { 0 };
  //float outputs[N];
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

int
main ()
{
  srand (time (NULL));
  float FILTER_COE[23] =
    { 0.013239156, 0.0043691504, -0.022531772, -0.036399439, -0.0046777544,
0.039288234, 0.020806413, -0.059205871, -0.079080485, 0.068135582, 0.30790523, 0.42687139,
0.30790523, 0.068135582, -0.079080485, -0.059205871, 0.020806413, 0.039288234, -0.0046777544,
-0.036399439, -0.022531772, 0.0043691504, 0.013239156 };
  float FIXED_FILTER_COE[23];
  float inputs[10] =
    { 0.025027, 1.379723, 0.666515, 0.434150, 1.665457, 0.415387, 1.415958,
0.301792, 0.468457, 0.249128 };
  float fixedInputs[10];
  float *ptr1 = NULL, *ptr2 = NULL;
  float outputSNR[15] = { 0 };

  for (int j = 2; j < 16; j++)
    {
      float powerOfFp = 0, powerOfDiff = 0;
      for (int i = 0; i < N; i++)
	{
	  fixedInputs[i] = float_to_fixed (inputs[i], j);
	  cout << fixedInputs[i] << endl;
	}

      ptr1 = convolution (FILTER_COE, inputs, 23, N);
      ptr2 = convolution (FILTER_COE, fixedInputs, 23, N);
      for (int i = 0; i < N; i++)
	{
	  //cout<<ptr1[i]<<", "<<ptr2[i]<<endl;
	  powerOfFp += ptr1[i] * ptr1[i];
	  powerOfDiff += (ptr1[i] - ptr2[i]) * (ptr1[i] - ptr2[i]);
	}

      outputSNR[j - 2] = 10 * log10 (powerOfFp / powerOfDiff);
      cout << outputSNR[j - 2] << endl;
    }

  return 0;
}
