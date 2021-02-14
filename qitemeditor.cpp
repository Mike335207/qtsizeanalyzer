#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include "qitemeditor.h"
#include "ui_qitemeditor.h"
#include "utils.h"


#define SCALE 2
#define DIST_THR 5

#define MAX_ERROR_RADIANS 0.15


using namespace cv;
using namespace std;

QItemEditor::QItemEditor(cv::Mat currentFrame, calibrationParameters params, QWidget *parent):
    QDialog(parent),
    ui(new Ui::QItemEditor)
{
    ui->setupUi(this);
    calibParams = params;
    imgUtils = new QImageProcessingControl();
    croppedFrame = currentFrame(params.bRect);
    ui->lb_itemImg->setMaximumWidth(croppedFrame.cols/SCALE);
    ui->lb_itemImg->setMinimumWidth(croppedFrame.cols/SCALE);

    ui->lb_itemImg->setMaximumHeight(croppedFrame.rows/SCALE);
    ui->lb_itemImg->setMinimumHeight(croppedFrame.rows/SCALE);

    ui->lb_itemImg->setFrame(croppedFrame);
    ui->lb_itemImg->isRequestMouseMovePosition(true);
    ui->lb_itemImg->isRequestClickPosition(true);
    connect(ui->lb_itemImg, SIGNAL(mouseMovePosition(QPoint)), this, SLOT(testPoint(QPoint)));
    connect(ui->lb_itemImg, SIGNAL(mouseClickPosition(QPoint)), this, SLOT(drawArrow(QPoint)));

    std::vector<Vec4i> hierarchy;

    imgUtils->performItemExtraction(croppedFrame, contours, hierarchy, largestContourID);
    Scalar color( 255, 255, 255);
    drawContours(croppedFrame, contours, largestContourID, color, /*CV_FILLED*/4, 8, hierarchy );

    //////////////////////////////////////////////////////////////////////////////////////////

    Mat imgBlack = Mat::zeros(croppedFrame.rows, croppedFrame.cols,  croppedFrame.type());
    int numberOfPoints = (int)contours[largestContourID].size();
    const Point* elementPoints[1] = { &contours[largestContourID][0] };
    cv::fillPoly(imgBlack,elementPoints, &numberOfPoints, 1, Scalar(255, 255, 255));

    bitwise_and(croppedFrame, imgBlack, imgBlack);

    //get orientation
    double orientation = imgUtils->getOrientation(contours[largestContourID], imgBlack);

    //imshow("ORIENTATION", imgBlack);
    Moments mu;

    qDebug() << "Orientation: " << orientation;

    while (abs(orientation) > MAX_ERROR_RADIANS)
    {
        // Get the moment:
        mu = moments( contours[largestContourID], false );

        // Get the mass center:
        cv::Point2f mc = Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );

        double degrees = orientation * (180.0/3.141592653589793238463);
        imgUtils->rotateImage(imgBlack, degrees, mc);

        //get orientation
        imgUtils->performItemExtraction(imgBlack, contours, hierarchy, largestContourID);
        //drawContours(imgBlack, contours, largestContourID, color, /*CV_FILLED*/1, 8, hierarchy );
        orientation = imgUtils->getOrientation(contours[largestContourID], imgBlack);
        qDebug() << "Orientation: " << orientation;
    }


    /////////////////////////////////////////////////////////////////////////////////////////

    drawContours(imgBlack, contours, largestContourID, Scalar(0, 255, 0), /*CV_FILLED*/3, 8, hierarchy ); // Draw the largest contour using previously stored index.

    // Get the moment:
    //Moments mu;
    mu = moments( contours[largestContourID], false );

    // Get the mass center:
    mc = Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );
    cArea = contourArea(contours[largestContourID])/1000.0;

    //Mat debugMat;
    //cv::resize(croppedFrame, debugMat, Size(croppedFrame.cols/2, croppedFrame.rows/2));
    //imshow( "largest Contour",croppedFrame);

    ui->lb_itemImg->setFrame(imgBlack);
    imgBlack.copyTo(croppedFrame);

    /*QStringList labelsList;
    labelsList << "isActive" << "sizeID" << "SizeInMM" << "AdmErrorInMM";
    ui->tabWidget->setHorizontalHeaderLabels(labelsList);*/
}

QItemEditor::~QItemEditor()
{
    delete ui;
    delete imgUtils;
}

void QItemEditor::testPoint(QPoint pnt)
{
    cv::Point testPnt = cv::Point(pnt.x(), pnt.y());
    double distance  = pointPolygonTest(contours[largestContourID], testPnt*SCALE, true);
    qDebug() << "Distance:" << distance;

    if (abs(distance) < DIST_THR)
    {
        ui->lb_itemImg->setCursor(Qt::CrossCursor); // Set the cursor target
    } else
    {
        ui->lb_itemImg->setCursor(Qt::ArrowCursor);
    }


}

