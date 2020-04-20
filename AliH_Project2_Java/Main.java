import java.io.*;
import java.util.*;
import java.lang.Math;

public class Main {
	
	static int numRows, numCols, minVal, maxVal , offSet, thrVal, maxHight;
	static int histAry [];
	static int GaussAry[];
	static int GaussGraph[][];
	static int histGraph[][];
	static int gapGraph[][];
	
	public static void main(String[] args) throws IOException {
		Scanner inFile = new Scanner (new File(args[0]));
		FileWriter outFile1 = new FileWriter(args[1]);
		FileWriter outFile2 = new FileWriter(args[2]);
		FileWriter outFile3 = new FileWriter(args[3]);
		numRows = inFile.nextInt();
		numCols = inFile.nextInt();
		minVal = inFile.nextInt();
		maxVal = inFile.nextInt();
		offSet = (maxVal - minVal) / 10;
		thrVal = offSet;
		histAry = new int[maxVal+1];
		loadHist(inFile, histAry);
		maxHight = getMax(histAry);
		GaussAry = new int [maxVal+1];
		GaussGraph = new int [maxVal+1][maxHight+1];
		histGraph = new int [maxVal+1][maxHight+1];
		gapGraph = new int [maxVal+1][maxHight+1];
		plotHistGraph(histGraph);
		outFile1.write("2-D display of the histogram: " + "\n" + "\n");
		prettyPrint(histGraph, outFile1);
		int bestThrVal = biMeanGauss(thrVal, outFile2, outFile3);
		outFile1.write("\n" + "The best threshold value is : " + bestThrVal + "\n" + "\n");
		bestThrPlot(bestThrVal);
		outFile1.write("2-D display of the two best fitting curves on top of histogram : " + "\n" + "\n");
		prettyPrint(GaussGraph, outFile1);
		outFile1.write("\n" + "2-D display of Gaps between the best fitting curves : " + "\n" + "\n");
		prettyPrint(gapGraph, outFile1);
		inFile.close();
		outFile1.close();
		outFile2.close();
		outFile3.close();
		
	}
	
	public static void loadHist(Scanner inFile, int [] histAry) throws IOException {
		for (int i = 0; i < histAry.length; i++ ) {
			inFile.nextInt();
			histAry[i] = inFile.nextInt();
		}
	
	}
	
	public static void plotHistGraph(int histGraph [][]) {
		for (int i = 0; i < maxVal+1; i++) {
			histGraph[i][histAry[i]] = 1;
		}
	}
	
	public static double computeMean(int leftIndex, int rightIndex, int maxHeight) {
		maxHeight = 0;
		int sum = 0;
		int numPixels = 0;
		int index = leftIndex;
		while(index < rightIndex) {
			sum += (histAry[index] * index);
			numPixels += histAry[index];
			if (histAry[index] > maxHeight) {
				maxHeight = histAry[index];
			}
			index++;
		}
		return (double)sum / (double)numPixels;
	}
	
	public static double computeVar(int leftIndex, int rightIndex, double mean) {
		int sum = 0;
		int numPixels = 0;
		int index = leftIndex;
		while (index < rightIndex) {
			sum += (double) histAry[index] * ((double) index-mean)*((double) index-mean);
			numPixels += histAry[index];
			index++;
		}
		return (double) sum / (double) numPixels;
	}
	
	public static double modifiedGauss(int x, double mean, double var, int maxHeight) {
		return (double) (maxHeight * Math.exp(-(((x-mean)*(x-mean)) / (2*var))));
		
	}
	
	public static void set1DZero(int [] array) {
		for (int i = 0; i < array.length; i++) {
			array[i] = 0;
		}
	}
	
	public static void set2DZero(int array[][]) {
		for (int i = 0; i < maxVal+1; i++) {
			for (int j = 0; j < maxHight+1; j++) {
				array[i][j] = 0;
			}
		}
	}
	
	public static int biMeanGauss(int thrVal, FileWriter outFile2, FileWriter outFile3) throws IOException {
		double sum1, sum2, total, minSumDiff;
		int bestThr = thrVal;
		minSumDiff = 999999.0;
		while(thrVal < (maxVal - offSet)) {
			set1DZero(GaussAry);
			set2DZero(GaussGraph);
			set2DZero(gapGraph);
			sum1 = fitGauss(0, thrVal, GaussAry, GaussGraph);
			sum2 = fitGauss(thrVal, maxVal, GaussAry, GaussGraph);
			total = sum1 + sum2;
			if (total < minSumDiff) {
				minSumDiff = total;
				bestThr = thrVal;
			}
			thrVal++;
			prettyPrint(GaussGraph, outFile2);
			plotGaps(histAry,GaussGraph, gapGraph);
			prettyPrint(gapGraph, outFile3);
		}
		return bestThr;
	}
	
	public static double fitGauss(int leftIndex, int rightIndex, int GaussAry[], int GaussGraph[][]) {
		double mean, var, Gval, maxGval , sum;
		int index;
		sum = 0;
		mean = computeMean(leftIndex,rightIndex,maxHight);
		var = computeVar(leftIndex, rightIndex, mean);
		index = leftIndex;
		while(index <= rightIndex) {
			Gval = modifiedGauss(index, mean, var, maxHight);
			sum += Math.abs(Gval - (double)histAry[index]);
			GaussAry[index] = (int)Gval;
			GaussGraph[index][(int)Gval] = 1;
			index++;
		}
		return sum;
			
	}
		
	public static void bestThrPlot(int bestThrVal) {
		double sum1, sum2;
		set1DZero(GaussAry);
		set2DZero(GaussGraph);
		set2DZero(gapGraph);
		sum1 = fitGauss(0, bestThrVal, GaussAry, GaussGraph);
		sum2 = fitGauss(bestThrVal, maxVal, GaussAry, GaussGraph);
		plotGaps(histAry, GaussGraph, gapGraph);
	}
	
	public static void prettyPrint(int histGraph[][], FileWriter outFile) throws IOException {
		for (int i = 0; i < maxVal+1; i++) {
			for (int j = 0; j < maxHight+1; j++) {
				if (histGraph[i][j] <= 0) {
					outFile.write(" ");
				}
				else {
					outFile.write("*");
				}
			}
			outFile.write("\n");
		}
	}
	
	public static void plotGaps(int [] histAry, int[][] GaussGraph, int [][]gapGraph) {
		int index = minVal;
		while(index < maxVal) {
			int first = Math.min(histAry[index], GaussAry[index]);
			int last = Math.max(histAry[index], GaussAry[index]);
			while (first < last) {
				gapGraph[index][first] = 1;
				first++;
			}
			index++;
		}
	}
	
	public static int getMax(int[] histAry){ 
	    int maxValue = histAry[0]; 
	    for(int i=1;i < histAry.length;i++){ 
	      if(histAry[i] > maxValue){ 
	         maxValue = histAry[i]; 
	      } 
	    } 
	    return maxValue; 
	  }


}

