// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
/* Factored discrete Fourier transform, or FFT, and its inverse iFFT */
#include "stdafx.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <time.h>
#include<vector>
#define N 1024

std::ifstream inFile;

#ifndef PI
# define PI	3.14159265358979323846264338327950288
#endif

int main(void)
{

	int k, i = 0, threshold = 0.5;
	float in[N] = { 0 }, x, mag[N / 2], Maxfreq=0, fund = 0;
	const int numOfNotes = 49, dsf = 8;
	int guitarNoteArr[numOfNotes];
	int midiNoteArr[numOfNotes], counter = 0;
	float guitarNoteStart = 77.782;

	for (int i = 0; i < numOfNotes; i++)
	{
		midiNoteArr[i] = i+40;
		guitarNoteStart = guitarNoteStart *1.059460968;
		guitarNoteArr[i] = std::round(guitarNoteStart);
	}

	inFile.open("C:\\Users\\Donnacha\\Desktop\\Final-Project\\MatLabWork\\88Hz_1024_samples_guitar.txt");
	
	if (!inFile) 
	{
		std::cerr << "\nUnable to open file datafile.txt\n";
		exit(1);   // call system to stop
	}

	while (inFile >> x) 
	{
		i++;
		in[i] = x;
		
	}
	
	std::vector<float> autoCorrOut;
	float peak = 0, maxPeak = 0, sum = 0;

	clock_t t= clock();

	for (int n = 0; n < N; n = n+dsf)
	{

		for (int i = n; i < N; i = i +dsf)
		{
			sum = sum + (in[i] * in[i - n]);
			counter++;
		}
		//std::cout << "num of loops: " << counter << std::endl;
		autoCorrOut.push_back(sum);
		sum = 0;
	}
	t =  clock() - t;

	for (int i = 1; i < autoCorrOut.size() - 1; i++)
	{

		if ((autoCorrOut.at(i) > autoCorrOut.at(i + 1)) && (autoCorrOut.at(i) > autoCorrOut.at(i - 1)))
		{

			if (autoCorrOut.at(i) > peak)
			{
				peak = autoCorrOut.at(i);
				maxPeak = i;
			}
		}
	}

	fund = (44100/dsf)/ maxPeak;
	int fundy = std::round(fund);
	autoCorrOut.clear();
	std::cout << "fundamental is " << fundy << "\nnumber of interations: "<< counter << "--- time taken : "<< (((float)t) / CLOCKS_PER_SEC) << std::endl;
	int max;
	int  min;
	for (int i = 0; i < numOfNotes; i++)
	{
		 max = std::round(guitarNoteArr[i] * 1.03);
		 min = std::round(guitarNoteArr[i] / 1.03);
		if (fundy <= max && fundy >= min)
		{
			std::cout << "midi note is " << midiNoteArr[i] << std::endl;
			break;
		}
	}


	exit(EXIT_SUCCESS);

}

	

	

	