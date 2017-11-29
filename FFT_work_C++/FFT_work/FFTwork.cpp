/* Factored discrete Fourier transform, or FFT, and its inverse iFFT */
#include "stdafx.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "FFTwork.h"
#include <iostream>
#include <iomanip>
#include <time.h>

using namespace std;


#define q   14	//for 2^14 points 
#define N	(1<<q)// N-point FFT

ifstream inFile;
typedef float real;
typedef struct { real Re; real Im; } complex;

#ifndef PI
# define PI	3.14159265358979323846264338327950288
#endif


/*
fft(v,N):
[0] If N==1 then return.
[1] For k = 0 to N/2-1, let ve[k] = v[2*k]
[2] Compute fft(ve, N/2);
[3] For k = 0 to N/2-1, let vo[k] = v[2*k+1]
[4] Compute fft(vo, N/2);
[5] For m = 0 to N/2-1, do [6] through [9]
[6]   Let w.re = cos(2*PI*m/N)
[7]   Let w.im = -sin(2*PI*m/N)
[8]   Let v[m] = ve[m] + w*vo[m]
[9]   Let v[m+N/2] = ve[m] - w*vo[m]
*/
void fft(complex *v, int n, complex *tmp)
{
	if (n>1) {			/* otherwise, do nothing and return */
		int k, m;    complex z, w, *vo, *ve;
		ve = tmp; vo = tmp + n / 2;
		for (k = 0; k<n / 2; k++) {
			ve[k] = v[2 * k];
			vo[k] = v[2 * k + 1];
		}
		fft(ve, n / 2, v);		/* FFT on even-indexed elements of v[] */
		fft(vo, n / 2, v);		/* FFT on odd-indexed elements of v[] */
		for (m = 0; m<n / 2; m++) {
			w.Re = cos(2 * PI*m / (double)n);
			w.Im = -sin(2 * PI*m / (double)n);
			z.Re = w.Re*vo[m].Re - w.Im*vo[m].Im;	/* Re(w*vo[m]) */
			z.Im = w.Re*vo[m].Im + w.Im*vo[m].Re;	/* Im(w*vo[m]) */
			v[m].Re = ve[m].Re + z.Re;
			v[m].Im = ve[m].Im + z.Im;
			v[m + n / 2].Re = ve[m].Re - z.Re;
			v[m + n / 2].Im = ve[m].Im - z.Im;
		}
	}
	return;
}



int main(void)
{
	//clock_t start = cloc;k();
	complex v[N] = { 0 }, v1[N], scratch[N];
	int k, i = 0, threshold = 0.5;
	double in[N] = {0}, x, mag[N] , peak ,Maxfreq;


	inFile.open("C:\\Users\\Donnacha\\Desktop\\Final-Project\\MatLabWork\\guitardata220HzSineWave.txt");

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}


	while (inFile >> x) {

		v[i].Re = x;
		v[i].Im = 0;
		i++;
	}

	
	fft(v, N, scratch);
	//print_vector(" FFT", v, N);
	
	for (int i = 0; i < N; i++)
	{
		mag[i] = sqrt((pow(v[i].Re, 2)) + (pow(v[i].Im, 2))) /(N*0.5); //get magnitude and normalize
		
	}
	/*
	clock_t stop = clock();
	double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
	printf("Time elapsed in ms: %f", elapsed);
	*/
	peak = mag[0];

	for (int i = 1; i < N - 1; i++)
	{
		if ( mag[i] > peak)
		{
			peak = mag[i];
			Maxfreq = i* 2.6916;
		    
		}
	}

	printf("max peak is %f", Maxfreq);

	exit(EXIT_SUCCESS);
}