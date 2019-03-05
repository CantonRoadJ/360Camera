/*
 * main.cpp
 *
 *  Created on: 2016年9月5日
 *      Author: hh
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <getopt.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>
#include <linux/videodev2.h>
#include <pthread.h>

#include <errno.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>

#include "v4l2_work_fourin.h"
#include <QDebug>
#include <QTime>

//struct g2d_buf *g_g2d_test_buffer[NUMBER_BUFFERS];
//struct g2d_buf *g_g2d_src_buffer[NUMBER_BUFFERS];
//struct g2d_buf *g_g2d_dst_buffer[NUMBER_BUFFERS];

//////////////////////////////////////////////////////
CameraWorkFourin::CameraWorkFourin(QObject *parent)
{
    QTime time;

    time.start();


    for (int i = 0; i < g_capture_num_buffers; i++) {

            int in_buffer_size = NUMBER_BUFFERS * width * height * 4;
            int out_buffer_size = NUMBER_BUFFERS * width * height * 4;

                            g_g2d_test_buffer[i] = g2d_alloc(in_buffer_size, 0);
                            g_g2d_dst_buffer[i] =g2d_alloc(out_buffer_size, 0);

                            if (!g_g2d_test_buffer[i])
                            {
                                qDebug("can not malloc memory for g2d source\n") ;
                                return;
                            }

                            if (!g_g2d_dst_buffer[i])
                            {
                                qDebug("can not malloc memory for g2d source\n") ;
                                return;
                            }
                            memset(g_g2d_test_buffer[i]->buf_vaddr,0xff,in_buffer_size);

                            memset(g_g2d_dst_buffer[i]->buf_vaddr,0xff,out_buffer_size);
            }


    offsetCount = 0;
    initG2DBufferStatus = false;

#if 1
    memset(fourCamera.topCamera.dev_name, '\0', sizeof(fourCamera.topCamera.dev_name));

    for (int i=0;i<NUMBER_BUFFERS;i++)
    {
            fourCamera.topCamera.buf[i].start = NULL;
            fourCamera.topCamera.buf[i].offset = 0;
            fourCamera.topCamera.buf[i].length = 0;
    }

    fourCamera.topCamera.data = NULL;
    fourCamera.topCamera.fd = -1;
    fourCamera.topCamera.n_buffers = 0;
    setCameraName(&(fourCamera.topCamera),"/dev/video0");
    initTopCamera.setData(&(fourCamera.topCamera),topCameraWhich);
    initTopCamera.start();
#endif

#if 1
    memset(fourCamera.bottomCamera.dev_name, '\0', sizeof(fourCamera.bottomCamera.dev_name));

    for (int i=0;i<NUMBER_BUFFERS;i++)
    {
            fourCamera.bottomCamera.buf[i].start = NULL;
            fourCamera.bottomCamera.buf[i].offset = 0;
            fourCamera.bottomCamera.buf[i].length = 0;
    }
    fourCamera.bottomCamera.data = NULL;
    fourCamera.bottomCamera.fd = -1;
    fourCamera.bottomCamera.n_buffers = 0;
    setCameraName(&(fourCamera.bottomCamera),"/dev/video1");
    initBottomCamera.setData(&(fourCamera.bottomCamera),bottomCameraWhich);
    initBottomCamera.start();
#endif

#if 1
    memset(fourCamera.leftCamera.dev_name, '\0', sizeof(fourCamera.leftCamera.dev_name));

    for (int i=0;i<NUMBER_BUFFERS;i++)
    {
            fourCamera.leftCamera.buf[i].start = NULL;
            fourCamera.leftCamera.buf[i].offset = 0;
            fourCamera.leftCamera.buf[i].length = 0;
    }

    fourCamera.leftCamera.data = NULL;
    fourCamera.leftCamera.fd = -1;
    fourCamera.leftCamera.n_buffers = 0;
    setCameraName(&(fourCamera.leftCamera),"/dev/video2");
    initLeftCamera.setData(&(fourCamera.leftCamera),leftCameraWhich);
    initLeftCamera.start();
#endif

#if 1
    memset(fourCamera.rightCamera.dev_name, '\0', sizeof(fourCamera.rightCamera.dev_name));


    for (int i=0;i<NUMBER_BUFFERS;i++)
    {
            fourCamera.rightCamera.buf[i].start = NULL;
            fourCamera.rightCamera.buf[i].offset = 0;
            fourCamera.rightCamera.buf[i].length = 0;
    }

    fourCamera.rightCamera.data = NULL;
    fourCamera.rightCamera.fd = -1;
    fourCamera.rightCamera.n_buffers = 0;
    setCameraName(&(fourCamera.rightCamera),"/dev/video3");
    initRightCamera.setData(&(fourCamera.rightCamera),rightCameraWhich);
    initRightCamera.start();
#endif

    while(1)
    {
        if ((initTopCamera.isFinished()) && (initBottomCamera.isFinished()) && (initLeftCamera.isFinished()) && (initRightCamera.isFinished()))
        {
            break;
        }
    }

    qDebug()<<"CameraWorkFourin init finis is "<<time.elapsed()/1000.0<<"s";

    return;
}

void CameraWorkFourin::finishTopCameraWork()
{
    finishInitTop = true;
}

void CameraWorkFourin::finishBottomCameraWork()
{
    finishInitBottoom = true;
}

void CameraWorkFourin::finishLeftCameraWork()
{
    finishInitLeft = true;
}

void CameraWorkFourin::finishRightCameraWork()
{
    finishInitRight = true;
}

int CameraWorkFourin::video_init()
{
    return TPASS;
}

int CameraWorkFourin::v4l_capture_setup(CameraStruct *camera)
{

        struct v4l2_capability cap;
        struct v4l2_cropcap cropcap;
        struct v4l2_control ctl;
        struct v4l2_crop crop;
        struct v4l2_format fmt;
        struct v4l2_requestbuffers req;
    //struct v4l2_dbg_chip_ident chip;
        struct v4l2_streamparm parm;
        v4l2_std_id id;
        unsigned int min;

        QTime time;

        time.start();
        if (ioctl (camera->fd, VIDIOC_QUERYCAP, &cap) < 0) {
                if (EINVAL == errno) {
                        qDebug ("%s is no V4L2 device\n",
                                        camera->dev_name);
                        return TFAIL;
                } else {
                        qDebug( "%s isn not V4L device,unknow error\n",
                        camera->dev_name);
                        return TFAIL;
                }
        }
        qDebug()<<"VIDIOC_QUERYCAP"<<time.elapsed()/1000.0<<"s";

        time.start();
        if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
                qDebug ( "%s is no video capture device\n",
                        camera->dev_name);
                return TFAIL;
        }
        qDebug()<<"V4L2_CAP_VIDEO_CAPTURE"<<time.elapsed()/1000.0<<"s";

        time.start();
        if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
                qDebug ( "%s does not support streaming i/o\n",
                        camera->dev_name);
                return TFAIL;
        }
        qDebug()<<"V4L2_CAP_STREAMING"<<time.elapsed()/1000.0<<"s";

        time.start();
        if (ioctl(camera->fd, VIDIOC_S_INPUT, &g_input) < 0) {
                qDebug("VIDIOC_S_INPUT failed\n");
                close(camera->fd);
                return TFAIL;
        }
        qDebug()<<"VIDIOC_S_INPUT"<<time.elapsed()/1000.0<<"s";

        time.start();
        memset(&cropcap, 0, sizeof(cropcap));
        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (ioctl (camera->fd, VIDIOC_CROPCAP, &cropcap) < 0) {
                crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                crop.c = cropcap.defrect; /* reset to default */

                if (ioctl (camera->fd, VIDIOC_S_CROP, &crop) < 0) {
                        switch (errno) {
                                case EINVAL:
                                        /* Cropping not supported. */
                                        qDebug ( "%s  doesn't support crop\n",
                                                camera->dev_name);
                                        close(camera->fd);
                                        return TFAIL;
                                        break;
                                default:
                                        /* Errors ignored. */
                                        close(camera->fd);
                                        return TFAIL;
                                        break;
                        }
                }
        } else {
                /* Errors ignored. */
        }
        qDebug()<<"VIDIOC_CROPCAP"<<time.elapsed()/1000.0<<"s";

        time.start();
        parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        parm.parm.capture.timeperframe.numerator = 1;
        parm.parm.capture.timeperframe.denominator = 0;
        parm.parm.capture.capturemode = 0;
        if (ioctl(camera->fd, VIDIOC_S_PARM, &parm) < 0) {
                qDebug("VIDIOC_S_PARM failed\n");
                close(camera->fd);
                return TFAIL;
        }
        qDebug()<<"VIDIOC_S_PARM"<<time.elapsed()/1000.0<<"s";

        time.start();
        memset(&fmt, 0, sizeof(fmt));

        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width = 0;
        fmt.fmt.pix.height = 0;
        fmt.fmt.pix.pixelformat = g_in_fmt;
        fmt.fmt.pix.field = V4L2_FIELD_ANY;
        if (ioctl (camera->fd, VIDIOC_S_FMT, &fmt) < 0) {
                qDebug ( "%s iformat not supported \n",
                        camera->dev_name);
                return TFAIL;
        }
        qDebug()<<"VIDIOC_S_FMT"<<time.elapsed()/1000.0<<"s";

        /* Note VIDIOC_S_FMT may change width and height. */

        /* Buggy driver paranoia. */
        time.start();
        min = fmt.fmt.pix.width * 2;
        if (fmt.fmt.pix.bytesperline < min)
                fmt.fmt.pix.bytesperline = min;

        min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
        if (fmt.fmt.pix.sizeimage < min)
                fmt.fmt.pix.sizeimage = min;

        if (ioctl(camera->fd, VIDIOC_G_FMT, &fmt) < 0) {
                qDebug("VIDIOC_G_FMT failed\n");
                close(camera->fd);
                return TFAIL;
        }
        qDebug()<<"VIDIOC_G_FMT"<<time.elapsed()/1000.0<<"s";

        file_length = fmt.fmt.pix.sizeimage;
        g_in_width = fmt.fmt.pix.width;
        g_in_height = fmt.fmt.pix.height;
        //qDebug("g_in_width = %d, g_in_height = %d.\r\n", g_in_width, g_in_height);

        if (g_g2d_render) {
                switch (g_in_fmt) {
                        case V4L2_PIX_FMT_RGB565:
                                g_frame_size = g_in_width * g_in_height * 2;
                                g_g2d_fmt = G2D_RGB565;
                                break;

                        case V4L2_PIX_FMT_UYVY:
                                g_frame_size = g_in_width * g_in_height * 2;
                                g_g2d_fmt = G2D_UYVY;
                                break;

                        case V4L2_PIX_FMT_YUYV:
                                g_frame_size = g_in_width * g_in_height * 2;
                                g_g2d_fmt = G2D_YUYV;
                                break;

                        case V4L2_PIX_FMT_YUV420:
                                g_frame_size = g_in_width * g_in_height * 3 / 2;
                                g_g2d_fmt = G2D_I420;
                                break;

                        case V4L2_PIX_FMT_NV12:
                                g_frame_size = g_in_width * g_in_height * 3 / 2;
                                g_g2d_fmt = G2D_NV12;
                                break;

                        default:
                                qDebug("Unsupported format.\n");
                                return TFAIL;
                }
        }

        time.start();
        memset(&req, 0, sizeof (req));
        req.count = g_capture_num_buffers;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = (v4l2_memory)g_mem_type;
        if (ioctl (camera->fd, VIDIOC_REQBUFS, &req) < 0) {
                if (EINVAL == errno) {
                        qDebug ( "%s does not support "
                                         "memory mapping\n", camera->dev_name);
                        return TFAIL;
                } else {
                        qDebug ( "%s does not support "
                                         "memory mapping, unknow error\n", camera->dev_name);
                        return TFAIL;
                }
        }
        qDebug()<<"VIDIOC_REQBUFS"<<time.elapsed()/1000.0<<"s";

        if (req.count < 2) {
                qDebug ( "Insufficient buffer memory on %s\n",
                         camera->dev_name);
                return TFAIL;
        }

        return TPASS;
}

