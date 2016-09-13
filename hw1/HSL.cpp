// Homework 1// This code converts RGB to HSL and saves the individual grayscale channels// Question 1.b.2// This code takes 9 arguments in the following format// program_name input_image.raw output_image.raw output_C.raw output_M.raw output_Y.raw [BytesPerPixel = 1] [Width = 256] [Height = 256]// //// Name: Ekraam Sabir // USC ID: 1428-2892-90// email: esabir@usc.edu// #include <stdio.h>#include <iostream>#include <stdlib.h>#include <math.h>#include <algorithm>using namespace std;int main(int argc, char *argv[]){	// Define file pointer and variables	FILE *file;	int BytesPerPixel;	int Height = 256;	int Width = 256;		// Check for proper syntax	if (argc < 8){		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;		cout << "This program is for converting RGB image to HSL" << endl;		cout << "You must have 10 arguments in the following format" << endl;		cout << "program_name input_image.raw output_image.raw output_C.raw output_M.raw output_Y.raw [BytesPerPixel = 1] [Width = 256] [Height = 256]" << endl;		return 0;	}	//Get the command line parameters in the appropriate variables	else{	  BytesPerPixel = atoi(argv[6]);	  Width = atoi(argv[7]);	  Height = atoi(argv[8]);	}			// Allocate image data array	unsigned char Imagedata[Height][Width][BytesPerPixel];	// Read image (filename specified by first argument) into image data matrix	if (!(file=fopen(argv[1],"rb"))) {		cout << "Cannot open file: " << argv[1] <<endl;		exit(1);	}	fread(Imagedata, sizeof(unsigned char), Height*Width*BytesPerPixel, file);	fclose(file);	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////		//create a blank new image array	unsigned char newImage[Height][Width][BytesPerPixel];	unsigned char newImage_H[Height][Width];	unsigned char newImage_S[Height][Width];	unsigned char newImage_L[Height][Width];	//temporary image initialized to store normalized floating values	float tempImage_H[Height][Width];	float tempImage_S[Height][Width];	float tempImage_L[Height][Width];	//initializing variables	float M, m, C;	float H, S, L;	//initialization of intermediate variables	float maxH = -1;	float minH = 5000;	float maxS = -1;	float minS = 5000;	float maxL = -1;	float minL = 5000;	for (int row=0; row<Height; row++){	  for (int col=0; col<Width; col++){	    //RGB scaled to 0-1	    float R = (float)Imagedata[row][col][0]/255;	    float G = (float)Imagedata[row][col][1]/255;	    float B = (float)Imagedata[row][col][2]/255;	    m = min(min(R,G),B);	    M = max(max(R,G),B);	    C = M - m;	    //calculate the H value	    if (C==0)	      H = 0;	    else if (M == R)	      H = 60 * fmod(((G-B)/C), 6);	    else if (M == G)	      H = 60 * ((B-R)/C + 2);	    else if (M == B)	      H = 60 * ((R-G)/C + 4);	    //caluclate the value of L	    L = (M+m)/2;	    //calculate the S value	    if (L==0)	      S = 0;	    else if (0<L && L<0.5)	      S = C/(2*L);	    else	      S = C/(2 - 2*L);	    //store the dynamic range of floating values in a temporary array	    tempImage_H[row][col] = H;	    tempImage_S[row][col] = S;	    tempImage_L[row][col] = L;	    //store the max and min of each channel for normalization	    if (H>maxH)	      maxH=H;	    if (H<minH)	      minH=H;	    if (L<minL)	      minL=L;	    if (L>maxL)	      maxL=L;	    if (S>maxS)	      maxS = S;	    if (S<minS)	    minS = S;	  }	}	//Renormalize and store in the final image	for (int row=0; row<Height; row++){	  for (int col=0; col<Width; col++){	    newImage_H[row][col] = (unsigned char)((tempImage_H[row][col] - minH)/(maxH - minH)*255);	    newImage_S[row][col] = (unsigned char)((tempImage_S[row][col] - minS)/(maxS - minS)*255);	    newImage_L[row][col] = (unsigned char)((tempImage_L[row][col] - minL)/(maxL - minL)*255);	  }	}	//Store a combination of all 3 channels (Not required by assignment)	for (int row=0; row<Height; row++){	  for (int col=0; col<Width; col++){	    newImage[row][col][0] = newImage_H[row][col];	    newImage[row][col][1] = newImage_S[row][col];	    newImage[row][col][2] = newImage_L[row][col];	  }	}	// Write image data (filename specified by second argument) from image data matrix	if (!(file=fopen(argv[2],"wb"))) {		cout << "Cannot open file: " << argv[2] << endl;		exit(1);	}	fwrite(newImage, sizeof(unsigned char), Height*Width*BytesPerPixel, file);	fclose(file);	if (!(file=fopen(argv[3],"wb"))) {		cout << "Cannot open file: " << argv[3] << endl;		exit(1);	}	fwrite(newImage_H, sizeof(unsigned char), Height*Width, file);	fclose(file);	if (!(file=fopen(argv[4],"wb"))) {		cout << "Cannot open file: " << argv[4] << endl;		exit(1);	}	fwrite(newImage_S, sizeof(unsigned char), Height*Width, file);	fclose(file);	if (!(file=fopen(argv[5],"wb"))) {		cout << "Cannot open file: " << argv[5] << endl;		exit(1);	}	fwrite(newImage_L, sizeof(unsigned char), Height*Width, file);	fclose(file);	return 0;}