#ifndef INITCAMERATHREAD_H
#define INITCAMERATHREAD_H

#include <QThread>
#include <linux/videodev2.h>
#include "public.h"

class InitCameraThread : public QThread
{
    Q_OBJECT

#define TFAIL -1
#define TPASS 0

public:
    InitCameraThread();
    ~InitCameraThread();
    int init(CameraStruct *camera,int theCamera);
    void setData(CameraStruct *camera,int theCamera,int tmpinitG2DBufferStatus = true);

private:
    void run();
    int start_capturing(CameraStruct *camera);
    int  v4l_capture_setup(CameraStruct *camera);
    int prepare_g2d_buffers(CameraStruct *camera,int theCamera);

public:
    static const unsigned int bufferCount = 4;
    int countCamera = 4;
//    struct g2d_buf *g_g2d_dst_buffer[NUMBER_BUFFERS];
//    struct g2d_buf *g_g2d_test_buffer[NUMBER_BUFFERS];
    unsigned char * startData = NULL;

private:

    CameraStruct *camera;
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

     static bool initG2DBufferStatus;

signals:
     void finishInit();
};

#endif // INITCAMERATHREAD_H