int CameraWorkFourin::start_capturing(CameraStruct *camera)
{
        unsigned int i;
        struct v4l2_buffer buf;
        enum v4l2_buf_type type;

        for (i = 0; i < g_capture_num_buffers; i++) {

//         camera->buf[i] = (struct testbuffer*) calloc(1, file_length); //内存中建立对应空间

//            if (camera->buf[i] == NULL)
//            {
//                qDebug("camera.buf is NULL\n");
//                close(camera->fd);
//                return TFAIL;
//            }

                memset(&buf, 0, sizeof (buf));
                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory =(v4l2_memory) g_mem_type;
                buf.index = i;
                if (g_mem_type == V4L2_MEMORY_USERPTR) {



                        buf.length = camera->buf[i].length;
                        buf.m.userptr = (unsigned long)camera->buf[i].offset;
                }
                if (ioctl(camera->fd, VIDIOC_QUERYBUF, &buf) < 0) {
                        qDebug("VIDIOC_QUERYBUF error\n");
                        return TFAIL;
                }

                if (g_mem_type == V4L2_MEMORY_MMAP) {
                        camera->buf[i].length = buf.length;
                        camera->buf[i].offset = (size_t) buf.m.offset;
                        camera->buf[i].start = (unsigned char *) mmap(NULL, camera->buf[i].length,
                            PROT_READ | PROT_WRITE, MAP_SHARED,
                            camera->fd, camera->buf[i].offset);
                        memset(camera->buf[i].start, 0xFF, camera->buf[i].length);
                }
        }

        for (i = 0; i < g_capture_num_buffers; i++) {
                memset(&buf, 0, sizeof (buf));
                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = (v4l2_memory)g_mem_type;
                buf.index = i;
                if (g_mem_type == V4L2_MEMORY_USERPTR)
                        buf.m.offset = (unsigned int) (camera->buf[i].start);
                else
                        buf.m.offset = camera->buf[i].offset;
                buf.length = camera->buf[i].length;
                if (ioctl(camera->fd, VIDIOC_QBUF, &buf) < 0) {
                        qDebug("VIDIOC_QBUF error\n");
                        return TFAIL;
                }
        }

        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (ioctl (camera->fd, VIDIOC_STREAMON, &type) < 0) {
                qDebug("VIDIOC_STREAMON error\n");
                return TFAIL;
        }
        return TPASS;
}

