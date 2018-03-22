/*
Student Name: Colin Gill
Date: 08-03-2018

Sandbox Console application that emulates realtime audio stream. 
Data is read from file and held in buffer, then split into 512 sample chunks for proccessing.

*/
#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <time.h>
#include<vector>
#include "AutoCorr.h"

#define N 1024
#define BufferSize 512
#define PI 3.14159265358979323846264338327950288

std::ifstream inFile;

int main(void)
{
	//set up variables
	int k, i = 0, threshold = 0.5, bufferFill = 0 , sample=0;
	float in[N] = { 0 }, x, mag[N / 2], Maxfreq = 0, fund = 0, signal[BufferSize] = { 0 };
	const int numOfNotes = 49, dsf = 2, sampleRate = 44100;
	int guitarNoteArr[numOfNotes];
	int midiNoteArr[numOfNotes], counter = 0;
	float guitarNoteStart = 77.782;
	std::vector<float> sigIn;
	float peak = 0, maxPeak = 0, sum = 0;

	//generate midi map of guitar notes
	for (int i = 0; i < numOfNotes; i++)
	{
		midiNoteArr[i] = i+40;
		guitarNoteStart = guitarNoteStart *1.059460968;
		guitarNoteArr[i] = std::round(guitarNoteStart);
	}

	
	inFile.open("C:\\Users\\Donnacha\\Desktop\\Final-Project\\MatLabWork\\EF#G.txt");
	
	if (!inFile) 
	{
		std::cerr << "\nUnable to open file datafile.txt\n";
		exit(1);   // call system to stop
	}

	//while there is data available, store it into vector
	while (inFile >> x) 
	{	
		sigIn.push_back(x);
		counter++;
	}
	
	counter = 0;
	int sizeOfData = sigIn.size() / 512; //get total number of 512 chunks...

	for (int i = 0; i < sizeOfData; i++)
	{

		//data is overlay in 512 chunks... auto Corr is done on 1024 buffer
		for (int bufferFill = 0; bufferFill < N / 2; bufferFill++)
		{
			signal[bufferFill] = sigIn[bufferFill];
			in[bufferFill] = in[bufferFill + 512]; //push old data to start of 1024 buffer
		}
		for (int bufferFill = 512; bufferFill < N; bufferFill++)
		{
			in[bufferFill] = sigIn[sample]; //new data is inserted into middle of 1024 buffer
			sample++;
		}

		AutoCorr a;

		std::vector<float> aCorr = a.performAutoCorr(in, N , dsf );			
		int fundy = std::round(a.getFundamental(aCorr, sampleRate, dsf ));

		std::cout << "fundamental is " << fundy << "\nnumber of interations: " << a.getTotalIterationsTaken() << "--- time taken : " << a.getExecutionTime() << std::endl;

		//find associated midi note
		for (int i = 0; i < numOfNotes; i++)
		{
			int max = std::round(guitarNoteArr[i] * 1.03);
			int min = std::round(guitarNoteArr[i] / 1.03);
			if (fundy <= max && fundy >= min)
			{
				std::cout << "midi note is " << midiNoteArr[i] << std::endl;
				break;
			}
		}		
	}
	exit(EXIT_SUCCESS);
}

	

	

	