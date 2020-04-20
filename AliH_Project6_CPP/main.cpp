#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

class CCLabel {
public:
	int numRows, numCols, minVal, maxVal, newMin, newMax, newLabel, trueNumCC, numNb;
	int** zeroFramedAry;
	int* NonZeroNeighborAry;
	int* EQAry;
	struct Property {
		int label, numpixels, upperLftR, upperLftC, lowerRgtR, lowerRgtC;
	};
	Property* CCproperty;

	void loadImage(int** ary, ifstream& inFile) {
		for (int i = 1; i < numRows + 1; i++) {
			for (int j = 1; j < numCols + 1; j++) {
				inFile >> ary[i][j];
			}
		}
	}

	void zero2D(int** ary) {
		for (int i = 0; i < numRows + 2; i++) {
			for (int j = 0; j < numCols + 2; j++) {
				ary[i][j] = 0;
			}
		}
	}

	void minus1D(int* ary) {
		for (int i = 0; i < numNb; i++) {
			ary[i] = -1;
		}
	}

	int connectPass1(int Connectness, int** Ary, int* nonZeroAry) {
		newLabel = 0;
		int minLabel;
		bool diffFlag;
		int nonZeroCount;
		for (int i = 1; i < numRows + 1; i++) {
			for (int j = 1; j < numCols + 1; j++) {
				if (Ary[i][j] > 0) {
					minLabel = loadNonZeroPass1(Ary, Connectness, i, j, nonZeroAry, diffFlag, nonZeroCount);
					if (minLabel == 0) {
						newLabel++;
						Ary[i][j] = newLabel;
					}
					else if (minLabel > 0) {
						Ary[i][j] = minLabel;
						if (diffFlag == true) {
							updateEQ(EQAry, NonZeroNeighborAry, minLabel, nonZeroCount);
						}
					}
				}
			}
		}
		return newLabel;
	}

	void connectPass2(int Connectness, int** Ary, int* nonZeroAry) {
		int minLabel = 0;
		bool diffFlag;
		int nonZeroCount;
		for (int i = numRows; i > 0; i--) {
			for (int j = numCols; j > 0; j--) {
				if (Ary[i][j] > 0) {
					minLabel = loadNonZeroPass2(Ary, Connectness, i, j, nonZeroAry, diffFlag, nonZeroCount);
					if (minLabel != Ary[i][j]) {
						Ary[i][j] = minLabel;
					}
					if (diffFlag == true) {
						updateEQ(EQAry, nonZeroAry, minLabel, nonZeroCount);
					}
				}
			}
		}
	}

	void connectPass3(int** ary) {
		newMax = 0;
		newMin = EQAry[0];
		for (int i = 1; i < numRows + 1; i++) {
			for (int j = 1; j < numCols + 1; j++) {
				if (ary[i][j] > 0) {
					ary[i][j] = EQAry[ary[i][j]];
					CCproperty[ary[i][j]].numpixels++;
					if (CCproperty[ary[i][j]].upperLftR == 0) CCproperty[ary[i][j]].upperLftR = i;
					if (CCproperty[ary[i][j]].lowerRgtR < i) CCproperty[ary[i][j]].lowerRgtR = i;
					if (CCproperty[ary[i][j]].upperLftC == 0) CCproperty[ary[i][j]].upperLftC = j;
					if (CCproperty[ary[i][j]].lowerRgtC < j) CCproperty[ary[i][j]].lowerRgtC = j;
					if (newMax < ary[i][j]) {
						newMax = ary[i][j];
					}
				}
			}
		}
	}

	int loadNonZeroPass1(int** Ary, int Connectness, int i, int j, int* nonZeroAry, bool& diffFlag, int& nonZeroCount) {
		minus1D(nonZeroAry);
		nonZeroCount = 0;
		if (Ary[i - 1][j] > 0) {
			nonZeroAry[nonZeroCount] = Ary[i - 1][j];
			nonZeroCount++;
		}
		if (Ary[i][j - 1] > 0) {
			nonZeroAry[nonZeroCount] = Ary[i][j - 1];
			nonZeroCount++;
		}
		if (Connectness == 8) {
			if (Ary[i - 1][j - 1] > 0) {
				nonZeroAry[nonZeroCount] = Ary[i - 1][j - 1];
				nonZeroCount++;
			}
			if (Ary[i - 1][j + 1] > 0) {
				nonZeroAry[nonZeroCount] = Ary[i - 1][j + 1];
				nonZeroCount++;
			}
		}
		if (nonZeroCount <= 0) {
			return 0;
		}
		int minLabel = nonZeroAry[0];
		diffFlag = false;
		int index = 1;
		while (index < nonZeroCount) {
			if (minLabel != nonZeroAry[index]) {
				diffFlag = true;
			}
			if (minLabel > nonZeroAry[index]) {
				minLabel = nonZeroAry[index];
			}
			index++;
		}
		return minLabel;
	}