int CameraWorkFourin::prepare_g2d_buffers(CameraStruct *camera,int theCamera)
{

    for (int i = 0; i < g_capture_num_buffers; i++) {

            int in_buffer_size = NUMBER_BUFFERS * width * height * 4;
            int out_buffer_size = NUMBER_BUFFERS * width * height * 4;
                       if (initG2DBufferStatus == false)
                       {
                            g_g2d_test_buffer[i] = g2d_alloc(in_buffer_size, 0);
                            g_g2d_dst_buffer[i] =g2d_alloc(out_buffer_size, 0);

                            if (!g_g2d_test_buffer[i])
                            {
                                qDebug("can not malloc memory for g2d source\n") ;
                                return TFAIL;
                            }

                            if (!g_g2d_dst_buffer[i])
                            {
                                qDebug("can not malloc memory for g2d source\n") ;
                                return TFAIL;
                            }
                            memset(g_g2d_test_buffer[i]->buf_vaddr,0xff,in_buffer_size);

                            memset(g_g2d_dst_buffer[i]->buf_vaddr,0xff,out_buffer_size);
                       }

        camera->buf[i].start = ( ( (unsigned char *) (g_g2d_test_buffer[i]->buf_vaddr) )+(theCamera*g_frame_size));
        camera->buf[i].offset = ((g_g2d_test_buffer[i]->buf_paddr)+(theCamera*g_frame_size));
        camera->buf[i].length = g_frame_size;
    }
    initG2DBufferStatus= true;
    return 0;
}


