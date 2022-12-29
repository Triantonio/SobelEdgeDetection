#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QFileDialog>

#include <iostream>
#include <math.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/objdetect/objdetect.hpp>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mUi(new Ui::MainWindow)
{
    mUi->setupUi(this);
    connectWidgets();
}

MainWindow::~MainWindow()
{
    delete mUi;
}

void MainWindow::connectWidgets()
{
    connect(mUi->LoadImagePushButton, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(mUi->SobelPushButton, &QPushButton::clicked, this, &MainWindow::sobelImage);
    connect(mUi->OpencvSobelPushButton, &QPushButton::clicked, this, &MainWindow::sobelOpencvImage);
}

void MainWindow::loadImage()
{
    QString qstringImagePath = QFileDialog::getOpenFileName(this, tr("Open Image"),"",tr("PPM File (*.ppm);;JPEG File (*.jpeg);;PNG File (*.png);;JPG File (*.jpg)"));

    mImagePath = qstringImagePath.toStdString();

    if(!mImagePath.empty())
    {
        mUi->SobelPushButton->setEnabled(true);
        mUi->OpencvSobelPushButton->setEnabled(true);
    }

    QPixmap pixmap(mImagePath.c_str());
    mUi->InputLabel->setPixmap(pixmap);
}

void MainWindow::sobelImage()
{
    // Reading image
    Mat img = imread(mImagePath);

    // Converting to grayscale
    Mat imgGray,imageBlur;
    GaussianBlur( img, imageBlur, Size(5,5), 3, 3);
    cvtColor(imageBlur,imgGray,COLOR_RGB2GRAY);

    mCols = imgGray.cols;
    mRows = imgGray.rows;

    int radius = 1;

    // Handle border issues
    Mat source;

    //Set the borders
    copyMakeBorder(imgGray, source, radius, radius, radius, radius, BORDER_REFLECT101);

    // Create output matrix
    Mat gradientX = imgGray.clone();
    Mat gradientY = imgGray.clone();
    Mat gradientMerged = imgGray.clone();

    // Correlation loop in x direction

    // Iterate on image
    for (int x = radius; x < source.rows - radius; ++x)
    {
        for (int y = radius; y < source.cols - radius; ++y)
        {
            int sourcePixel = 0;

            // Iterate on kernel
            for (int i = -radius; i <= radius; ++i)
            {
                for (int j = -radius; j <= radius; ++j)
                {
                    sourcePixel += source.at<uchar>(x + i, y + j) * mSobelX[i + radius][j + radius];
                }
            }
            gradientX.at<uchar>(x - radius, y - radius) = sourcePixel/30;
        }
    }

    Mat absGradX;
    convertScaleAbs( gradientX, absGradX );

    // Conrrelation loop in y direction

    // Iterate on image
    for (int x = radius; x < source.rows - radius; ++x)
    {
        for (int y = radius; y < source.cols - radius; ++y)
        {
            int sourcePixel = 0;

            // Iterate on kernel
            for (int i = -radius; i <= radius; ++i)
            {
                for (int j = -radius; j <= radius; ++j)
                {
                    sourcePixel += source.at<uchar>(x + i, y + j) * mSobelY[i + radius][j + radius];
                }
            }

            gradientY.at<uchar>(x - radius, y - radius) = sourcePixel/30;
        }
    }

    Mat absGradY;
    convertScaleAbs( gradientY, absGradY );

    //Calculating gradient magnitude
    for(int i=0; i<gradientMerged.rows; i++)
    {
        for(int j=0; j<gradientMerged.cols; j++)
        {
            gradientMerged.at<uchar>(i,j) = sqrt( pow(gradientX.at<uchar>(i,j),2) + pow(gradientY.at<uchar>(i,j),2) );

             if(gradientMerged.at<uchar>(i,j) >240)
                gradientMerged.at<uchar>(i,j) = 100;
            else
                gradientMerged.at<uchar>(i,j) = 0;
        }
    }

    imshow("Own Sobel Code Image",gradientMerged);

    //Sobel edge detection function from OpenCV
    cv::Mat Gx, Gy; int kSize=3;
    Mat absGradXOpencv, absGradYOpencv;
    cv::Sobel(imgGray, Gx, CV_8U, 1, 0, kSize);
    convertScaleAbs( Gx, absGradXOpencv );
    cv::Sobel(imgGray, Gy, CV_8U, 0, 1, kSize);
    convertScaleAbs( Gy, absGradYOpencv );
    Mat opencvGradient;
    addWeighted( absGradXOpencv, 0.5, absGradYOpencv, 0.5, 0, opencvGradient);

    imshow("Opencv Sobel Image",opencvGradient);
}

void MainWindow::sobelOpencvImage()
{
    cv::Mat img = cv::imread(mImagePath, cv::IMREAD_GRAYSCALE);
    cv::Mat outputImg;

    int dx = 1;
    int dy = 1;
    int sobelKernelSize = 3;
    int scaleFactor = 1;
    int deltaValue = 1;

    while(1)
    {

        cv::Sobel(img, outputImg, CV_8UC1, dx, dy, sobelKernelSize, scaleFactor, deltaValue);

        int keyPressed = cv::waitKey(1);

        if((char)keyPressed == 'q' || (char)keyPressed == 'Q')
        {
            cv::destroyWindow("Output");
            break;
        }
        else if((char)keyPressed=='a' || (char)keyPressed == 'A')
        {
            if(dx && dy)
                dx = 0;
            else
                dy =1;
        }
        else if((char)keyPressed=='s' || (char)keyPressed == 'S')
        {
            if(dx && dy)
                dy = 0;
            else
                dx =1;
        }
        else if((char)keyPressed=='d' || (char)keyPressed == 'D')
        {
            sobelKernelSize += 2;
        }
        else if((char)keyPressed=='f' || (char)keyPressed == 'F')
        {
            if(sobelKernelSize>1)
                sobelKernelSize -=2;
        }
        else if((char)keyPressed=='z' || (char)keyPressed == 'Z')
        {
            scaleFactor++;
        }
        else if((char)keyPressed=='x' || (char)keyPressed == 'X')
        {
            scaleFactor--;
        }

    cv::imshow("Output",outputImg);
    }
}