	int loadNonZeroPass2(int** Ary, int Connectness, int i, int j, int* nonZeroAry, bool& diffFlag, int& nonZeroCount) {
		minus1D(nonZeroAry);
		nonZeroCount = 0;
		nonZeroAry[nonZeroCount] = Ary[i][j];
		nonZeroCount++;
		if (Ary[i + 1][j] > 0) {
			nonZeroAry[nonZeroCount] = Ary[i + 1][j];
			nonZeroCount++;
		}
		if (Ary[i][j + 1] > 0) {
			nonZeroAry[nonZeroCount] = Ary[i][j + 1];
			nonZeroCount++;
		}
		if (Connectness == 8) {
			if (Ary[i + 1][j - 1] > 0) {
				nonZeroAry[nonZeroCount] = Ary[i + 1][j - 1];
				nonZeroCount++;
			}
			if (Ary[i + 1][j + 1] > 0) {
				nonZeroAry[nonZeroCount] = Ary[i + 1][j + 1];
				nonZeroCount++;
			}
		}
		int minLabel = nonZeroAry[0];
		diffFlag = false;
		int index = 1;
		while (index < nonZeroCount) {
			if (minLabel != nonZeroAry[index]) {
				diffFlag = true;
			}
			if (minLabel > nonZeroAry[index]) {
				minLabel = nonZeroAry[index];
			}
			index++;
		}
		return minLabel;
	}

	void updateEQ(int* EQAry, int* nonZeroAry, int minLabel, int nonZeroCount) {
		int index = 0;
		while (index < nonZeroCount && nonZeroAry[index] != -1) {
			EQAry[nonZeroAry[index]] = minLabel;
			index++;
		}
	}

	void prettyPrint(int** ary, ofstream& outFile, string whichPretty) {
		if (whichPretty == "Pass 1") outFile << "Pass 1 Output:" << endl;
		if (whichPretty == "Pass 2") outFile << "Pass 2 Output:" << endl;
		if (whichPretty == "Pass 3") outFile << "Pass 3 Output:" << endl;;
		if (whichPretty == "Boxed") outFile << "Boxed Output:" << endl;
		for (int i = 0; i < numRows + 2; i++) {
			for (int j = 0; j < numCols + 2; j++) {
				if (ary[i][j] == 0) outFile << " " << " ";
				else outFile << zeroFramedAry[i][j] << " ";
			}
			outFile << endl;
		}
	}

	void drawBoxes(int** ary, Property* CCproperty) {

		int	index = 1;
			while (index <= trueNumCC) {
				int minRow = CCproperty[index].upperLftR;
				int maxRow = CCproperty[index].lowerRgtR;
				int minCol = CCproperty[index].upperLftC;
				int maxCol = CCproperty[index].lowerRgtC;
				for (int i = minRow; i < maxRow + 1; i++) {
					ary[i][minCol] = index;
					ary[i][maxCol] = index;
				}
				for (int j = minCol; j < maxCol + 1; j++) {
					ary[minRow][j] = index;
					ary[maxRow][j] = index;
				}
				index++;
			}
	}
	void printEQAry(int newLabel, int* ary, ofstream& out, string whichPrint) {
		if (whichPrint == "Pass 1") out << "Pass 1 EQ Table" << endl;
		else if (whichPrint == "Pass 2") out << "Pass 2 EQ Table" << endl;
		else if (whichPrint == "Pass 3") out << "Pass 3 EQ Table" << endl;
		else if (whichPrint == "Manage") out << "Managed EQ Table" << endl;
		for (int i = 0; i <= newLabel; i++) {
			out << ary[i] << " ";
		}
		out << endl << endl;
	}


