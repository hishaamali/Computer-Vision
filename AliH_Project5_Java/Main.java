import java.io.*;
import java.util.*;
import java.lang.Math;

public class Main {

    public static void main(String args[]) throws IOException {
        if (args.length != 6) {
            throw new IllegalArgumentException("Invalid number of arguments!");
        }
        Scanner Input = new Scanner(new File(args[0]));
        FileWriter RobertEdgeOut = new FileWriter(args[1]);
        FileWriter SobelEdgeOut = new FileWriter(args[2]);
        FileWriter GradientEdgeOut = new FileWriter(args[3]);
        FileWriter prettyOut = new FileWriter(args[4]);
        FileWriter debugOut = new FileWriter(args[5]);
        imageEdge img = new imageEdge();
        img.numRows = Input.nextInt();
        img.numCols = Input.nextInt();
        img.minVal = Input.nextInt();
        img.maxVal = Input.nextInt();
        img.mirroredFramedAry = new int[img.numRows + 2][img.numCols + 2];
        img.maskRobertRightDiag = new int[][]{{1, -1}, {-1, 1}};
        img.maskRobertLeftDiag = new int[][]{{-1, 1}, {1, -1}};
        img.maskSobelRightDiag = new int[][]{{2, 1, 0}, {1, 0, -1}, {0, -1, -2}};
        img.maskSobelLeftDiag = new int[][]{{0, 1, 2}, {-1, 0, 1}, {-2, -1, 0}};
        img.RobertRightDiag = new int[img.numRows + 2][img.numCols + 2];
        img.RobertLeftDiag = new int[img.numRows + 2][img.numCols + 2];
        img.SobelRightDiag = new int[img.numRows + 2][img.numCols + 2];
        img.SobelLeftDiag = new int[img.numRows + 2][img.numCols + 2];
        img.GradientEdge = new int[img.numRows + 2][img.numCols + 2];
        img.edgeSum = new int[img.numRows + 2][img.numCols + 2];
        img.loadImage(img.mirroredFramedAry, Input);
        img.mirrorFramed(img.mirroredFramedAry);
        for (int i = 1; i < img.numRows + 1; i++) {
            for (int j = 1; j < img.numCols + 1; j++) {
                img.RobertRightDiag[i][j] = Math.abs(img.convoluteRobert(i, j, img.maskRobertRightDiag));
                img.RobertLeftDiag[i][j] = Math.abs(img.convoluteRobert(i, j, img.maskRobertLeftDiag));
                img.SobelRightDiag[i][j] = Math.abs(img.convoluteSobel(i, j, img.maskSobelRightDiag));
                img.SobelLeftDiag[i][j] = Math.abs(img.convoluteSobel(i, j, img.maskSobelLeftDiag));
                img.GradientEdge[i][j] = img.computeGradient(i, j);
            }
        }
        img.addTwoArys(img.RobertRightDiag, img.RobertLeftDiag, img.edgeSum);
        debugOut.write("Robert Right Diag:" + "\n");
        img.outputToFile(img.RobertRightDiag, debugOut);
        prettyOut.write("Robert Right Diag:" + "\n");
        img.prettyPrint(img.RobertRightDiag, prettyOut);
        debugOut.write("Robert Left Diag:" + "\n");
        img.outputToFile(img.RobertLeftDiag, debugOut);
        prettyOut.write("Robert Left Diag:" + "\n");
        img.prettyPrint(img.RobertLeftDiag, prettyOut);
        int min = img.getMinValue(img.edgeSum);
        int max = img.getMaxValue(img.edgeSum);
        RobertEdgeOut.write(img.numRows + " " + img.numCols + " " + min + " " + max + "\n");
        for (int i = 1; i < img.numRows + 1; i++) {
            for (int j = 1; j < img.numCols + 1; j++) {
                RobertEdgeOut.write(img.edgeSum[i][j] + " ");
            }
            RobertEdgeOut.write("\n");
        }
        img.addTwoArys(img.SobelRightDiag, img.SobelLeftDiag, img.edgeSum);
        debugOut.write("Sobel Right Diag:" + "\n");
        img.outputToFile(img.SobelRightDiag, debugOut);
        prettyOut.write("Sobel Right Diag:" + "\n");
        img.prettyPrint(img.SobelRightDiag, prettyOut);
        debugOut.write("Sobel Left Diag:" + "\n");
        img.outputToFile(img.SobelLeftDiag, debugOut);
        prettyOut.write("Sobel Left Diag:" + "\n");
        img.prettyPrint(img.SobelLeftDiag, prettyOut);
        min = img.getMinValue(img.edgeSum);
        max = img.getMaxValue(img.edgeSum);
        SobelEdgeOut.write(img.numRows + " " + img.numCols + " " + min + " " + max + "\n");
        for (int i = 1; i < img.numRows + 1; i++) {
            for (int j = 1; j < img.numCols + 1; j++) {
                SobelEdgeOut.write(img.edgeSum[i][j] + " ");
            }
            SobelEdgeOut.write("\n");
        }
        min = img.getMinValue(img.GradientEdge);
        max = img.getMaxValue(img.GradientEdge);
        GradientEdgeOut.write(img.numRows + " " + img.numCols + " " + min + " " + max + "\n");
        for (int i = 1; i < img.numRows + 1; i++) {
            for (int j = 1; j < img.numCols + 1; j++) {
                GradientEdgeOut.write(img.GradientEdge[i][j] + " ");
            }
            GradientEdgeOut.write("\n");
        }
        Input.close();
        RobertEdgeOut.close();
        SobelEdgeOut.close();
        GradientEdgeOut.close();
        prettyOut.close();
        debugOut.close();
    }
}
