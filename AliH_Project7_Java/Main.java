import java.util.*;
import java.io.*;
import java.lang.Object;


class Main {
    public static void main(String[] args) throws IOException {
        ImageProcessing p = new ImageProcessing();
        Scanner in = new Scanner(new File(args[0]));
        FileWriter outFile1 = new FileWriter(args[1]);
        FileWriter outFile2 = new FileWriter(args[2]);
        p.numRows = in.nextInt();
        p.numCols = in.nextInt();
        p.minVal = in.nextInt();
        p.maxVal = in.nextInt();
        p.zeroFramedAry = new int[p.numRows + 2][p.numCols + 2];
        p.skeletonAry = new int[p.numRows + 2][p.numCols + 2];
        String skeletonFileName = args[0].replace(".txt", "") + "_skeleton.txt";
        FileWriter skeletonFile = new FileWriter(skeletonFileName);
        String decompressedFileName = args[0].replace(".txt","") + "_decompressed.txt";
        FileWriter decompressedFile = new FileWriter(decompressedFileName);
        p.setZero(p.zeroFramedAry);
        p.setZero(p.skeletonAry);
        p.loadImage(p.zeroFramedAry, in);
        p.Compute8Distance(p.zeroFramedAry, outFile1);
        p.skeletonExtraction(p.zeroFramedAry, p.skeletonAry, skeletonFile, outFile1);
        Scanner skeletonFileTwo = new Scanner(new File(skeletonFileName));
        p.skeletonExpansion(p.zeroFramedAry, skeletonFileTwo, outFile2);
        p.ary2File(p.zeroFramedAry, decompressedFile);
        outFile1.close();
        outFile2.close();
        skeletonFile.close();
        decompressedFile.close();
    }
}