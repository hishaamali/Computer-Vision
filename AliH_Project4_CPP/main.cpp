#include <iostream>
#include <fstream>
using namespace std;

int numImgRows, numImgCols, imgMin, imgMax, numStructRows, numStructCols, structMin, structMax, rowOrigin, colOrigin, rowFrameSize, colFrameSize, extraRows, extraCols;
int ** zeroFramedAry;
int ** morphAry;
int ** structAry;
int ** m2;
int ** m3;

void loadImg(ifstream & imgFile, int ** zeroFramedAry){
  for (int i = rowOrigin; i < numImgRows + rowOrigin; i++){
    for (int j = colOrigin; j < numImgCols + colOrigin; j++){
      imgFile >> zeroFramedAry[i][j];
    }
  }
}

void zero2DAry(int ** ary){
  for (int i = 0; i < numImgRows + extraRows; i++){
    for (int j = 0; j < numImgCols + extraCols; j++){
      ary[i][j] = 0;
    }
  }
}

void zeroStruct(int ** structAry){
  for (int i = 0; i < numStructRows; i++){
    for (int j = 0; j < numStructCols; j++){
      structAry[i][j] = 0;
    }
  }
}

void loadStruct(ifstream & structFile, int ** structAry){
  for (int i = 0; i < numStructRows; i++){
    for (int j = 0; j < numStructCols; j++){
      structFile >> structAry[i][j];
    }
  }
}


void dilation(int i , int j){
  int m = 0;
  int k = 0;
  for (int a = i; a < i+numStructRows; a++){
    for (int b = j; b < j+numStructCols; b++){
      if (structAry[m][k] == 1){
        morphAry[a-rowOrigin][b-colOrigin] = structAry[m][k];
        m3[a-rowOrigin][b-colOrigin] = structAry[m][k];
      }
      k++;
    }
    k = 0;
    m++;
  }
}

void erosion(int i , int j, int ** ary){
  int m = 0;
  int k = 0;
  int s = 0;
  for (int a = i; a < i+numStructRows; a++){
    for (int b = j; b < j+numStructCols; b++){
      if ((structAry[m][k] == 1) && (ary[a-rowOrigin][b-colOrigin] == 0)){
        morphAry[i][j] = 0;
        m2[i][j] = 0;
        s = 1;
        break;
      }
      k++;
    } 
    k = 0;
    m++;
  }
  if (s == 0){
    morphAry[i][j] = ary[i][j];
    m2[i][j] = ary[i][j];
  }
}

void ComputeErosion(int ** ary){
  for (int i = rowOrigin; i < numImgRows + rowOrigin; i++){
    for (int j = colOrigin; j < numImgCols + colOrigin; j++){
      if (ary[i][j] > 0) erosion(i,j, ary);
    }
  }
}

void ComputeDilation(int ** ary){
  for (int i = rowOrigin; i < numImgRows + rowOrigin; i++){
    for (int j = colOrigin; j < numImgCols + colOrigin; j++){
      if (ary[i][j] > 0) dilation(i,j);
    }
  }
}

void opening(int i , int j){
}

void closing(int i , int j){

}

void ComputeOpening(int ** ary){
  ComputeErosion(ary);
  ComputeDilation(m2);
}

void ComputeClosing(int ** ary){
  ComputeDilation(ary);
  ComputeErosion(m3);
}

void prettyPrint(int ** ary, ofstream & out){
  for (int i = rowOrigin; i < numImgRows + rowOrigin; i++){
    for (int j = colOrigin; j < numImgCols + colOrigin; j++){
      if (ary[i][j] == 0) out << "." << " ";
      else out << ary[i][j] << " ";
    }
    out << endl;
  }
  out << endl;
  out << endl;
}

void prettyPrintStruct(int ** ary, ofstream & out){
  for (int i = 0; i < numStructRows; i++){
    for (int j = 0; j < numStructCols; j++){
      if (ary[i][j] == 0) out << "." << " ";
      else out << ary[i][j] << " ";
    }
    out << endl;
  }
  out << endl;
  out << endl;
}

void AryToFile(int ** ary, ofstream & out){
  out << numImgRows << " " << numImgCols << " " << imgMin << " " << imgMax << " " << endl;
  for (int i = rowOrigin; i < numImgRows + rowOrigin; i++){
    for (int j = colOrigin; j < numImgCols + colOrigin; j++){
      out << ary[i][j] << " ";
    }
    out << endl;
  }
}

int main(int argc, char* argv[]) {
  ifstream imgFile;
  ifstream structFile;
  ofstream dilateOutFile;
  ofstream erodeOutFile;
  ofstream closingOutFile;
  ofstream openingOutFile;
  ofstream prettyPrintFile;
  imgFile.open(argv[1]);
  structFile.open(argv[2]);
  dilateOutFile.open(argv[3]);
  erodeOutFile.open(argv[4]);
  closingOutFile.open(argv[5]);
  openingOutFile.open(argv[6]);
  prettyPrintFile.open(argv[7]);
  imgFile >> numImgRows >> numImgCols >> imgMin >> imgMax;
  structFile >> numStructRows >> numStructCols >> structMin >> structMax >> rowOrigin >> colOrigin;
  rowFrameSize = numStructRows / 2;
  colFrameSize = numStructCols / 2;
  extraRows = rowFrameSize * 2;
  extraCols = colFrameSize * 2;
  zeroFramedAry = new int*[numImgRows + extraRows];
  morphAry = new int*[numImgRows + extraRows];
  structAry = new int*[numStructRows];
  m2 = new int*[numImgRows + extraRows];
  m3 = new int*[numImgRows + extraRows];
  for (int i = 0; i < numImgRows + extraRows; i++){
    zeroFramedAry[i] = new int[numImgCols + extraCols];
    morphAry[i] = new int[numImgCols + extraCols];
    m2[i] = new int[numImgCols + extraCols];
    m3[i] = new int[numImgCols + extraCols];
  }
  for (int i = 0; i < numStructRows; i++){
    structAry[i] = new int[numStructCols];
  }
  zero2DAry(zeroFramedAry);
  loadImg(imgFile, zeroFramedAry);
  prettyPrint(zeroFramedAry, prettyPrintFile);
  zeroStruct(structAry);
  loadStruct(structFile, structAry);
  cout << endl;
  prettyPrintStruct(structAry, prettyPrintFile);
  zero2DAry(morphAry);
  ComputeDilation(zeroFramedAry);
  AryToFile(morphAry, dilateOutFile);
  prettyPrint(morphAry, prettyPrintFile);
  zero2DAry(morphAry);
  ComputeErosion(zeroFramedAry);
  AryToFile(morphAry, erodeOutFile);
  prettyPrint(morphAry, prettyPrintFile);
  zero2DAry(morphAry);
  zero2DAry(m2);
  zero2DAry(m3);
  ComputeOpening(zeroFramedAry);
  AryToFile(morphAry, openingOutFile);
  prettyPrint(morphAry, prettyPrintFile);
  zero2DAry(morphAry);
  zero2DAry(m2);
  zero2DAry(m3);
  ComputeClosing(zeroFramedAry);
  AryToFile(morphAry, closingOutFile);
  prettyPrint(morphAry, prettyPrintFile);
  imgFile.close();
  structFile.close();
  dilateOutFile.close();
  erodeOutFile.close();
  openingOutFile.close();
  closingOutFile.close();
  prettyPrintFile.close();
  return 0;
}


