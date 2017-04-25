#-------------------------------------------------
#
# Project created by QtCreator 2017-04-24T17:09:14
#
#-------------------------------------------------

QT       += core gui network widgets


TARGET = RPi_Video
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


# OPEN CV
INCLUDEPATH += "C:/dev/OpenCV/OpenCV_3_0/opencv/build/include"
LIBS        += -L"C:/dev/OpenCV/OpenCV_3_0/Build_Shared_64bit/lib/Release" \
                -lopencv_core300        \
                -lopencv_videoio300     \
                -lopencv_imgproc300     \
                -lopencv_highgui300     \
                -lopencv_video300       \
                -lopencv_videostab300   \
                -lopencv_imgcodecs300   \
                -lopencv_calib3d300     \
                -lopencv_features2d300  \
                -lopencv_flann300       \
                -lopencv_hal300         \
                -lopencv_ml300          \
                -lopencv_objdetect300   \
                -lopencv_photo300       \
                -lopencv_shape300       \
                -lopencv_stitching300   \
                -lopencv_superres300    \
                -lopencv_ts300
