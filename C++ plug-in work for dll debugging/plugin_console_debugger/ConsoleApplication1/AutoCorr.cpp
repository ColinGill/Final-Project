#include "AutoCorr.h"
#include <iostream>
#include <time.h>
#include <chrono>

AutoCorr::AutoCorr()
{

}

AutoCorr::~AutoCorr()
{
}

std::vector<float> AutoCorr::performAutoCorr(float signal [], int NumOfSamples, const int dsf)
{

	auto start = std::chrono::high_resolution_clock::now();

	for (int n = 0; n < NumOfSamples; n = n + dsf)
	{

		for (int i = n; i < NumOfSamples; i = i + dsf)
		{
			sum = sum + (signal[i] * signal[i - n]);
			interations++;
		}

		autoCorrOutput.push_back(sum);
		sum = 0;
		
	}
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	timeTaken = microseconds;

	return autoCorrOutput;
}

float AutoCorr::getFundamental(std::vector<float> autoCorrIn, const int sampleRate, const int dsf)
{

	peak = 0;
	maxPeak = 0;

	for (int i = 1; i < autoCorrIn.size() - 1; i++)
	{

		if ((autoCorrIn.at(i) > autoCorrIn.at(i + 1)) && (autoCorrIn.at(i) > autoCorrIn.at(i - 1)))
		{

			if (autoCorrIn.at(i) > peak) //find highest peak
			{
				peak = autoCorrIn.at(i);
				maxPeak = i;
			}
		}
	}

	if (maxPeak == 0) { fund = 1; return fund; }

	else
	{
		fund = (sampleRate / dsf) / maxPeak; //convert to frequency
		return fund;
	}

}
