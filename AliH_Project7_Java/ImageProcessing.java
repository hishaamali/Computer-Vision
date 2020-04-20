import java.util.*;
import java.io.*;

public class ImageProcessing{
    int numRows, numCols, minVal, maxVal, newMinVal, newMaxVal;
    int [][] zeroFramedAry;
    int [][] skeletonAry;

    public void setZero(int [][] ary){
        for (int i = 0; i < numRows + 2; i++){
            for(int j = 0; j < numCols + 2; j++){
                ary[i][j] = 0;
            }
        }
    }

    public void loadImage(int [][] ary, Scanner s) throws IOException{
        for (int i = 1; i < numRows + 1; i++){
            for (int j = 1; j < numCols + 1; j++){
                ary[i][j] = s.nextInt();
            }
        }
    }

    public void firstPass_8Distance(int [][] ary){
        for (int i = 1; i < numRows + 1; i++){
            for (int j = 1; j < numCols + 1; j++){
                if (ary[i][j] > 0) {
                    ary[i][j] = findMin(ary[i - 1][j - 1], ary[i - 1][j], ary[i - 1][j + 1], ary[i][j - 1]) + 1;
                }
            }
        }
    }

    public void secondPass_8Distance(int [][] ary){
        newMinVal = ary[1][1];
        newMaxVal =  ary[1][1];
        for (int i = numRows; i > 0; i--){
            for (int j = numCols; j > 0; j--){
                if (ary[i][j] > 0){
                    ary[i][j] = Math.min(ary[i][j], 1 + findMin(ary[i][j+1], ary[i+1][j-1], ary[i + 1][j], ary[i + 1][j + 1]));
                    if (ary[i][j] < newMinVal){
                        newMinVal = ary[i][j];
                    }
                    if(ary[i][j] > newMaxVal){
                        newMaxVal = ary[i][j];
                    }
                }
            }
        }
    }

    public int findMin(int a, int b, int c, int d){
        int[] array = new int [4];
        array[0] = a;
        array[1] = b;
        array[2] = c;
        array[3] = d;
        Arrays.sort(array);
        return array[0];
    }

    public int findMax(int a, int b, int c, int d, int e, int f, int g, int h){
        int[] array = new int [8];
        array[0] = a;
        array[1] = b;
        array[2] = c;
        array[3] = d;
        array[4] = e;
        array[5] = f;
        array[6] = g;
        array[7] = h;
        Arrays.sort(array);
        return array[7];
    }

    public void Compute8Distance(int [][] ary, FileWriter w) throws IOException{
        firstPass_8Distance(ary);
        prettyPrint(ary, w, "first");
        secondPass_8Distance(ary);
        prettyPrint(ary, w, "second");

    }

    public void prettyPrint(int [][] ary, FileWriter w, String whichPass) throws IOException{
        if (whichPass == "first") w.write("1st Pass Distance Tranformation:" + "\n");
        if (whichPass == "second") w.write("2nd Pass Distance Transformation:" + "\n");
        if (whichPass == "local") w.write("Local Maxima:" + "\n");
        if (whichPass == "firstex") w.write("1st Pass Expansion:" + "\n");
        if (whichPass == "secondex") w.write("2nd Pass Expansion:" + "\n");
        for (int i = 1; i < numRows + 1; i++){
            for (int j = 1; j < numCols + 1; j++){
                if (ary[i][j] > 0) {
                    w.write(ary[i][j] + " ");
                }
                else w.write(" " + " ");
            }
            w.write("\n");
        }
        w.write("\n");
    }

    public void skeletonExtraction(int [][] ary, int [][] skary, FileWriter s, FileWriter o) throws IOException{
        computeLocalMaxima(ary, skary);
        prettyPrint(skary, o, "local");
        extractLocalMaxima(skary, s);
        s.close();
    }

    public void extractLocalMaxima(int [][] ary, FileWriter s) throws IOException{
        s.write(numRows + " " + numCols + " " + newMinVal + " " + newMaxVal + "\n");
        for (int i = 1; i < numRows + 1; i++){
            for (int j = 1; j < numCols + 1; j++){
                if (ary[i][j] > 0 ) s.write(i + " " + j + " " + ary[i][j]+ "\n");
            }
        }
    }

    public void computeLocalMaxima(int [][] ary, int [][] skary){
        for (int i = 1; i < numRows + 1; i++){
            for (int j = 1; j < numCols + 1; j++){
                if (ary[i][j] > 0 && isLocalMaxima(ary, i, j)){
                    skary[i][j] = ary[i][j];
                }
                else skary[i][j] = 0;
            }
        }
    }


    public boolean isLocalMaxima(int [][] ary, int i , int j){
        if (ary[i][j] >= ary[i - 1][j - 1] && ary[i][j] >= ary[i - 1][j] && ary[i][j] >= ary[i - 1][j + 1] && ary[i][j] >= ary[i][j - 1] && ary[i][j] >= ary[i][j + 1] && ary[i][j] >= ary[i + 1][j - 1] && ary[i][j] >= ary[i + 1][j] &&ary[i][j] >= ary[i + 1][j + 1]) {
            return true;
        }
        else return false;
    }

    public void skeletonExpansion(int ary[][], Scanner s, FileWriter o) throws IOException{
        setZero(ary);
        int numRows = s.nextInt();
        int numCols = s.nextInt();
        int minVal = s.nextInt();
        int maxVal = s.nextInt();
        load(ary, s);
        firstPassExpansion(ary);
        prettyPrint(ary, o, "firstex");
        seconPassExpansion(ary);
        prettyPrint(ary, o, "secondex");

    }

    public void load(int [][] ary, Scanner s){
        while(s.hasNextInt()){
            ary[s.nextInt()][s.nextInt()] = s.nextInt();
        }
    }

    public void firstPassExpansion(int [][] ary){
        for (int i = 1; i < numRows + 1; i++){
            for (int j = 1; j < numCols + 1; j++){
                if (ary[i][j] == 0){
                    ary[i][j] = Math.max(0, findMax(ary[i - 1][j - 1], ary[i - 1][j], ary[i - 1][j + 1], ary[i][j - 1], ary[i][j + 1], ary[i + 1][j - 1], ary[i + 1][j], ary[i + 1][j + 1]) - 1);
                }
            }
        }
    }

    public void seconPassExpansion(int ary[][]){
        for (int i = numRows; i > 0; i--){
            for (int j = numCols; j > 0; j--){
                ary[i][j] = Math.max(ary[i][j], findMax(ary[i - 1][j - 1], ary[i - 1][j], ary[i - 1][j + 1], ary[i][j - 1], ary[i][j + 1], ary[i + 1][j - 1], ary[i + 1][j], ary[i + 1][j + 1]) - 1);
            }
        }
    }
    public void ary2File(int [][] ary, FileWriter o) throws IOException{
        o.write(numRows + " " + numCols + " " + newMinVal + " " + newMaxVal + "\n");
        for (int i = 1; i < numRows + 1; i++){
            for (int j = 1; j < numCols + 1; j++){
                if (ary[i][j] >= 1){
                    o.write("1" + " ");
                }
                else o.write("0" + " ");
            }
            o.write("\n");
        }
    }
}
