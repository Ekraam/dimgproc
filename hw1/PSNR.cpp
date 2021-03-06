// Homework 1
// This code measures PSNR
// Question 3.a
// This code takes 6 arguments in the following format
// program_name input_image.raw input_clean_image.raw [BytesPerPixel = 1] [Width = 256] [Height = 256]
// 
//
// Name: Ekraam Sabir 
// USC ID: 1428-2892-90
// email: esabir@usc.edu
// 

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Height = 256;
	int Width = 256;
	
	// Check for proper syntax
	if (argc < 6){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "This program measures PSNR" << endl;
		cout << "You must have 6 arguments in the following format" << endl;
		cout << "program_name input_noisy_image.raw input_clean_image.raw [BytesPerPixel = 1] [Width = 256] [Height = 256]" << endl;
		return 0;
	}
	//Get the command line parameters in the appropriate variables
	else{
	  BytesPerPixel = atoi(argv[3]);
	  Width = atoi(argv[4]);
	  Height = atoi(argv[5]);
	}
		
	// Allocate image data array
	unsigned char Imagedata[Height][Width][BytesPerPixel];
	unsigned char cleanImage[Height][Width][BytesPerPixel];

	// Read noisyImage into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Height*Width*BytesPerPixel, file);
	fclose(file);

	// Read cleanImage into image data matrix
	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[2] <<endl;
		exit(1);
	}
	fread(cleanImage, sizeof(unsigned char), Height*Width*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	
	for (int channel=0; channel<BytesPerPixel; channel++){

	  float MSE = 0;
	  
	  for (int row=0; row<Height; row++){
	    for (int col=0; col<Width; col++){
	      MSE += pow(cleanImage[row][col][channel] - Imagedata[row][col][channel], 2);
	    }
	  }
	
	  MSE = MSE/(Height*Width);
	  
	  float noise = 10*log10( pow(255, 2)/MSE );	
	  
	  cout << "The PSNR for channel " << channel << " is " << noise << endl;

	}
	
}
