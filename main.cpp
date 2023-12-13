#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#define N 1000

using namespace std;

// floating point to fixed point conversion
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

// convolution operation
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

// read .txt file 
float *read_file(char fileName[], int len)
{
	ifstream inFile;
	inFile.open(fileName);
	float temp;
	float *ptr = (float *) malloc(len * sizeof(float));
	int i = 0;
	
	while(!inFile.eof())
	{
		inFile >> temp;
		if(!inFile.fail())
		{
			ptr[i] = temp;			
			i++;
		}
	}
	inFile.close();
	
	return ptr;
}	

int main (int argc, char* argv[])
{
  /*float FILTER_COE[23] =
    { 0.013239156, 0.0043691504, -0.022531772, -0.036399439, -0.0046777544,
0.039288234, 0.020806413, -0.059205871, -0.079080485, 0.068135582, 0.30790523, 0.42687139,
0.30790523, 0.068135582, -0.079080485, -0.059205871, 0.020806413, 0.039288234, -0.0046777544,
-0.036399439, -0.022531772, 0.0043691504, 0.013239156 };*/
  float *FILTER_COE = NULL;
  float FIXED_FILTER_COE[23];
  float powerOfFp = 0;
  float fixedInputs[N];
  float *floatOutputs = NULL, *fixedOutputs = NULL, *floatInputs =  NULL;
  float *semiOutputs = NULL;
  float outputSNR1[15] = { 0 }, outputSNR2[15] = {0};
  std::ofstream ofs;

  floatInputs = read_file(argv[1], N);
  FILTER_COE = read_file(argv[2], 23);

  floatOutputs = convolution (FILTER_COE, floatInputs, 23, N);
  for (int i = 0; i < N; i++)
  {
    powerOfFp += floatOutputs[i] * floatOutputs[i];
  }

  for (int j = 1; j < 16; j++)
  {
    float powerOfDiff = 0, powerOfDiffs = 0;

    for (int i = 0; i < N; i++)
	{
	  fixedInputs[i] = float_to_fixed (floatInputs[i], 1, j);
	}
	
    for (int i = 0; i < 23; i++)
	{
	  FIXED_FILTER_COE[i] = float_to_fixed (FILTER_COE[i], 1, j);
	}

      semiOutputs = convolution (FILTER_COE, fixedInputs, 23, N);
      fixedOutputs = convolution (FIXED_FILTER_COE, fixedInputs, 23, N);	  

	  for (int i = 0; i < N; i++)
	{
	  powerOfDiff += (floatOutputs[i] - fixedOutputs[i]) * (floatOutputs[i] - fixedOutputs[i]);
	  powerOfDiffs += (floatOutputs[i] - semiOutputs[i]) * (floatOutputs[i] - semiOutputs[i]);
	}

      outputSNR1[j] = 10 * log10 (powerOfFp / powerOfDiffs);
      outputSNR2[j] = 10 * log10 (powerOfFp / powerOfDiff);
      cout << outputSNR1[j] << ", " << outputSNR2[j] <<endl;
    }

	ofs.open("output_SNR_1.txt");
	if(!ofs.is_open())
	{
		cout << "Write file fail" << endl;
	}
	else
	{
	  for (int i = 0; i < 15; i++)
	  {
		  ofs << outputSNR1[i] << endl;
	  }
	}
	ofs.close();
	
	ofs.open("output_SNR_2.txt");
	if(!ofs.is_open())
	{
		cout << "Write file fail" << endl;
	}
	else
	{
	  for (int i = 0; i < 15; i++)
	  {
		  ofs << outputSNR2[i] << endl;
	  }
	}
	ofs.close();

  return 0;
}
