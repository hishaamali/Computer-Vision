#include <iostream>
#include <fstream>
using namespace std;

class imageProcessing{
  public:

  int numRows, numCols, minVal, maxVal, newMin, newMax, maskRows, maskCols, maskMin, maskMax;
  int** mirrorFramedAry;
  int** avgAry;
  int** medianAry;
  int** GaussAry;
  int** maskAry;
  int val;
  int m;
  
  void loadMask(int** maskAry, ifstream & maskFile){
    for (int i = 0; i < maskRows; i++){
      for(int j = 0; j < maskCols; j++){
        maskFile >> m;
        maskAry[i][j] = m;
      }
    }
  }

  void loadImage(int** mirrorFramedAry, ifstream & inFile){
    for (int i = 1; i < numRows + 1; i++){
      for (int j = 1; j < numCols + 1 ; j++){
        inFile >> val;
        mirrorFramedAry[i][j] = val;
      }
    }
  }

  void mirrorFraming(int ** mirrorFramedAry){
    for (int i = 0; i < numRows + 2; i++){
      mirrorFramedAry[i][0] = mirrorFramedAry[i][1];
      mirrorFramedAry[i][numCols+1] = mirrorFramedAry[i][numCols];
      }
    for (int j = 0; j < numCols + 2; j++){
      mirrorFramedAry[0][j] = mirrorFramedAry[1][j];
      mirrorFramedAry[numRows + 1][j] = mirrorFramedAry[numRows][j];
    }
  }
  
  void ComputeAvgImg(int ** avgAry, int & newMin, int & newMax){
    newMin = 9999;
    newMax = 0;
    for (int i = 1; i < numRows + 1; i++){
      for (int j = 1; j < numCols + 1; j++){
        avgAry[i][j] = avg3x3(i, j, avgAry);
        if (newMin > avgAry[i][j]){
          newMin = avgAry[i][j];
        }
        if (newMax < avgAry[i][j]){
          newMax = avgAry[i][j];
        }
      }
    }
  }

  void outputAryToFile(int ** ary, ofstream & outFile, int & newMin, int & newMax){
    outFile << numRows << " " << numCols << " " << newMin << " " << newMax << endl;
    for (int i = 1; i < numRows + 1 ; i++){
      for (int j = 1; j < numCols + 1; j++){
        outFile << ary[i][j] << " ";
      }
      outFile << endl;
    }
  }

  void computeMedianImg(int ** medianAry, int & newMin, int & newMax){
    newMin = 9999;
    newMax = 0;
    int i = 1;
    int j = 1;
    int neighborAry[9];
    int n = sizeof(neighborAry)/sizeof(neighborAry[0]);
    for (int i = 1; i < numRows + 1; i++){
      for (int j = 1; j < numCols + 1; j++){
        loadNeighbors(i, j, neighborAry);
        sort(neighborAry, n);
        medianAry[i][j] = neighborAry[4];
        if (newMin > medianAry[i][j]){
         newMin = medianAry[i][j];
        }
        if (newMax < medianAry[i][j]){
          newMax = medianAry[i][j];
        }
      }
    }
  }

  void computeGaussImg(int ** GaussAry, int & newMin, int & newMax){
    newMin = 9999;
    newMax = 0;
    for (int i = 1; i < numRows + 1; i++){
      for (int j = 1; j < numCols + 1; j++){
        GaussAry[i][j] = convolution(i, j, GaussAry, maskAry);
        if (newMin > GaussAry[i][j]){
          newMin = GaussAry[i][j];
        }
        if (newMax < GaussAry[i][j]){
          newMax = GaussAry[i][j];
        }
      }
    }
  }

  void loadNeighbors(int i , int j, int neighborAry[]){
    for (int k = i; k < i + 1; k++){
      for (int l = j; l < j + 1; l++){
        neighborAry[0] = mirrorFramedAry[k-1][l-1];
        neighborAry[1] = mirrorFramedAry[k-1][l];
        neighborAry[2] = mirrorFramedAry[k-1][l+1];
        neighborAry[3] = mirrorFramedAry[k][l-1];
        neighborAry[4] = mirrorFramedAry[k][l];
        neighborAry[5] = mirrorFramedAry[k-1][l+1];
        neighborAry[6] = mirrorFramedAry[k+1][l-1];
        neighborAry[7] = mirrorFramedAry[k+1][l];
        neighborAry[8] = mirrorFramedAry[k+1][l+1];
     }
   }
  }

