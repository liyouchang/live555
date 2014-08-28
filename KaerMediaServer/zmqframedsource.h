#ifndef ZMQFRAMEDSOURCE_H
#define ZMQFRAMEDSOURCE_H

#include "FramedSource.hh"

#include "zmq.h"
#include "zmq.hpp"
#include "zhelpers.hpp"
#include "zmsg.hpp"


class ZmqFramedSource : public FramedSource
{
public:
    static ZmqFramedSource* createNew(UsageEnvironment& env);

    void doGetNextFrame();

protected:
    ZmqFramedSource(UsageEnvironment& env);
    virtual ~ZmqFramedSource();

    bool connect(std::string url,std::string filter);

private:
    zmq::context_t *context;
    zmq::socket_t *subscriber;
//    bool isConnected;

    u_int8_t* fBuffer;
    u_int64_t fBufferSize;
    u_int64_t fCurIndex;
    const int totalBufferSize;
//    unsigned fLastPlayTime;
//    Boolean fLimitNumBytesToStream;
//    u_int64_t fNumBytesToStream; // used iff "fLimitNumBytesToStream" is True

};

#endif // ZMQFRAMEDSOURCE_H
