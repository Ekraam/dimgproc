// Homework 3// This code implements SIFT and SURF detection// Question 2a// This code takes no arguments// The image paths are hardcoded inside the script. change them here if needed.// //// Name: Ekraam Sabir // USC ID: 1428-2892-90// email: esabir@usc.edu// #include <opencv/cv.h>#include <opencv2/core/core.hpp>#include <opencv2/highgui/highgui.hpp>//#include <opencv2/features2d/features2d.hpp>#include <opencv2/nonfree/features2d.hpp>#include <opencv2/imgproc/imgproc.hpp>#include <fstream>#include <stdio.h>#include <iostream>#include <stdlib.h>#include <sstream>#define _USE_MATH_DEFINES#include <math.h>#include <cmath>#include <vector>#include <string>#include <algorithm>#include <limits.h>using namespace std;using namespace cv;int main(int argc, char *argv[]){  FILE *file;  // dimensions  int jeepRow = 500;  int jeepCol= 380;  int busRow = 450;  int busCol = 240;    // Allocate image data array  unsigned char jeepImage[jeepRow][jeepCol][3];  unsigned char busImage[busRow][busCol][3];  ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////  // transfer Images to opencv  Mat jeepCVImg = imread("./HW3 Images/P2/jeep.jpg", CV_LOAD_IMAGE_UNCHANGED);  Mat busCVImg = imread("./HW3 Images/P2/bus.jpg", CV_LOAD_IMAGE_UNCHANGED);  // convert to grayscale  Mat jeepCVImg_gray;  Mat busCVImg_gray;  cvtColor(jeepCVImg, jeepCVImg_gray, CV_BGR2GRAY);  cvtColor(busCVImg, busCVImg_gray, CV_BGR2GRAY);  // get SIFT features  SiftFeatureDetector siftDetector;  vector<KeyPoint> jeepKeypoints, busKeypoints;  // detect  siftDetector.detect(jeepCVImg_gray, jeepKeypoints);  siftDetector.detect(busCVImg_gray, busKeypoints);  // draw  drawKeypoints(jeepCVImg_gray, jeepKeypoints, jeepCVImg);    drawKeypoints(busCVImg_gray, busKeypoints, busCVImg);    // write resulting keypoints to image  // jeep  if (jeepCVImg.empty()){    cout << "empty image found " << endl;  }  namedWindow("jeep SIFT features", CV_WINDOW_AUTOSIZE);   imshow("jeep SIFT features", jeepCVImg);  waitKey(0);  // bus  if (busCVImg.empty()){    cout << "empty image found " << endl;  }  namedWindow("bus SIFT features", CV_WINDOW_AUTOSIZE);   imshow("bus SIFT features", busCVImg);  waitKey(0);  // get SURF features  int threshold = 3000;  SurfFeatureDetector surfDetector(threshold);  vector<KeyPoint> jeepSurfKeypoints, busSurfKeypoints;  // detect  surfDetector.detect(jeepCVImg_gray, jeepSurfKeypoints);  surfDetector.detect(busCVImg_gray, busSurfKeypoints);  // draw  drawKeypoints(jeepCVImg_gray, jeepSurfKeypoints, jeepCVImg);    drawKeypoints(busCVImg_gray, busSurfKeypoints, busCVImg);    // write resulting keypoints to image  // jeep  if (jeepCVImg.empty()){    cout << "empty image found " << endl;  }  namedWindow("jeep SURF features", CV_WINDOW_AUTOSIZE);   imshow("jeep SURF features", jeepCVImg);  waitKey(0);  // bus  if (busCVImg.empty()){    cout << "empty image found " << endl;  }  namedWindow("bus SURF features", CV_WINDOW_AUTOSIZE);   imshow("bus SURF features", busCVImg);  waitKey(0);    return 0;}