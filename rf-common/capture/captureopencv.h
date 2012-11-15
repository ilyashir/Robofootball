#pragma once

#include <opencv/highgui.h>

#include "captureinterface.h"

class CaptureOpenCv : public CaptureInterface
{
public:
    CaptureOpenCv(VarList * _settings=0);
    virtual ~CaptureOpenCv();

    virtual RawImage getFrame();
    virtual bool isCapturing();
    virtual void releaseFrame();
    virtual bool startCapture();
    virtual bool stopCapture();
    virtual string getCaptureMethodName() const;

private:
	RawImage result;
    cv::VideoCapture mCapture;
    VarList *mCaptureSettings;
    VarString *mIndex;
    QMutex mutex;
};