//////////////////////////////////////////////////////
//获取一帧数据
//////////////////////////////////////////////////////
int CameraWorkFourin::read_frame(CameraStruct *camera,int theCamera)
{

}


//工作线程
int CameraWorkFourin::init(CameraStruct *camera,int theCamera)
{
     QTime time;

     time.start();
    if ((camera->fd = open(camera->dev_name, O_RDWR, 0)) < 0) {
    qDebug("Unable to open %s\n", camera->dev_name);
            return TFAIL;
    }
    qDebug()<<"open function"<<time.elapsed()/1000.0<<"s";

    time.start();
    if (v4l_capture_setup(camera) < 0) {
            qDebug("Setup v4l capture failed.\n");
            ::close(camera->fd);
            return TFAIL;
    }
    qDebug()<<"v4l_capture_setup function"<<time.elapsed()/1000.0<<"s";

#if 1
    time.start();
    if (prepare_g2d_buffers(camera,theCamera) < 0) {
        qDebug("prepare_g2d_buffers failed\n");
        ::close(camera->fd);
        return TFAIL;
    }
    qDebug()<<"prepare_g2d_buffers function"<<time.elapsed()/1000.0<<"s";
#endif

#if 1
    time.start();
    if (start_capturing(camera) < 0) {
            qDebug("start_capturing failed\n");
            ::close(camera->fd);
            return TFAIL;
    }
    qDebug()<<"start_capturing function"<<time.elapsed()/1000.0<<"s";
#endif

    return TPASS;
}

