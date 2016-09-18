// Homework 1// This code attempts implements NLM// Question 3.b// This code takes 6 arguments in the following format// program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Width = 256] [Height = 256]// //// Name: Ekraam Sabir // USC ID: 1428-2892-90// email: esabir@usc.edu// #include <stdio.h>#include <iostream>#include <stdlib.h>#define _USE_MATH_DEFINES#include <math.h>#include <cmath>#include <vector>#include <algorithm>using namespace std;int main(int argc, char *argv[]){	// Define file pointer and variables	FILE *file;	int BytesPerPixel;	int Height = 256;	int Width = 256;	int searchSize = 10;	int winSize = 3;	int sigma = 1;		// Check for proper syntax	if (argc < 9){		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;		cout << "This program performs denoising" << endl;		cout << "You must have 9 arguments in the following format" << endl;		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Width = 256] [Height = 256] [searchSize = 10] [winSize = 3] [sigma = 1]" << endl;		return 0;	}	//Get the command line parameters in the appropriate variables	else{	  BytesPerPixel = atoi(argv[3]);	  Width = atoi(argv[4]);	  Height = atoi(argv[5]);	  searchSize = atoi(argv[6]);	  winSize = atoi(argv[7]);	  sigma = atoi(argv[8]);	}			// Allocate image data array	unsigned char Imagedata[Height][Width][BytesPerPixel];	// Read noisyImage into image data matrix	if (!(file=fopen(argv[1],"rb"))) {		cout << "Cannot open file: " << argv[1] <<endl;		exit(1);	}	fread(Imagedata, sizeof(unsigned char), Height*Width*BytesPerPixel, file);	fclose(file);	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////		//create blank new image arrays	unsigned char newImage[Height][Width][BytesPerPixel];	//value of h according to paper	int h = 10*sigma;		int pixelsPerWindow = (2*winSize+1)*(2*winSize+1);	//get a copy of the input image with 0's padded to boundaries	int paddingNeeded = 2*winSize;	vector<vector<vector<int> > > temp4Image; //temporary Image initialization	temp4Image.resize(Height + paddingNeeded);	for (int i=0; i<(Height+paddingNeeded); i++){	  temp4Image[i].resize(Width + paddingNeeded);	  for (int j=0; j<(Width+paddingNeeded); j++){	    temp4Image[i][j].resize(BytesPerPixel);	  }	}	cout << "Temporary image matrix created " << endl;	//populate the temporary image	for (int channel=0; channel<BytesPerPixel; channel++){	  for (int row=0; row<(Height+paddingNeeded); row++){	    for (int col=0; col<(Width+paddingNeeded); col++){	      //0's padded on edges	      bool notOnEdge = true;	      //check if on edge	      for (int i=0; i<winSize; i++){		if (i==row){		  temp4Image[row][col][channel] = 0;		  notOnEdge = false;		}		else if (i==col){		  temp4Image[row][col][channel] = 0;		  notOnEdge = false;		}	      }	      //check if not on edge	      if (notOnEdge)		temp4Image[row][col][channel] = Imagedata[row-winSize][col-winSize][channel];	    }	  }	}	cout << "Temporary image matrix populated " << endl;	//loop through all the pixels of the image	for (int iRow=winSize; iRow<(Height+winSize); iRow++){	  for (int iCol=winSize; iCol<(Width+winSize); iCol++){	    	    //get the coordinates for the area to search in the image	    //searching over entire image is computationally expensive	    int startRow = max(iRow - searchSize, winSize);	    int endRow = min(iRow + searchSize, Height+winSize-1);	    int startCol = max(iCol - searchSize, winSize);	    int endCol = min(iCol + searchSize, Width+winSize-1);	    //some intermediate variables	    float Zi = 0;	    float finalPixelVal[BytesPerPixel];	    for (int channel=0; channel<BytesPerPixel; channel++)	      finalPixelVal[channel] = 0;	    	    //iterate over the search space for similar windows	    for (int jRow=startRow; jRow<=endRow; jRow++){	      for (int jCol=startCol; jCol<=endCol; jCol++){		float eucDist = 0;				//in each window find the euclidian distance		for (int row=-1*winSize; row<=winSize; row++){		  for (int col=-1*winSize; col<=winSize; col++){		    for (int channel=0; channel<BytesPerPixel; channel++){		      //cout << "Here 1 " << endl;		      //cout << iRow+row << " " << iCol+col << " " << channel << " " << jRow+row << " " << jCol+col << endl;		      float diff = temp4Image[iRow+row][iCol+col][channel] - temp4Image[jRow+row][jCol+col][channel];		      //cout<< "Here 2 " << endl;		       eucDist += pow(diff,2);		    }		  }		}				//apply the exponent to the distance computed above		float dist_ij = (float)eucDist/(pixelsPerWindow*BytesPerPixel);		float Wij = (float)exp( -1*max(dist_ij-pow(2*sigma,2), 0.0)/pow(h,2) );		//update the sum of weight		Zi += Wij;				//start modifying the final pixel Val		for (int channel=0; channel<BytesPerPixel; channel++){		  finalPixelVal[channel] += temp4Image[jRow][jCol][channel]*Wij;		}	      }	    }	    	    //store the final computed values in the image after normalizing with the sum of weight	    for (int channel=0; channel<BytesPerPixel; channel++){	      newImage[iRow-winSize][iCol-winSize][channel] = (unsigned char)(finalPixelVal[channel]/Zi);	    }	    	  }	}	// Write image data (filename specified by second argument) from image data matrix	if (!(file=fopen(argv[2],"wb"))) {		cout << "Cannot open file: " << argv[2] << endl;		exit(1);	}	fwrite(newImage, sizeof(unsigned char), Height*Width*BytesPerPixel, file);	fclose(file);	return 0;}