import java.io.*;
import java.util.*;
import java.lang.Math.*;
public class HoughTransform {

    int angleInDegree;
    double angleInRadians;
    int HoughDist;
    int HoughAngle;
    int HoughMinVal;
    int HoughMaxVal;
    int [][] HoughAry;
    xyCoord point = new xyCoord();
    double dist;

    public void buildHoughSpace(int [][] ary, int numRows, int numCols){
        for (int r = 0; r < numRows; r++){
            for (int c = 0; c < numCols; c++){
                if (ary[r][c] > 0) {
                    point.x = c;
                    point.y = r;
                    angleInDegree = 0;
                    while (angleInDegree <= 179) {
                        angleInRadians = angleInDegree / 180.00 * Math.PI;
                        dist = computeDistance(point, angleInRadians);
                        int distint = (int) dist;
                        HoughAry[distint][angleInDegree]++;
                        angleInDegree++;
                    }
                }
            }
        }
    }

    public double computeDistance(xyCoord point, double angleInRadians){
        double t = angleInRadians - Math.atan((double)point.y / (double)point.x) - (Math.PI / 2);
        double offset = HoughDist / 2;
        double d = Math.sqrt(((double)point.x * (double)point.x) + ((double)point.y * (double)point.y)) * Math.cos(t) + offset;
        return d;
    }

    public void determineMinMax(int [][] ary){
        HoughMinVal = ary[0][0];
        HoughMaxVal = ary[0][0];
        for (int i = 0; i < HoughDist; i++){
            for (int j = 0; j < HoughAngle; j++){
                if (ary[i][j] < HoughMinVal){
                    HoughMinVal = ary[i][j];
                }
                if (ary[i][j] > HoughMaxVal){
                    HoughMaxVal = ary[i][j];
                }
            }
        }
    }

    public void prettyPrint(int [][] ary , FileWriter o) throws IOException {
        for (int i = 0; i < HoughDist; i++){
            for (int j = 0; j < HoughAngle; j++){
                if (ary[i][j] > 0) o.write(ary[i][j] + " ");
                else o.write("  ");
            }
            o.write("\n");
        }
    }

    public void ary2File(int ary [][] , FileWriter o) throws IOException {
        for (int i = 0; i < HoughDist; i++){
            for (int j = 0; j < HoughAngle; j++){
                o.write(ary[i][j] + " ");
            }
            o.write("\n");
        }
    }
}
