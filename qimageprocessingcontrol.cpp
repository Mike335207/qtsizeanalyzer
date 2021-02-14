#include "qimageprocessingcontrol.h"
#include "const.h"
#include "utils.h"

#include <QDebug>

using namespace std;
using namespace cv;

QImageProcessingControl::QImageProcessingControl(QObject *parent) : QObject(parent)
{

}

bool QImageProcessingControl::calibrateCamera(cv::Mat frame)
{
    Mat tempFrame;
    if (!frame.empty())
    {
        frame.copyTo(tempFrame);

        //just test image corners areas
        Rect ROI_1 = Rect(0, 0, tempFrame.cols* CALIB_TEST_RATIO, tempFrame.rows* CALIB_TEST_RATIO);
        Mat subMat1 = tempFrame(ROI_1);
        vector<RotatedRect> boxes1 = detectSquares(subMat1);


        Rect ROI_2 = Rect(tempFrame.cols*(1.0 - CALIB_TEST_RATIO), 0, tempFrame.cols* CALIB_TEST_RATIO, tempFrame.rows* CALIB_TEST_RATIO);
        Mat subMat2 = tempFrame(ROI_2);
        vector<RotatedRect> boxes2 = detectSquares(subMat2);


        Rect ROI_3 = Rect(0, tempFrame.rows*(1.0-CALIB_TEST_RATIO), tempFrame.cols* CALIB_TEST_RATIO, tempFrame.rows* CALIB_TEST_RATIO);
        Mat subMat3 = tempFrame(ROI_3);
        vector<RotatedRect> boxes3 = detectSquares(subMat3);


        Rect ROI_4 = Rect(tempFrame.cols*(1.0 - CALIB_TEST_RATIO), tempFrame.rows*(1.0-CALIB_TEST_RATIO), tempFrame.cols* CALIB_TEST_RATIO, tempFrame.rows* CALIB_TEST_RATIO);
        Mat subMat4 = tempFrame(ROI_4);
        vector<RotatedRect> boxes4 = detectSquares(subMat4);

        isCamCalibrated = ((boxes1.size() > 0) && (boxes2.size() > 0) && (boxes3.size() > 0) && (boxes4.size() > 0));

        if (!isCamCalibrated) return false;

        //perform calibration
        double pixSizeAccum = 0.0;

        /*double pixSize1 = calculatePixelSizeInMilimeters(boxes1[0], SQUARE_SIDE_MM);
        double pixSize2 = calculatePixelSizeInMilimeters(boxes2[0], SQUARE_SIDE_MM);
        double pixSize3 = calculatePixelSizeInMilimeters(boxes3[0], SQUARE_SIDE_MM);
        double pixSize4 = calculatePixelSizeInMilimeters(boxes4[0], SQUARE_SIDE_MM);
        qDebug() << "pixSizes: " << pixSize1 << "; " <<  pixSize2 << "; " <<  pixSize3 << "; " <<  pixSize4;*/
        pixSizeAccum += calculatePixelSizeInMilimeters(boxes1[0], SQUARE_SIDE_MM);
        pixSizeAccum += calculatePixelSizeInMilimeters(boxes2[0], SQUARE_SIDE_MM);
        pixSizeAccum += calculatePixelSizeInMilimeters(boxes3[0], SQUARE_SIDE_MM);
        pixSizeAccum += calculatePixelSizeInMilimeters(boxes4[0], SQUARE_SIDE_MM);

        calibrParams.pixelSizeInMilims = pixSizeAccum/4.0;
        qDebug() << "PixelSizeImMilim =" <<  calibrParams.pixelSizeInMilims;



        // recover global ROI
        vector<Point2f> globalROIVertexes;  //clockwise order, 1, 2, 4, 3
        globalROIVertexes.push_back(boxes1[0].center);
        globalROIVertexes.push_back(boxes2[0].center + Point2f(ROI_2.x, ROI_2.y));
        globalROIVertexes.push_back(boxes4[0].center + Point2f(ROI_4.x, ROI_4.y));
        globalROIVertexes.push_back(boxes3[0].center + Point2f(ROI_3.x, ROI_3.y));

        calibrParams.rRect =  minAreaRect(globalROIVertexes);
        calibrParams.bRect = calibrParams.rRect.boundingRect();

        rectangle(tempFrame, calibrParams.bRect, Scalar(0, 0, 255), 3);

        for(int i = 0; i < globalROIVertexes.size()-1; i++)
        {
            line(tempFrame, globalROIVertexes[i], globalROIVertexes[i+1], Scalar(0, 255, 0), 2);
        }

        line(tempFrame, globalROIVertexes[0], globalROIVertexes[globalROIVertexes.size()-1], Scalar(0, 255, 0), 2);

        cv::resize(tempFrame, tempFrame, Size(tempFrame.cols*SCALING_F, tempFrame.rows*SCALING_F));
        imshow("CALIBRATION", tempFrame);

        isCamCalibrated = (calibrParams.bRect.area() > 0);

        return isCamCalibrated;

    } else return false;
}

