import java.util.*;
import java.io.*;
import java.lang.Math;
public class imageEdge {
    int numRows, numCols, minVal, maxVal;
    int [][] mirroredFramedAry;
    int [][] maskRobertRightDiag;
    int [][] maskRobertLeftDiag;
    int [][] maskSobelRightDiag;
    int [][] maskSobelLeftDiag;
    int [][] RobertRightDiag;
    int [][] RobertLeftDiag;
    int [][] SobelRightDiag;
    int [][] SobelLeftDiag;
    int [][] GradientEdge;
    int [][] edgeSum;
    int[][] test;

    public void loadImage(int [][] ary, Scanner Input) throws IOException{
        for (int i = 1; i < numRows + 1; i++){
            for (int j = 1; j < numCols + 1; j++){
                ary[i][j] = Input.nextInt();
            }
        }
    }

    public void mirrorFramed(int [][] ary){
        for (int i = 0; i < numRows + 2; i++){
            ary[i][0] = ary[i][1];
            ary[i][numCols + 1] = ary[i][numCols];
        }
        for (int j = 0; j < numCols + 2; j++){
            ary[0][j] = ary [1][j];
            ary[numRows + 1][j] = ary [numRows][j];
        }
    }

    public int convoluteRobert(int i, int j, int mask[][]){
        int sum = 0;
        int r = 0;
        int c = 0;
        for (int a = i; a < i + 2; a++){
            for (int b = j; b < j + 2; b++){
                sum += (mirroredFramedAry[a][b] * mask[r][c]);
                c++;
            }
            c = 0;
            r++;
        }
        return sum;
    }

    public int convoluteSobel(int i , int j, int mask[][]){
        int sum = 0;
        int r = 0;
        int c = 0;
        for (int a = i - 1; a < i + 2; a++){
            for (int b = j - 1; b < j + 2; b++) {
                sum += (mirroredFramedAry[a][b] * mask[r][c]);
                c++;
            }
            c = 0;
            r++;
        }
        return sum;
    }

    public int computeGradient(int i , int j){
        int sol = 0;
        int x = mirroredFramedAry[i][j];
        int a = mirroredFramedAry[i][j + 1];
        int b = mirroredFramedAry[i + 1][j];
        sol = (int) Math.sqrt((Math.pow(x - b, 2) + Math.pow(x - a, 2)));
        return sol;
    }


    public void addTwoArys(int [][] ary1, int [][] ary2, int [][] ary3){
        for (int i = 0; i < numRows + 2; i++){
            for (int j = 0; j < numCols + 2; j++){
                ary3[i][j] = ary1[i][j] + ary2[i][j];
            }
        }
    }

    public void outputToFile(int [][] ary , FileWriter w) throws IOException {
        for (int i = 1 ; i < numRows + 1; i++){
            for (int j = 1; j < numCols + 1; j++){
               w.write(ary[i][j] + " ");
            }
            w.write("\n");
        }
        w.write("\n");
    }

    public int getMaxValue(int[][] numbers) {
        int maxValue = numbers[1][1];
        for (int j = 1; j < numRows + 1; j++) {
            for (int i = 1; i < numCols + 1; i++) {
                if (numbers[j][i] > maxValue) {
                    maxValue = numbers[j][i];
                }
            }
        }
        return maxValue;
    }

    public int getMinValue(int[][] numbers) {
        int minValue = numbers[1][1];
        for (int j = 1; j < numRows + 1; j++) {
            for (int i = 1; i < numCols + 1; i++) {
                if (numbers[j][i] < minValue) {
                    minValue = numbers[j][i];
                }
            }
        }
        return minValue;
    }

    public void prettyPrint(int [][] ary, FileWriter w) throws IOException {
        for (int i = 1 ; i < numRows + 1; i++){
            for (int j = 1; j < numCols + 1; j++){
                if(ary[i][j] == 0) w.write(" ");
                else w.write(ary[i][j] + " ");
            }
            w.write("\n");
        }
        w.write("\n");
    }
}
