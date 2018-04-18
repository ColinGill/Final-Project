
#include <iostream>

#define SIZE 1024
#define SAMPLE_LENGTH 128

int main()
{
	float input[SAMPLE_LENGTH];
	int numSamples = SAMPLE_LENGTH;
	float bufferSize_2[SIZE] = { 0 };
	float  p = 0.0;



	for (int i = 0; i < 10 ; i++)
	{
		for (int i = 0; i < numSamples; i++, p++)
		{
			input[i] = p;
		}
		for (int i = 0; i < (SIZE/numSamples)-1; i++)
		{
			for (int fillHalfBuffer = 0; fillHalfBuffer < numSamples; fillHalfBuffer++)
			{
				bufferSize_2[fillHalfBuffer + (numSamples * i)] = bufferSize_2[fillHalfBuffer + (numSamples* (i+1))];
			}
			
		}
		for (int i = 0; i < numSamples; i++)
		{
			bufferSize_2[(SIZE - numSamples) + i] = input[i];
		}
		
	}

	std::cout << "finished!";
}

