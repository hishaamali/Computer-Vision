#include <iostream>
#include <fstream>
using namespace std;

class ThinningSkeleton {
public:
	int numRows, numCols, minVal, maxVal, changeFlag, cycleCount;
	int** firstAry;
	int** secondAry;

	void loadImage(ifstream& inFile, int** ary) {
		for (int i = 1; i < numRows + 1; i++) {
			for (int j = 1; j < numCols + 1; j++) {
				inFile >> ary[i][j];
			}
		}
	}

	void zeroFrame(int** ary) {
		for (int i = 0; i < numRows + 2; i++) {
			for (int j = 0; j < numCols + 2; j++) {
				ary[i][j] = 0;
			}
		}
	}

	void prettyPrint(ofstream& o, int** ary) {
		for (int i = 0; i < numRows + 2; i++) {
			for (int j = 0; j < numCols + 2; j++) {
				if (ary[i][j] > 0) {
					o << ary[i][j] << " ";
				}
				else o << " " << " ";
			}
			o << endl;
		}
	}

	void doThinning(int** firstAry, int** secondAry, int& changeFlag) {
		northThinning(firstAry, secondAry, changeFlag);
		copyArys(firstAry, secondAry);
		southThinning(firstAry, secondAry, changeFlag);
		copyArys(firstAry, secondAry);
		westThinning(firstAry, secondAry, changeFlag);
		copyArys(firstAry, secondAry);
		eastThinning(firstAry, secondAry, changeFlag);
		copyArys(firstAry, secondAry);
	}

	void copyArys(int** firstAry, int** secondAry) {
		for (int i = 1; i < numRows + 1; i++) {
			for (int j = 1; j < numCols + 1; j++) {
				firstAry[i][j] = secondAry[i][j];
			}
		}
	}

	void northThinning(int** firstAry, int** secondAry, int& changeFlag) {
		for (int i = 1; i < numRows + 1; i++) {
			for (int j = 1; j < numCols + 1; j++) {
				secondAry[i][j] = firstAry[i][j];
				if (firstAry[i][j] > 0 && firstAry[i - 1][j] <= 0) {
					if (check3n4Conditions(firstAry, i, j, "North") == true) {
						secondAry[i][j] = 0;
						changeFlag++;
					}
				}
			}
		}
	}

	void southThinning(int** firstAry, int** secondAry, int& changeFlag) {
		for (int i = 1; i < numRows + 1; i++) {
			for (int j = 1; j < numCols + 1; j++) {
				secondAry[i][j] = firstAry[i][j];
				if (firstAry[i][j] > 0 && firstAry[i + 1][j] <= 0) {
					if (check3n4Conditions(firstAry, i, j, "South") == true) {
						secondAry[i][j] = 0;
						changeFlag++;
					}
				}
			}
		}
	}

	void westThinning(int** firstAry, int** secondAry, int& changeFlag) {
		for (int i = 1; i < numRows + 1; i++) {
			for (int j = 1; j < numCols + 1; j++) {
				secondAry[i][j] = firstAry[i][j];
				if (firstAry[i][j] > 0 && firstAry[i][j - 1] <= 0) {
					if (check3n4Conditions(firstAry, i, j, "West") == true) {
						secondAry[i][j] = 0;
						changeFlag++;
					}
				}
			}
		}
	}

	void eastThinning(int** firstAry, int** secondAry, int& changeFlag) {
		for (int i = 1; i < numRows + 1; i++) {
			for (int j = 1; j < numCols + 1; j++) {
				secondAry[i][j] = firstAry[i][j];
				if (firstAry[i][j] > 0 && firstAry[i][j + 1] <= 0) {
					if (check3n4Conditions(firstAry, i, j, "East") == true) {
						secondAry[i][j] = 0;
						changeFlag++;
					}
				}
			}
		}
	}

