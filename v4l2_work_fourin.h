#ifndef V4L2_WORK_FOURIN_H
#define V4L2_WORK_FOURIN_H

#include <QThread>
#include <QString>
#include  <QObject>
#include "g2d_yuyv_bgra.h"
#include "g2d.h"
#include <linux/videodev2.h>
#include "public.h"
#include "initcamerathread.h"

class CameraWorkFourin : public QThread
{

    struct CameraStatusBool{
        bool topCameraStatus = true;
        bool bottomCameraStatus = true;
        bool leftCameraStatus = true;
        bool rightCameraStatus = true;
    };

    Q_OBJECT

public:
    CameraWorkFourin(QObject *parent = Q_NULLPTR);
    ~CameraWorkFourin();

    void setCameraName(CameraStruct *camera,QString name);
    int video_init();
    void setWhichCamera(int theCamera);
    unsigned char * setCameraData(unsigned char **data);
    void CloseCamera();

protected:
     int init(CameraStruct *camera,int theCamera);
     int start_capturing(CameraStruct *camera);
     int  v4l_capture_setup(CameraStruct *camera);
    void uninit(CameraStruct *camera,int theCamera);
    int cameraRun(CameraStruct *camera,int theCamera);
    int read_frame(CameraStruct *camera,int theCamera);
     int prepare_g2d_buffers(CameraStruct *camera,int theCamera);

protected:
    void run();

private slots:
    void finishTopCameraWork();
    void finishBottomCameraWork();
    void finishLeftCameraWork();
    void finishRightCameraWork();

private:
    bool finishInitTop = false;
    bool finishInitBottoom = false;
    bool finishInitLeft = false;
    bool finishInitRight = false;



signals: void Frame(unsigned int);

public:
    static const unsigned int bufferCount = 4;
    int countCamera = 4;
//    struct g2d_buf *g_g2d_dst_buffer[NUMBER_BUFFERS];
//    struct g2d_buf *g_g2d_test_buffer[NUMBER_BUFFERS];
    unsigned char * startData = NULL;

private:

    QString cameraName;
//    FourCameraStruct fourCamera;
    const bool analog = true;
    const unsigned int count = 4;
    const unsigned int width = 1280; //宽，必须是16的倍数
    const unsigned int height = 800; //高，必须是16的倍数
    unsigned int offsetCount = 0;

    int theCamera;


    char v4l_capture_dev[100];
    char save_file[100];
    int fd_capture_v4l = 0;
    int fd_fb_display = 0;
    int fd_ipu = 0;
    unsigned char * g_fb_display = NULL;
    int g_input = 1;
    int g_display_num_buffers = 3;
    int g_capture_num_buffers = NUMBER_BUFFERS;
    int g_in_width = 0;
    int g_in_height = 0;
    int g_in_fmt = V4L2_PIX_FMT_YUYV;
    int g_display_width = 1200;
    int g_display_height = 720;
    int g_display_top = 0;
    int g_display_left = 0;
    int g_display_fmt = V4L2_PIX_FMT_BGR32;
    int g_display_base_phy;
    int g_display_size;
    int g_display_fg = 1;
    int g_display_id = 1;
    //struct fb_var_screeninfo g_screen_info;
    //int g_frame_count = 0x7FFFFFFF;
    int g_frame_count = 0x1;
    int g_frame_size;
    bool g_g2d_render = 1;
    int g_g2d_fmt;
//    int g_mem_type = V4L2_MEMORY_MMAP;
    int g_mem_type = V4L2_MEMORY_USERPTR;
    struct testbuffer display_buffers[3];
    struct testbuffer capture_buffers[NUMBER_BUFFERS];

    unsigned long file_length;
     bool initG2DBufferStatus;

private:
     InitCameraThread initTopCamera;
     InitCameraThread initBottomCamera;
     InitCameraThread initLeftCamera;
     InitCameraThread initRightCamera;

     struct CameraStatusBool cameraStatus;
};




#endif // V4L2_WORK_FOURIN_H