void CameraWorkFourin::setWhichCamera(int theCamera)
{
    this->theCamera = theCamera;
}

void CameraWorkFourin::run()
{
    while(1)
    {

//        QTime time;
//        time.start();
            if (cameraStatus.topCameraStatus == true)
            {
                if ( cameraRun(&(fourCamera.topCamera),topCameraWhich) != TPASS)
                {
                    cameraStatus.topCameraStatus = false;
                    qDebug()<<"cameraRun topCameraWhich is fail";
                }
            }
            if (cameraStatus.bottomCameraStatus == true)
            {
                if ( cameraRun(&(fourCamera.bottomCamera),bottomCameraWhich) != TPASS)
                {
                    cameraStatus.bottomCameraStatus = false;
                    qDebug()<<"cameraRun bottomCameraWhich is fail";
                }
            }

            if (cameraStatus.leftCameraStatus ==true)
            {
                if (cameraRun(&(fourCamera.leftCamera),leftCameraWhich) != TPASS)
                {
                    cameraStatus.leftCameraStatus = false;
                    qDebug()<<"cameraRun leftCameraWhich is fail";
                }
            }

            if (cameraStatus.rightCameraStatus ==true)
            {
                if (cameraRun(&(fourCamera.rightCamera),rightCameraWhich) != TPASS)
                {
                    cameraStatus.rightCameraStatus = false;
                    qDebug()<<"cameraRun rightCameraWhich is fail";
                }
            }
                            if (YUYV2BGRA(g_g2d_test_buffer[offsetCount],width,height *countCamera,g_g2d_dst_buffer[offsetCount],width,height*countCamera) == TPASS)
                            {
                                emit Frame(offsetCount);
                            }
							else
							{
                                qDebug()<<"YUYV2BGRA is fail";
							}

                            offsetCount++;

                            if (offsetCount % count == 0)
                            {
                                offsetCount = 0;
                            }
    }
}

int CameraWorkFourin::cameraRun(CameraStruct *camera,int theCamera)
{
    struct v4l2_buffer capture_buf;

    for (int i = 0; i < g_frame_count; i++) {
            memset(&capture_buf, 0, sizeof(capture_buf));
            capture_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            capture_buf.memory = (v4l2_memory)g_mem_type;

            if (ioctl(camera->fd, VIDIOC_DQBUF, &capture_buf) < 0) {
                    qDebug("VIDIOC_DQBUF failed.\n");
                    return TFAIL;
            }

            if (ioctl(camera->fd, VIDIOC_QBUF, &capture_buf) < 0) {
                    qDebug("VIDIOC_QBUF failed\n");
                    return TFAIL;
            }
    }
    return TPASS;
}

void CameraWorkFourin::uninit(CameraStruct *camera,int theCamera)
{
    enum v4l2_buf_type type;

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(camera->fd, VIDIOC_STREAMOFF, &type);
    close(camera->fd);

}

void CameraWorkFourin::setCameraName(CameraStruct *camera,QString name)
{
//    this->cameraName = name;
    memcpy(&(camera->dev_name),name.toLatin1().data(),sizeof(camera->dev_name));
}

unsigned char * CameraWorkFourin::setCameraData(unsigned char **data)
//unsigned char * CameraWorkFourin::setCameraData(unsigned char *data)
{
    *data = new unsigned char [count *countCamera * 2 * width *height];
    startData = *data;
    return *data;
}
void CameraWorkFourin::CloseCamera()
{
uninit(&(fourCamera.topCamera),topCameraWhich);
uninit(&(fourCamera.bottomCamera),bottomCameraWhich);
uninit(&(fourCamera.leftCamera),leftCameraWhich);
uninit(&(fourCamera.rightCamera),rightCameraWhich);
}


CameraWorkFourin::~CameraWorkFourin()
{
    qDebug("CameraWorkFourin::~CameraWorkFourin() function");

    uninit(&(fourCamera.topCamera),topCameraWhich);
    uninit(&(fourCamera.bottomCamera),bottomCameraWhich);
    uninit(&(fourCamera.leftCamera),leftCameraWhich);
    uninit(&(fourCamera.rightCamera),rightCameraWhich);

    if (g_g2d_render) {
        for (int i = 0; i < g_capture_num_buffers; i++) {
            g2d_free(g_g2d_test_buffer[i]);
            g2d_free(g_g2d_dst_buffer[i]);
        }
    }
}