void QItemEditor::drawArrow(QPoint pnt)
{
    cv::Point testPnt = cv::Point(pnt.x(), pnt.y());
    double distance  = pointPolygonTest(contours[largestContourID], testPnt*SCALE, true);

    if (abs(distance) < DIST_THR)
    {
      usersPnts.append(testPnt*SCALE);
    }

    if (usersPnts.size() == 2)
    {
        //append new size
        sizeItem temp;
        temp.pnt1 = usersPnts[0];
        temp.pnt2 = usersPnts[1];
        temp.sizeInMilim = calculateDistanceBtw2Pnts(temp.pnt1, temp.pnt2)*calibParams.pixelSizeInMilims;

        sizes.append(temp);
        usersPnts.clear();

        //append size item
        ui->tabWidget->insertRow(ui->tabWidget->rowCount());
        QTableWidgetItem *itemID = new QTableWidgetItem(tr("%1__%2").arg(2*(sizes.length()-1)).arg(2*(sizes.length()-1)+1));
        itemID->setCheckState(Qt::Checked);
        ui->tabWidget->setItem(ui->tabWidget->rowCount()-1, 0, itemID);

        QTableWidgetItem *itemSize = new QTableWidgetItem(tr("%1").arg(temp.sizeInMilim));
        ui->tabWidget->setItem(ui->tabWidget->rowCount()-1, 1, itemSize);

        QTableWidgetItem *itemError = new QTableWidgetItem(tr("%1").arg(15));
        ui->tabWidget->setItem(ui->tabWidget->rowCount()-1, 2, itemError);





    }

    drawSizes();
}

void QItemEditor::drawSizes()
{
    Mat resMat;
    croppedFrame.copyTo(resMat);

    if (sizes.length() > 0)
    {
        for (int i = 0; i < sizes.length(); i++)
        {
            Point drawTextPnt = Point(-10 + (sizes[i].pnt1.x + sizes[i].pnt2.x)/2,  (sizes[i].pnt1.y + sizes[i].pnt2.y)/2);
            cv::putText(resMat, std::to_string(sizes[i].sizeInMilim), drawTextPnt, FONT_HERSHEY_DUPLEX, 1, Scalar(0,143,143), 2);
            line(resMat, sizes[i].pnt1, sizes[i].pnt2, Scalar(0, 255, 0), 2);
            cv::circle(resMat, sizes[i].pnt1, 10, Scalar(0, 255, 0), CV_FILLED, 5);
            cv::circle(resMat, sizes[i].pnt2, 10, Scalar(0, 255, 0), CV_FILLED, 5);

            cv::putText(resMat, std::to_string(i*2), sizes[i].pnt1, FONT_HERSHEY_DUPLEX, 1, Scalar(255, 0, 0), 2);
            cv::putText(resMat, std::to_string(i*2+1), sizes[i].pnt2, FONT_HERSHEY_DUPLEX, 1, Scalar(255, 0, 0), 2);

        }
    }

    if (usersPnts.size() == 1)
    {
        cv::circle(resMat, usersPnts[0], 10, Scalar(0, 255, 0), CV_FILLED, 5);
    }

     cv::circle(resMat, mc, 10, Scalar(255, 255, 255), CV_FILLED, 5);

    ui->lb_itemImg->setFrame(resMat);
}

void QItemEditor::on_buttonBox_accepted()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Item Model"), "",
            tr("Models (*.mod);;All Files (*)"));

    if (fileName.isEmpty()) return;
    else {
           QFile file(fileName);
           if (!file.open(QIODevice::WriteOnly)) {
               QMessageBox::information(this, tr("Unable to open file"),
                   file.errorString());
               return;
           }

           QTextStream stream(&file);
           QString text = tr("%1;%2;%3;%4;\n").arg(sizes.size()).arg(cArea).arg(croppedFrame.rows).arg(croppedFrame.cols);
           stream << text;

           for (int i = 0; i < ui->tabWidget->rowCount(); i++)
           {
               QString sizeID = ui->tabWidget->item(i, 0)->text();
               QString sizeMM = ui->tabWidget->item(i, 1)->text();
               QString errorMM = ui->tabWidget->item(i, 2)->text();
               Point2f startPointShifted = sizes[i].pnt1 - mc;
               Point2f endPointShifted = sizes[i].pnt2 - mc;

               QString startPntText = tr("%1;%2;").arg(startPointShifted.x/(float)croppedFrame.cols).arg(startPointShifted.y/(float)croppedFrame.rows);
               QString endPntText = tr("%1;%2;").arg(endPointShifted.x/(float)croppedFrame.cols).arg(endPointShifted.y/(float)croppedFrame.rows);

               text = tr("%1;%2%3%4;%5;\n").arg(sizeID).arg(startPntText).arg(endPntText).arg(sizeMM).arg(errorMM);

               stream << text;
           }


           file.close();
    }


}
