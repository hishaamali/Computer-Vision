import java.io.*;
import java.util.*;
public class Main {
    public static void main(String args[]) throws IOException {
        Scanner inFile = new Scanner(new File(args[0]));
        FileWriter outFile1 = new FileWriter(new File(args[1]));
        FileWriter outFile2 = new FileWriter(new File(args[2]));
        ImageProcessing ip = new ImageProcessing();
        HoughTransform ht = new HoughTransform();
        ip.numRows = inFile.nextInt();
        ip.numCols = inFile.nextInt();
        ip.minVal = inFile.nextInt();
        ip.maxVal = inFile.nextInt();
        ht.HoughAngle = 180;
        ht.HoughDist = 2 * (int)(Math.sqrt((ip.numRows*ip.numRows + ip.numCols*ip.numCols)));
        ip.imgAry = new int[ip.numRows][ip.numCols];
        ht.HoughAry = new int[ht.HoughDist][ht.HoughAngle];
        ip.loadImage(inFile, ip.imgAry);
        ht.buildHoughSpace(ip.imgAry, ip.numRows, ip.numCols);
        ht.prettyPrint(ht.HoughAry, outFile1);
        ht.determineMinMax(ht.HoughAry);
        outFile2.write(ht.HoughDist + " " + ht.HoughAngle + " " + ht.HoughMinVal + " " + ht.HoughMaxVal + "\n");
        ht.ary2File(ht.HoughAry, outFile2);
        inFile.close();
        outFile1.close();
        outFile2.close();
    }
}
