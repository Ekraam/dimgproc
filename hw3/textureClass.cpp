// Homework 3// This code implements // Question // This code takes 11 arguments in the following format// program_name input_image.raw logo_image.raw output_image.raw [BytesPerPixel = 1] [Width = 256] [Height = 256] [BytesPerPixel_Logo = 1] [Width_Logo = 256] [Height_Logo = 256] [background = 1]// //// Name: Ekraam Sabir // USC ID: 1428-2892-90// email: esabir@usc.edu// #include <opencv2/core/core.hpp>#include <opencv2/highgui/highgui.hpp>#include <fstream>#include <stdio.h>#include <iostream>#include <stdlib.h>#include <sstream>#define _USE_MATH_DEFINES#include <math.h>#include <cmath>#include <vector>#include <string>#include <algorithm>#include <limits.h>using namespace std;using namespace cv;extern int *k_means(double**, int, int, int, double, double**);double getEnergyVal(int filt1[5], int filt2[5], int** img){  int filt[5][5];  // compute the coefficients of the filters  int coeff1 = 0;  int coeff2 = 0;  for (int i=0; i<5; i++){    coeff1 += filt1[i];    coeff2 += filt2[i];  }  int coeff = (coeff1+2)*(coeff2+2);  // muiltiply both the filters to get the 2d filter filt1'*filt2  for (int row=0; row<5; row++){    for (int col=0; col<5; col++){      filt[row][col] = filt1[row]*filt2[col];    }  }  // get the DC component (mean) of the image  double DC = 0;  for (int row=0; row<128; row++){    for (int col=0; col<128; col++){      DC += img[row+2][col+2];    }  }  DC = DC/(128*128);  /*  // subtract DC component  for (int row=0; row<132; row++){    for (int col=0; col<132; col++){      img[row][col] -= DC;    }  }  */  // dummy energy response image  double** Energy = new double*[128];  for (int i=0; i<128; i++){    Energy[i] = new double[128];  }  // initialize the Energy matrix  for (int row=0; row<128; row++){    for (int col=0; col<128; col++){      Energy[row][col] = 0;    }  }  // fill it energy matrix with values  for (int row=0; row<128; row++){    for (int col=0; col<128; col++){            // multiply each filter after subtracting DC component      for (int i=0; i<5; i++){	for (int j=0; j<5; j++){	  Energy[row][col] += filt[i][j]*(img[row+i][col+j]-DC);	}      }      Energy[row][col] /= coeff;      //interAvg += Energy[row][col];      /*if (filt1[0]==filt2[0] && filt1[1]==filt2[1] && filt1[0]==1 && filt1[1]==-4){	cout << interAvg << " " << Energy[row][col] << endl;	}*/    }  }  // get the average of the Energy  double AvgEnergy = 0;  for (int row=0; row<128; row++){    for (int col=0; col<128; col++){      AvgEnergy += Energy[row][col]*Energy[row][col];    }  }  AvgEnergy = AvgEnergy/(128*128);  // release Energy memory  for (int i=0; i<128; i++){    delete [] Energy[i];  }  delete [] Energy;  return AvgEnergy;}double* getEnergyVec(int** img){  int L[5] = {1, 4, 6, 4, 1};  int E[5] = {-1, -2, 0, 2, 1};  int S[5] = {-1, 0, 2, 0, -1};  int W[5] = {-1, 2, 0, -2, 1};  int R[5] = {1, -4, 6, -4, 1};  int** filtbank = new int*[5];  for (int i=0; i<5; i++){    filtbank[i] = new int[5];  }  filtbank[0] = L;  filtbank[1] = E;  filtbank[2] = S;  filtbank[3] = W;  filtbank[4] = R;  double* EnergyVec = new double[25];    int counter=0;  for (int i=0; i<5; i++){    for (int j=0; j<5; j++){      EnergyVec[counter] = getEnergyVal(filtbank[i], filtbank[j], img);      counter++;    }  }  /*for (int i=0; i<5; i++){    delete [] filtbank[i];    }*/  delete [] filtbank;  return EnergyVec;}int main(int argc, char *argv[]){  FILE *file;    // Allocate image data array  unsigned char tempImage[128][128];  int** texture1 = new int*[128+4];  for (int i=0; i<128+4; i++){    texture1[i] = new int[128+4];  }  // Allocate 12x25 feature vector  double** eVec25 = new double*[12];  for (int i=0; i<12; i++){    eVec25[i] = new double[25];  }  // array of image names  string basename = "./HW3 Images/P1/Texture";  string extension = ".raw";  ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////    for (int imgNum=1; imgNum<=12; imgNum++){    stringstream ss;    ss << imgNum;    string num = ss.str();    string image = basename+num+extension;    const char* imageName = image.c_str();    // Read Image into image data matrix    if (!(file=fopen(imageName,"rb"))) {      cout << "Cannot open file: " << imageName << endl;      exit(1);    }    fread(tempImage, sizeof(unsigned char), 128*128, file);    fclose(file);    for (int row=0; row<128; row++){      for (int col=0; col<128; col++){	texture1[row+2][col+2] = (int)tempImage[row][col];      }    }        // pad elements to edges    // rows    for (int col=0; col<128; col++)      texture1[0][col+2] = texture1[3][col+2];    for (int col=0; col<128; col++)      texture1[1][col+2] = texture1[2][col+2];    for (int col=0; col<128; col++)      texture1[130][col+2] = texture1[129][col+2];    for (int col=0; col<128; col++)      texture1[131][col+2] = texture1[128][col+2];        // cols    for (int row=0; row<128; row++)      texture1[row+2][0] = texture1[row+2][3];    for (int row=0; row<128; row++)      texture1[row+2][1] = texture1[row+2][2];    for (int row=0; row<128; row++)      texture1[row+2][130] = texture1[row+2][129];    for (int row=0; row<128; row++)      texture1[row+2][131] = texture1[row+2][128];    // corners    texture1[0][0] = texture1[3][3];    texture1[1][1] = texture1[2][2];    texture1[0][1] = texture1[2][3];    texture1[1][0] = texture1[3][2];        texture1[0][130] = texture1[2][128];    texture1[0][131] = texture1[3][128];    texture1[1][130] = texture1[2][129];    texture1[1][131] = texture1[3][129];        texture1[130][130] = texture1[129][129];    texture1[130][131] = texture1[128][129];    texture1[131][131] = texture1[128][128];    texture1[131][130] = texture1[129][128];        texture1[130][0] = texture1[128][2];    texture1[130][1] = texture1[129][2];    texture1[131][0] = texture1[128][3];    texture1[131][1] = texture1[129][3];        double* EnergyVec = getEnergyVec(texture1);    eVec25[imgNum-1] = EnergyVec;        cout << "Energy Vector " << imgNum << endl;    for (int i=0; i<25; i++){      cout << EnergyVec[i] << " ";    }    cout << endl;  }  /*  // normalize the energy vector  for (int col=0; col<25; col++){    // find max min    double xmin = INT_MAX;    double xmax = INT_MIN;    for (int row=0; row<12; row++){      if (eVec25[row][col]<xmin)	xmin = eVec25[row][col];      if (eVec25[row][col]>xmax)	xmax = eVec25[row][col];    }    double span = xmax-xmin;    // normalize now    for (int row=0; row<12; row++){      if (span==0){	eVec25[row][col] = 0;      }      else {	eVec25[row][col] = (eVec25[row][col]-xmin)/(xmax-xmin);      }    }  }  // print normalized vectors  cout << "Normalized vectors " << endl;  for (int row=0; row<12; row++){    for (int col=0; col<25; col++){      cout << eVec25[row][col] << " ";    }    cout << endl;  }  cout << endl;  */  // find top and least most discriminant features    // average the vectors for each image into one  double avgVec4x25[4][25] = {};  int imageSeq[12] = {0, 1, 2, 0, 2, 0, 3, 3, 1, 3, 2, 1};  for (int i=0; i<4; i++){    for(int j=0; j<25; j++){      avgVec4x25[i][j] = 0;    }  }  // fill with avg values for each image  for (int i=0; i<4; i++){    // sum    for (int iSnum=0; iSnum<12; iSnum++){      if (imageSeq[iSnum]==i){	for (int j=0; j<25; j++){	  avgVec4x25[i][j] += eVec25[iSnum][j];	}      }    }    // avg    for (int j=0; j<25; j++){      avgVec4x25[i][j] /= 3;    }  }        // using standard dev as a metric  char filtOrder[5] = {'L', 'E', 'S', 'W', 'R'};  double stdev[25] = {};  double dummyStdev[25] = {};  for (int col=0; col<25; col++){        // find mean    double mean = 0;    for (int row=0; row<4; row++){      mean += avgVec4x25[row][col];    }    mean = mean/4;    // std dev    double stdDev = 0;    for (int row=0; row<4; row++){      stdDev += pow(avgVec4x25[row][col]-mean, 2);    }    stdDev /= 4;    stdDev = sqrt(stdDev);    stdev[col] = stdDev;    dummyStdev[col] = stdDev;  }  sort(stdev, stdev+25);  cout << "Standard deviations "<< endl;  cout << "Sorted standard deviations " << endl;  for (int i=0; i<25; i++)    cout << stdev[i] << " ";  cout << endl;  cout << "Unsorted standard deviations " << endl;  for (int i=0; i<25; i++)    cout << dummyStdev[i] << " ";  cout << endl;    // find top   int pos1 = -1;  int pos2 = -1;  for (int i=0; i<25; i++){    if (stdev[24]==dummyStdev[i]){      pos1 = i;    }    if (stdev[0]==dummyStdev[i]){      pos2 = i;    }  }  cout << "Most discriminant power is with " << filtOrder[int(pos1/5)] << filtOrder[pos1%5]<< endl;  cout << "Least discriminant power is with " << filtOrder[int(pos2/5)] << filtOrder[pos2%5]<< endl;  // pca using opencv  double norm_eVec[12][25];  for (int row=0; row<12; row++){    for (int col=0; col<25; col++){      norm_eVec[row][col] = eVec25[row][col];    }  }  Mat cv_eVec25 = Mat(12, 25, CV_64F, norm_eVec);  PCA pca(cv_eVec25, Mat(), CV_PCA_DATA_AS_ROW, 3);    Mat proj_eVec3 = Mat::zeros(12, 3, CV_32F);    pca.project(cv_eVec25, proj_eVec3);  cout << "Projected vectors into 3D space " << endl;  cout << proj_eVec3 << endl;  // transfer the feature vectors to c++ matrix  double** eVec3 = new double*[12];  for (int row=0; row<12; row++){    eVec3[row] = new double[3];  }    for (int row=0; row<12; row++){    for (int col=0; col<3; col++){      eVec3[row][col] = proj_eVec3.at<double>(row,col);    }  }  /*  // normalize the vectors again after pca  for (int col=0; col<3; col++){    // find max min    double xmin = INT_MAX;    double xmax = INT_MIN;    for (int row=0; row<12; row++){      if (eVec3[row][col]<xmin)	xmin = eVec3[row][col];      if (eVec3[row][col]>xmax)	xmax = eVec3[row][col];    }    // normalize now    for (int row=0; row<12; row++){      eVec3[row][col] = (eVec3[row][col]-xmin)/(xmax-xmin);    }  }  for (int row=0; row<12; row++){    cout << eVec3[row][0] << " " << eVec3[row][1] << " " << eVec3[row][2] << endl;  }  */  // K-means  // 25-D vectors  int* class_25D = k_means(eVec25, 12, 25, 4, 0.0001, 0);  cout << "clustering using 25d vectors " << endl;  for (int i=0; i<12; i++){    cout << class_25D[i] << " ";  }  cout << endl;  // 3-D vectors    int* class_3D = k_means(eVec3, 12, 3, 4, 0.0001, 0);  cout << "clustering using 3d vectors " << endl;  for (int i=0; i<12; i++){    cout << class_3D[i] << " ";  }  cout << endl;  // clear memory    for (int i=0; i<128; i++){    delete [] texture1[i];  }  delete [] texture1;    return 0;}