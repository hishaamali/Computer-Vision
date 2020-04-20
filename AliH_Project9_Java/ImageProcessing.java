import java.io.*;
import java.util.*;
public class ImageProcessing {
    int numRows, numCols, minVal, maxVal;
    int [][] imgAry;

    public void loadImage(Scanner s, int [][] ary){
        for (int i = 0; i < numRows; i++){
            for (int j = 0; j < numCols; j++){
                ary[i][j] = s.nextInt();
            }
        }
    }
}
