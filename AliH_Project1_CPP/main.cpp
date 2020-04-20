// Hishaam Ali Project 1 Computer Vision
#include <iostream>
#include <fstream>
#include <stdlib.h> 
using namespace std;

class image {

public:

	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	int* histAry;
	int thresholdValue;
	int pixel_val;
	void computeHist(ifstream &inFile, int* histAry);
	void printHist(int* histAry, ofstream &outFile1);
	void thresholdOp(ifstream &inFile, int thresholdValue, fstream &outFile2);
	void prettyPrint(fstream &outFile2, ofstream &outFile3);
	
};

void image::computeHist(ifstream &inFile, int* histAry){
	while(!inFile.eof()){
		inFile >> pixel_val;
		histAry[pixel_val]++;
	}
}

void image::printHist(int* histAry, ofstream &outFile1){
		for (int i = 0; i < maxVal + 1; i++){
			outFile1 << i << "(" << histAry[i] << ")" << ":";
			for (int j = 0; j < histAry[i]; j++){
				if(j < 60) outFile1 << "+";	
			}
			outFile1 << endl;
		}
}

void image::thresholdOp(ifstream &inFile, int thresholdValue, fstream &outFile2){
	int** pixelAry = new int*[numRows];
	for (int i = 0; i < numRows; i++){
		pixelAry[i] = new int[numCols];
	}
	minVal = 0;
	maxVal = 1;
	
	for (int iRow = 0; iRow < numRows; iRow++){
		for (int jCol = 0; jCol < numCols; jCol++){
			inFile >> pixel_val;
			if (pixel_val >= thresholdValue){
				pixelAry[iRow][jCol] = 1;
			}
			else {
				pixelAry[iRow][jCol] = 0;
			}
		outFile2 << pixelAry[iRow][jCol] << " ";
		}
	outFile2 << endl;
	}
}

void image::prettyPrint(fstream &outFile2, ofstream &outFile3){
	int** pixelAry = new int*[numRows];
	for (int i = 0; i < numRows; i++){
		pixelAry[i] = new int[numCols];
	}
	
	
	for (int iRow = 0; iRow < numRows; iRow++){
		for (int jCol = 0; jCol < numCols; jCol++){
			outFile2 >> pixel_val;
			if (pixel_val > 0){
				pixelAry[iRow][jCol] = pixel_val;
				outFile3 << pixel_val << " ";
			}
			else {
				pixelAry[iRow][jCol] = 0;
				outFile3 << " " << " ";
			}
		}
	outFile3 << endl;
	}
}



int main (int argc, char* argv[]){
	image img;
	ifstream inFile;
	ofstream outFile1;
	fstream outFile2;
	ofstream outFile3;
	inFile.open(argv[1]);
	outFile1.open(argv[2]);
	outFile2.open(argv[3],fstream::out);
	outFile3.open(argv[4]);
	img.thresholdValue = atoi(argv[5]);
	inFile >> img.numRows;
	inFile >> img.numCols;
	inFile >> img.minVal;
	inFile >> img.maxVal;
	outFile1 << img.numRows << " " << img.numCols << " " << img.minVal << " " << img.maxVal << endl;
	outFile2 << img.numRows << " " << img.numCols << " " << 0 << " " << 1 << endl;
	img.histAry = new int[img.maxVal + 1];
	for (int i = 0; i < img.maxVal + 1; i++){
		img.histAry[i] = 0;
	}
	img.computeHist(inFile, img.histAry);
	img.printHist(img.histAry, outFile1);
	inFile.close();
	inFile.open(argv[1]);
	inFile >> img.numRows;
	inFile >> img.numCols;
	inFile >> img.minVal;
	inFile >> img.maxVal;
	img.thresholdOp(inFile, img.thresholdValue, outFile2);
	outFile2.close();
	outFile2.open(argv[3]);
	outFile2 >> img.numRows;
	outFile2 >> img.numCols;
	outFile2 >> img.minVal;
	outFile2 >> img.maxVal;
	img.prettyPrint(outFile2, outFile3);
	inFile.close();
	outFile1.close();
	outFile2.close();
	outFile3.close();
	return 0;
}
	