	bool check3n4Conditions(int** ary, int i, int j, string direction) {
		int count = -1;
		for (int k = i - 1; k < i + 2; k++) {
			for (int l = j - 1; l < j + 2; l++) {
				if (ary[k][l] == 1) count++;
			}
		}

		if (count >= 4 && direction == "North" && firstAry[i+1][j] == 1) {
      if((firstAry[i-1][j-1] + firstAry[i-1][j+1] + firstAry[i][j-1] + firstAry[i][j+1] + firstAry[i+1][j-1] + firstAry[i+1][j] + firstAry[i+1][j+1]) == 4 && ((firstAry[i][j-1] == 0 && firstAry[i+1][j-1] == 0 && firstAry[i-1][j+1] == 0) || (firstAry[i-1][j-1] == 0 && firstAry[i][j+1] == 0 && firstAry[i+1][j+1] == 0))) {
        return false;
      }
      else return true;
		}

		if (count >= 4 && direction == "South" && firstAry[i-1][j] == 1) {
      if((firstAry[i-1][j-1] + firstAry[i-1][j+1] + firstAry[i][j-1] + firstAry[i][j+1] + firstAry[i+1][j-1] + firstAry[i+1][j] + firstAry[i+1][j+1]) == 4 && ((firstAry[i][j-1] == 0 && firstAry[i+1][j-1] == 0 && firstAry[i-1][j+1] == 0) || (firstAry[i-1][j-1] == 0 && firstAry[i][j+1] == 0 && firstAry[i+1][j+1] == 0))) {
        return false;
      }
      else return true;
		}
		
		if (count >= 3 && direction == "East" && firstAry[i][j-1] == 1) {
      if((firstAry[i-1][j-1] + firstAry[i-1][j+1] + firstAry[i][j-1] + firstAry[i][j+1] + firstAry[i+1][j-1] + firstAry[i+1][j] + firstAry[i+1][j+1]) == 4 && ((firstAry[i][j-1] == 0 && firstAry[i+1][j-1] == 0 && firstAry[i-1][j+1] == 0) || (firstAry[i-1][j-1] == 0 && firstAry[i][j+1] == 0 && firstAry[i+1][j+1] == 0))) {
        return false;
      }
      else return true;
		}
	
		if (count >= 3 && direction == "West" && firstAry[i][j+1] == 1) {
      if((firstAry[i-1][j-1] + firstAry[i-1][j+1] + firstAry[i][j-1] + firstAry[i][j+1] + firstAry[i+1][j-1] + firstAry[i+1][j] + firstAry[i+1][j+1]) == 4 && ((firstAry[i][j-1] == 0 && firstAry[i+1][j-1] == 0 && firstAry[i-1][j+1] == 0) || (firstAry[i-1][j-1] == 0 && firstAry[i][j+1] == 0 && firstAry[i+1][j+1] == 0))) {
        return false;
      }
      else return true;
		}
		
	}

};



int main(int argc, char** argv) {
	ifstream inFile;
	ofstream outFile1;
	ofstream outFile2;
	ThinningSkeleton ts;
	inFile.open(argv[1]);
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);
	inFile >> ts.numRows >> ts.numCols >> ts.minVal >> ts.maxVal;
	outFile1 << ts.numRows << " " << ts.numCols << " " << ts.minVal << " " << ts.maxVal << endl;
	ts.firstAry = new int* [ts.numRows + 2];
	ts.secondAry = new int* [ts.numRows + 2];
	for (int i = 0; i < ts.numRows + 2; i++) {
		ts.firstAry[i] = new int[ts.numCols + 2];
		ts.secondAry[i] = new int[ts.numCols + 2];
	}
	ts.zeroFrame(ts.firstAry);
	ts.zeroFrame(ts.secondAry);
	ts.loadImage(inFile, ts.firstAry);
	outFile2 << "Print before thinning :" << endl;
	ts.prettyPrint(outFile2, ts.firstAry);
	ts.cycleCount = 1;
	while (ts.changeFlag > 0) {
		ts.changeFlag = 0;
		ts.doThinning(ts.firstAry, ts.secondAry, ts.changeFlag);
		outFile2 << "Result of thinning: cycle " << ts.cycleCount << ":" << endl;
		ts.prettyPrint(outFile2, ts.firstAry);
    ts.cycleCount++;
	}
  for (int i = 1; i < ts.numRows + 1; i++){
    for (int j = 1; j < ts.numCols + 1; j++){
      outFile1 << ts.firstAry[i][j] << " ";
    }
    outFile1 << endl;
  }

	


	
}