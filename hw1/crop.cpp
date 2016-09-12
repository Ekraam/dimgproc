// Homework 1// This code implements the cropping function// Question 1.a.1// This code takes 10 arguments in the following format// program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Width = 256] [Height = 256] [x1 = 55] [y1 = 85] [x2 = 184] [y2 = 214]//// Name: Ekraam Sabir // USC ID: 1428-2892-90// email: esabir@usc.edu// Submission Date: //#include <stdio.h>#include <iostream>#include <stdlib.h>using namespace std;int main(int argc, char *argv[]){	// Define file pointer and variables	FILE *file;	int BytesPerPixel;	int Height = 256;	int Width = 256;	int x1, x2, y1, y2;		// Check for proper syntax	if (argc < 10){		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;		cout << "This program is for image cropping (x1,y1) and (x2,y2) are image cropping coordinates" << endl;		cout << "You must have 10 arguments in the following format" << endl;		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Width = 256] [Height = 256] [x1 = 55] [y1 = 85] [x2 = 184] [y2 = 214]" << endl;		return 0;	}	//Get the command line parameters in the appropriate variables	else{	  BytesPerPixel = atoi(argv[3]);	  Width = atoi(argv[4]);	  Height = atoi(argv[5]);	  x1 = atoi(argv[6]);	  y1 = atoi(argv[7]);	  x2 = atoi(argv[8]);	  y2 = atoi(argv[9]);	}			// Allocate image data array	unsigned char Imagedata[Height][Width][BytesPerPixel];	// Read image (filename specified by first argument) into image data matrix	if (!(file=fopen(argv[1],"rb"))) {		cout << "Cannot open file: " << argv[1] <<endl;		exit(1);	}	fread(Imagedata, sizeof(unsigned char), Height*Width*BytesPerPixel, file);	fclose(file);	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////	//get the height and width of the cropped image	int cropHeight = y2 - y1 + 1;	int cropWidth = x2 - x1 + 1;		//create a blank new iamge array	unsigned char newImage[cropHeight][cropWidth][BytesPerPixel];	//assign values to the cropped image element by element of each channel	for (int row=0; row<cropHeight; row++){	  for (int col=0; col<cropWidth; col++){	    newImage[row][col][0] = Imagedata[row+y1][col+x1][0]; //Red Channel	    newImage[row][col][1] = Imagedata[row+y1][col+x1][1]; //Blue Channel	    newImage[row][col][2] = Imagedata[row+y1][col+x1][2]; //Green Channel	  }	}	// Write image data (filename specified by second argument) from image data matrix	if (!(file=fopen(argv[2],"wb"))) {		cout << "Cannot open file: " << argv[2] << endl;		exit(1);	}	fwrite(newImage, sizeof(unsigned char), cropHeight*cropWidth*BytesPerPixel, file);	fclose(file);	cout << "Cropped successfully with image Width " << cropWidth << " and image Height " << cropHeight << endl;	return 0;}