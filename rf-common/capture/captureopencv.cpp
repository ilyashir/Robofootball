#include "captureopencv.h"

#include <opencv/cv.h>
#include <opencv/cxcore.h>

#include "timer.h"
#include "image.h"

#include <QtCore/QDebug>

CaptureOpenCv::CaptureOpenCv(VarList * _settings)
    : CaptureInterface(_settings)
{
    mCaptureSettings = new VarList("Capture Settings");
    settings->addChild(mCaptureSettings);

    mIndex = new VarString("Camera index");
    mCaptureSettings->addChild(mIndex);

	printf("We work with OpenCv, init USB-cam\n");
}

CaptureOpenCv::~CaptureOpenCv()
{
}

RawImage CaptureOpenCv::getFrame()
{
    mutex.lock();
    //RawImage result;
    result.setTime(0.0);

    if (!mCapture.grab()) {
        qDebug() <<  "OpenCV Error: Failed to capture from camera" << mIndex;
        mCapture.release();
        result.allocate(COLOR_RGB8, 640, 480);
    } else {
        cv::Mat image;
        mCapture.retrieve(image);

        int const widthStep = image.step;
        int const height = image.rows;
        int const width = image.cols;
        int const channels = 3;

        result.allocate(COLOR_RGB8, width, height);

        rgbImage img;
        img.fromRawImage(result);

        for (int x = 0 ; x < width; x++) {
            for (int y = 0 ; y < height; y++) {
              rgb color2;
              color2.r = image.data[widthStep * y + channels * x + 2];
              color2.g = image.data[widthStep * y + channels * x + 1];
              color2.b = image.data[widthStep * y + channels * x];
              img.setPixel(x, y, color2);
          }
        }

        result.setWidth(width);
        result.setHeight(height);

        tval tv;
        gettimeofday(&tv,NULL);
        result.setTime((double)tv.tv_sec + tv.tv_usec*(1.0E-6));
    }
    mutex.unlock();
    return result;
}

bool CaptureOpenCv::isCapturing()
{
    return mCapture.isOpened();
}

void CaptureOpenCv::releaseFrame()
{
}

bool CaptureOpenCv::startCapture()
{
    std::string const indexStr = mIndex->getString();

    bool err = false;
    int const index = QString(indexStr.c_str()).toInt(&err);

    qDebug() << "Starting capture from" << index;

    if (err)
        mCapture.open(index);

    qDebug() << "Success: " << mCapture.isOpened();
    return mCapture.isOpened();
}

bool CaptureOpenCv::stopCapture()
{
    mCapture.release();
    return true;
}

string CaptureOpenCv::getCaptureMethodName() const
{
    return "OpenCV";
}
