#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H

#include <QString>

#define DANMAKU_WS_PING_INTERVAL 1000

//#define DANMAKU_DEBUG

#ifndef DANMAKU_DEBUG

    #define DANMAKU_DOMAIN QString("danmakuit.panda2134.site")
    #define DANMAKU_SCHEMA QString("wss")

#else

    #define DANMAKU_DOMAIN QString("http://101.42.99.123:8000")
    #define DANMAKU_SCHEMA QString("ws")

#endif

#endif // NETWORKCONFIG_H