  int avg3x3(int i, int j, int ** avgAry){ 
    int sum = 0;
    for (int k = i - 1; k < i + 2; k++){
      for (int l = j - 1; l < j + 2; l++){
        sum += mirrorFramedAry[k][l];
      }
    }
    return sum/9;
  }

  int convolution(int i, int j, int ** GaussAry, int ** maskAry){
    int sum = 0;
    int total = 0;
    total = maskAry[0][0] + maskAry[0][1] + maskAry[0][2] + maskAry[1][0] + maskAry[1][1] + maskAry[1][2] + maskAry[2][0] + maskAry[2][1] + maskAry[2][2];
    sum = (mirrorFramedAry[i-1][j-1]*maskAry[0][0])+(mirrorFramedAry[i-1][j] * maskAry[0][1])+(mirrorFramedAry   [i-1][j+1] * maskAry[0][2])+(mirrorFramedAry[i][j-1] * maskAry[1][0])+(mirrorFramedAry[i][j] * maskAry[1][1])+(mirrorFramedAry[i][j+1] * maskAry[1][2])+(mirrorFramedAry[i+1][j-1] * maskAry[2][0])+(mirrorFramedAry[i+1][j] * maskAry[2][1])+(mirrorFramedAry[i+1][j+1] * maskAry[2][2]);
    return sum/total;
  }

  void sort(int * ary, int n){
   int i, j, min, temp;
   for (i = 0; i < n - 1; i++) {
      min = i;
      for (j = i + 1; j < n; j++)
         if (ary[j] < ary[min])
            min = j;
         temp = ary[i];
         ary[i] = ary[min];
         ary[min] = temp;
   } 
  }

};

int main(int argc, char* argv[]) {
  imageProcessing ip;
  ifstream inFile;
  ifstream maskFile;
  ofstream AvgOutImg;
  ofstream MedianOutImg;
  ofstream GaussOutImg;
  inFile.open(argv[1]);
  maskFile.open(argv[2]);
  AvgOutImg.open(argv[3]);
  MedianOutImg.open(argv[4]);
  GaussOutImg.open(argv[5]);
  inFile >> ip.numRows >> ip.numCols >>ip. minVal >> ip.maxVal;
  maskFile >> ip.maskRows >> ip.maskCols >> ip.maskMin >> ip.maskMax;
  ip.mirrorFramedAry = new int*[ip.numRows + 2];
  ip.avgAry = new int*[ip.numRows + 2];
  ip.medianAry = new int*[ip.numRows + 2];
  ip.GaussAry = new int*[ip.numRows + 2];
  ip.maskAry = new int*[ip.maskRows];
  for (int i = 0; i < ip.numRows + 2; i++){
    ip.mirrorFramedAry[i] = new int[ip.numCols + 2];
    ip.avgAry[i] = new int[ip.numCols + 2];
    ip.medianAry[i] = new int[ip.numCols + 2];
    ip.GaussAry[i] = new int[ip.numCols + 2];
  }
  for (int i = 0; i < ip.maskRows; i++){
    ip.maskAry[i] = new int[ip.maskCols];
  }
  ip.loadMask(ip.maskAry, maskFile);
  ip.loadImage(ip.mirrorFramedAry, inFile);
  ip.mirrorFraming(ip.mirrorFramedAry);
  ip.ComputeAvgImg(ip.avgAry, ip.newMin, ip.newMax);
  ip.outputAryToFile(ip.avgAry, AvgOutImg, ip.newMin, ip.newMax);
  ip.computeMedianImg(ip.medianAry, ip.newMin, ip.newMax);
  ip.outputAryToFile(ip.medianAry, MedianOutImg, ip.newMin, ip.newMax);
  ip.computeGaussImg(ip.GaussAry, ip.newMin, ip.newMax);
  ip.outputAryToFile(ip.GaussAry, GaussOutImg, ip.newMin, ip.newMax);
  inFile.close();
  maskFile.close();
  AvgOutImg.close();
  MedianOutImg.close();
  GaussOutImg.close();
  return 0;
}