void QImageProcessingControl::performItemExtraction(Mat croppedFrame, vector<vector<Point>>& contours, vector<Vec4i>& hierarchy, int& largestContourID)
{
    Mat grayImg, resultImg;
    int largestArea = 0;
    largestContourID = -1;
    contours.clear();
    hierarchy.clear();
    croppedFrame.copyTo(resultImg);

    cvtColor(croppedFrame, grayImg, CV_BGRA2GRAY);
    GaussianBlur(grayImg, grayImg, Size(GAUSS_KERN_SIZE, GAUSS_KERN_SIZE), 0);
    //threshold(grayImg, grayImg, 127, 255, CV_THRESH_BINARY);
    adaptiveThreshold(grayImg, grayImg, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 5, 2);

    //imshow("GRAY_IMG", grayImg);

    //perform edge detection, then perform a dilation + erosion to
    //close gaps in between object edges
    Mat edgedImg;
    Canny(grayImg, edgedImg, 0, THRESH, 5);
    dilate(edgedImg, edgedImg, Mat(), Point(-1,-1));
    erode(edgedImg, edgedImg, getStructuringElement(MORPH_RECT, Size(3, 3)));

    /*Mat debugImg;
    resize(edgedImg, debugImg, Size(edgedImg.cols/2, edgedImg.rows/2));
    imshow("EDGED_IMG", debugImg);*/


    //find contours in the edge map
    //findContours(edgedImg, contours, /*CV_RETR_LIST*/CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    findContours(edgedImg, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image

    for(int i = 0; i < contours.size(); i++ ) // iterate through each contour.
    {
          double currArea=contourArea( contours[i],false);  //  Find the area of contour
          if(currArea > largestArea)
          {
            largestArea = currArea;
            largestContourID = i;                //Store the index of largest contour
            //bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
          }

    }

    /*for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
    {

        Scalar color( 0,255, 0);
        drawContours(croppedFrame, contours,i, color, 3, 8, hierarchy );
    }*/
}

double QImageProcessingControl::getOrientation(vector<Point> &pts, Mat &img)
{
    //Construct a buffer used by the pca analysis
    Mat data_pts = Mat(pts.size(), 2, CV_64FC1);
    for (int i = 0; i < data_pts.rows; ++i)
    {
        data_pts.at<double>(i, 0) = pts[i].x;
        data_pts.at<double>(i, 1) = pts[i].y;
    }

    //Perform PCA analysis
    PCA pca_analysis(data_pts, Mat(), CV_PCA_DATA_AS_ROW);

    //Store the position of the object
    Point pos = Point(pca_analysis.mean.at<double>(0, 0),
                      pca_analysis.mean.at<double>(0, 1));

    //Store the eigenvalues and eigenvectors
    vector<Point2d> eigen_vecs(2);
    vector<double> eigen_val(2);
    for (int i = 0; i < 2; ++i)
    {
        eigen_vecs[i] = Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
                                pca_analysis.eigenvectors.at<double>(i, 1));

        eigen_val[i] = pca_analysis.eigenvalues.at<double>(0, i);
    }

    // Draw the principal components
    /*circle(img, pos, 3, CV_RGB(255, 0, 255), 2);
    line(img, pos, pos + 0.02 * Point(eigen_vecs[0].x * eigen_val[0], eigen_vecs[0].y * eigen_val[0]) , CV_RGB(255, 255, 0));
    line(img, pos, pos + 0.02 * Point(eigen_vecs[1].x * eigen_val[1], eigen_vecs[1].y * eigen_val[1]) , CV_RGB(0, 255, 255));
*/
    return atan2(eigen_vecs[0].y, eigen_vecs[0].x);
}

void QImageProcessingControl::rotateImage(Mat &img, double angle, Point2f center)
{
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    // determine bounding rectangle, center not relevant
    cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), img.size(), angle).boundingRect2f();
    // adjust transformation matrix
    rot.at<double>(0,2) += bbox.width/2.0 - center.x;
    rot.at<double>(1,2) += bbox.height/2.0 - center.y;

    cv::Mat dst;
    cv::warpAffine(img, dst, rot, bbox.size());
    Point2f centerT;
    vector<Point3d> p = { Point3d(center.x, center.y, 1)};
    Mat pDst;
    pDst = (rot*Mat(p[0])).t();
    cv::Point2d p_(pDst);

    std::cout<< p[0] <<" ---> "<<pDst<<"   " <<p_  <<"\n";

    Rect cropRect = Rect(p_.x - img.cols/2, p_.y - img.rows/2, img.cols, img.rows);
    Mat output = dst(cropRect);

    /*Mat grayscale;
    cvtColor(output, grayscale, CV_RGBA2GRAY);
    cv::threshold(grayscale, grayscale, 1, 255, CV_THRESH_BINARY);
    imshow("THR", grayscale);

    output.copyTo(img, grayscale);*/
    output.copyTo(img);
}
