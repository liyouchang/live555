#include "zmqframedsource.h"

#include "GroupsockHelper.hh"

ZmqFramedSource *ZmqFramedSource::createNew(UsageEnvironment &env,const char * streamID)
{
    ZmqFramedSource *source =  new ZmqFramedSource(env);
    //    envir()<<"connect ";
    source->connect("tcp://192.168.40.195:5556",streamID);
    //    envir()<<"connect end ";
    return source;
}

void ZmqFramedSource::doGetNextFrame()
{

    //    if ((fLimitNumBytesToStream && fNumBytesToStream == 0)) {
    //        handleClosure(this);
    //        return;
    //    }
    if(!subscriber->connected()){
        envir() << "disconnected  "<<" no "<<tt<<"\n";

        handleClosure(this);
        return;
    }

    fFrameSize = fMaxSize;

    if(fCurIndex >= fBufferSize){
        zmq::message_t msgevp;
        subscriber->recv(&msgevp);
        zmq::message_t msgData;
        subscriber->recv(&msgData);

        if(msgData.size() < totalBufferSize){
            memcpy(fBuffer,msgData.data(),msgData.size());
            fBufferSize = msgData.size();
            fCurIndex = 0;
        }else{
            envir() << "receive msg "<<(int)msgData.size()<<
                       " is more than max buffer size "<<(int)totalBufferSize ;
        }
    }
    int leftBufferSize = fBufferSize - fCurIndex;
    if(leftBufferSize < fMaxSize){
        fFrameSize = leftBufferSize;
        if(next){
            envir() << "leftBufferSize  "<< leftBufferSize <<
                       "  fMaxSize "<<fMaxSize << " no "<<tt<<"\n";
            next = false;
        }
    }else{
        next =true;
    }

    envir() << "large leftBufferSize  "<< leftBufferSize <<"  fMaxSize "<<fMaxSize << " no "<<tt<<"\n";

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

    //    nextTask() = envir().taskScheduler().scheduleDelayedTask(0,(TaskFunc*)FramedSource::afterGetting, this);
    // Inform the downstream object that it has data:
    FramedSource::afterGetting(this);
}

static zmq::context_t context;
static int test = 0;
ZmqFramedSource::ZmqFramedSource(UsageEnvironment &env):
    FramedSource(env),totalBufferSize(1024*1024)

{

    subscriber = new zmq::socket_t(context,ZMQ_SUB);
    fBuffer = new u_int8_t[totalBufferSize];
    fBufferSize = 0;
    fCurIndex = 0;
    tt = test++;
    envir()<<"ZmqFramedSource create "<<tt<<"\n";

}

ZmqFramedSource::~ZmqFramedSource()
{

    envir()<<"ZmqFramedSource destroy "<<tt<<"\n";

    delete subscriber;
    delete [] fBuffer;
}

bool ZmqFramedSource::connect(std::string url, std::string filter)
{
    envir()<<"connect "<<filter.c_str()<<"  no:"<<tt<<" url "<< url.c_str()<<"\n";

    subscriber->connect(url.c_str());
    //  Subscribe to zipcode, default is NYC, 10001
    subscriber->setsockopt(ZMQ_SUBSCRIBE, filter.c_str(), filter.length());
    return true;
}