	int manageEQAry(int* ary, int newLabel) {
		int realLabel = 0;
		int index = 1;
		while (index <= newLabel) {
			if (index != ary[index]) {
				ary[index] = ary[ary[index]];
			}
			else {
				realLabel++;
				ary[index] = realLabel;
			}
			index++;
		}
		return realLabel;
	}

	void printImg(ofstream& out) {
		out << endl;
		for (int i = 1; i < numRows + 1; i++) {
			for (int j = 1; j < numCols + 1; j++) {
				out << zeroFramedAry[i][j] << " ";
			}
			out << endl;
		}
	}

	void printCCproperty(ofstream& out) {
		out << numRows << " " << numCols << " " << newMin << " " << newMax << endl;
		out << trueNumCC << endl;
		int index = 1;
		while (index < trueNumCC + 1) {
			out << index << endl;
			out << CCproperty[index].numpixels << endl;
			out << CCproperty[index].upperLftR << " " << CCproperty[index].upperLftC << endl;
			out << CCproperty[index].lowerRgtR << " " << CCproperty[index].lowerRgtC << endl;
			out << endl;
			index++;
		}
	}

};

int main(int argc, char* argv[]) {
	CCLabel cc;
	ifstream inFile;
	ofstream prettyPrintFile;
	ofstream labelFile;
	ofstream propertyFile;
	int Connectness;
	inFile.open(argv[1]);
	Connectness = atoi(argv[2]);
	prettyPrintFile.open(argv[3]);
	labelFile.open(argv[4]);
	propertyFile.open(argv[5]);
	inFile >> cc.numRows >> cc.numCols >> cc.minVal >> cc.maxVal;
	cc.zeroFramedAry = new int* [cc.numRows + 2];
	for (int i = 0; i < cc.numRows + 2; i++) {
		cc.zeroFramedAry[i] = new int[cc.numCols + 2];
	}
	cc.numNb = 5;
	cc.NonZeroNeighborAry = new int[cc.numNb];

	cc.EQAry = new int[(cc.numRows * cc.numCols) / 2];
	for (int i = 0; i < cc.numNb; i++) {
		cc.NonZeroNeighborAry[i] = 0;
	}
	for (int i = 0; i < (cc.numRows * cc.numCols) / 2; i++) {
		cc.EQAry[i] = i;
	}
	cc.zero2D(cc.zeroFramedAry);
	cc.loadImage(cc.zeroFramedAry, inFile);
	cc.newLabel = cc.connectPass1(Connectness, cc.zeroFramedAry, cc.NonZeroNeighborAry);
	cc.prettyPrint(cc.zeroFramedAry, prettyPrintFile, "Pass 1");
	cc.printEQAry(cc.newLabel, cc.EQAry, prettyPrintFile, "Pass 1");
	cc.connectPass2(Connectness, cc.zeroFramedAry, cc.NonZeroNeighborAry);
	cc.prettyPrint(cc.zeroFramedAry, prettyPrintFile, "Pass 2");
	cc.printEQAry(cc.newLabel, cc.EQAry, prettyPrintFile, "Pass 2");
	cc.trueNumCC = cc.manageEQAry(cc.EQAry, cc.newLabel);
	cc.CCproperty = new CCLabel::Property[cc.trueNumCC + 1];
	for (int i = 0; i < cc.trueNumCC + 1; i++) {
		cc.CCproperty[i].label = 0;
		cc.CCproperty[i].lowerRgtR = 0;
		cc.CCproperty[i].lowerRgtC = 0;
		cc.CCproperty[i].numpixels = 0;
		cc.CCproperty[i].upperLftR = 0;
		cc.CCproperty[i].upperLftC = 0;
	}
	cc.printEQAry(cc.newLabel, cc.EQAry, prettyPrintFile, "Manage");
	cc.connectPass3(cc.zeroFramedAry);
	cc.prettyPrint(cc.zeroFramedAry, prettyPrintFile, "Pass 3");
	cc.printEQAry(cc.newLabel, cc.EQAry, prettyPrintFile, "Pass 3");
	labelFile << cc.numRows << " " << cc.numCols << " " << cc.newMin << " " << cc.newMax;
	cc.printImg(labelFile);
	cc.printCCproperty(propertyFile);
	cc.drawBoxes(cc.zeroFramedAry, cc.CCproperty);
	cc.prettyPrint(cc.zeroFramedAry, prettyPrintFile, "Boxed");
	inFile.close();
	prettyPrintFile.close();
	propertyFile.close();
	labelFile.close();
}