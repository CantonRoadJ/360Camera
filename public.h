#ifndef PUBLIC_H
#define PUBLIC_H

#define BUILD_FOR_ANDROID 1

#define G2D_CACHEABLE    0

#define TFAIL -1
#define TPASS 0

#define NUMBER_BUFFERS    4
#define NUMBER_CAMERAS  4

#define CLEAR(x) memset (&(x), 0, sizeof (x))

#include <sys/types.h>
#include "g2d.h"

#define topCameraWhich 0
#define bottomCameraWhich 1
#define leftCameraWhich 2
#define rightCameraWhich 3

struct buffer {
    void * start;
    size_t length;
};

struct testbuffer
{
        unsigned char *start;
        size_t offset;
        unsigned int length;
};

typedef struct __CameraStruct
{
    char dev_name[20];
    int fd;
    struct testbuffer  buf[4];
    unsigned int n_buffers;
    unsigned char * data;
}CameraStruct,* PCameraStruct;

typedef struct __FourCameraStruct
{
    CameraStruct topCamera;
    CameraStruct bottomCamera;
    CameraStruct leftCamera;
    CameraStruct rightCamera;
}FourCameraStruct, *PFourCameraStruct;

extern FourCameraStruct fourCamera;
extern struct g2d_buf *g_g2d_dst_buffer[4];
extern struct g2d_buf *g_g2d_test_buffer[4];
#endif // PUBLIC_H
