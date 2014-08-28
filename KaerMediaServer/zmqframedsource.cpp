#include "zmqframedsource.h"


ZmqFramedSource *ZmqFramedSource::createNew(UsageEnvironment &env)
{
    ZmqFramedSource *source =  new ZmqFramedSource(env);
    source->connect("tcp://localhost:5556","");
    return source;
}

void ZmqFramedSource::doGetNextFrame()
{
//    if ((fLimitNumBytesToStream && fNumBytesToStream == 0)) {
//        handleClosure(this);
//        return;
//    }

    fFrameSize = fMaxSize;

    if(fCurIndex >= fBufferSize){
        zmq::message_t msg;

        subscriber->recv(&msg);

        if(msg.size() < totalBufferSize){
            memcpy(fBuffer,msg.data(),msg.size());
            fBufferSize = msg.size();
            fCurIndex = 0;
        }else{
            envir() << "receive msg "<<(int)msg.size()<<
                       " is more than max buffer size "<<(int)totalBufferSize ;

        }
    }

    int leftBufferSize = fBufferSize - fCurIndex;

    if(leftBufferSize < fMaxSize){
        fFrameSize = leftBufferSize;
    }else{
        envir() << "leftBufferSize  "<<leftBufferSize<<" is more than fMaxSize "<<fMaxSize ;
    }

    memmove(fTo, &fBuffer[fCurIndex], fFrameSize);
    fCurIndex += fFrameSize;

    if (fFrameSize == 0) {
        envir() << "fFrameSize == 0 "<<fMaxSize ;
        handleClosure(this);
        return;
    }

    // Set the 'presentation time':
    // We don't know a specific play time duration for this data,
    // so just record the current time as being the 'presentation time':
    gettimeofday(&fPresentationTime, NULL);


    nextTask() = envir().taskScheduler().scheduleDelayedTask(0,(TaskFunc*)FramedSource::afterGetting, this);
    // Inform the downstream object that it has data:

}

ZmqFramedSource::ZmqFramedSource(UsageEnvironment &env):
    FramedSource(env),totalBufferSize(1024*1024)

{
    context = new zmq::context_t();
    subscriber = new zmq::socket_t(*context,ZMQ_SUB);
    fBuffer = new u_int8_t[totalBufferSize];
    fBufferSize = 0;
    fCurIndex = 0;
}

ZmqFramedSource::~ZmqFramedSource()
{
    delete subscriber;
    delete context;
    delete [] fBuffer;
}

bool ZmqFramedSource::connect(std::string url, std::string filter)
{
    envir()<<"ZmqFramedSource::connect";
    subscriber->connect("tcp://localhost:5556");
    //  Subscribe to zipcode, default is NYC, 10001
    subscriber->setsockopt(ZMQ_SUBSCRIBE, filter.c_str(), filter.length());
}
