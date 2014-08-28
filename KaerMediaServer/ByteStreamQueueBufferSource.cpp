#include "ByteStreamQueueBufferSource.h"
#include "GroupsockHelper.hh"

//#include <QDebug>
//#include<QAbstractSocket>

ByteStreamQueueBufferSource *ByteStreamQueueBufferSource::createNew(UsageEnvironment &env, unsigned preferredFrameSize, unsigned playTimePerFrame)
{
    qDebug()<<"create ByteStreamQueueBufferSource";
    return new ByteStreamQueueBufferSource(env, preferredFrameSize, playTimePerFrame);
}

bool ByteStreamQueueBufferSource::pushStream(const byte *data, size_t size)
{
   // size_t inSize = this->spsc_queue.push(data,size);
   // return (inSize == size);
    return true;
}

int ByteStreamQueueBufferSource::getSocketStream(unsigned char *data, size_t size)
{
    if(!isConnected){
        tcpClient->connectToHost("127.0.0.1",6666);
        isConnected =  tcpClient->waitForConnected();
        if(!isConnected){
            qDebug()<<"connect source error!";
            return 0;
        }
    }

    if(tcpClient->waitForReadyRead())
    {
        return tcpClient->read((char *)data,size);
    }
    else
    {
        qDebug()<<"live555 read from source no read!";
    }
    return 0;
}


ByteStreamQueueBufferSource::ByteStreamQueueBufferSource(
        UsageEnvironment &env, unsigned preferredFrameSize, unsigned playTimePerFrame)
    :FramedSource(env),fPreferredFrameSize(preferredFrameSize),
      fPlayTimePerFrame(playTimePerFrame),fLastPlayTime(0),isConnected(false)
{
    tcpClient = new QTcpSocket();
}

ByteStreamQueueBufferSource::~ByteStreamQueueBufferSource()
{
    tcpClient->close();
    delete tcpClient;
}

void ByteStreamQueueBufferSource::doGetNextFrame()
{
    if ((fLimitNumBytesToStream && fNumBytesToStream == 0)) {
        handleClosure(this);
        return;
    }

    // Try to read as many bytes as will fit in the buffer provided (or "fPreferredFrameSize" if less)
    fFrameSize = fMaxSize;
    if (fLimitNumBytesToStream && fNumBytesToStream < (u_int64_t)fFrameSize) {
        fFrameSize = (unsigned)fNumBytesToStream;
    }
    if (fPreferredFrameSize > 0 && fPreferredFrameSize < fFrameSize) {
        fFrameSize = fPreferredFrameSize;
    }
    //读取数据
    fFrameSize = getSocketStream(fTo,fFrameSize);

    if (fFrameSize == 0) {
        handleClosure(this);
        return;
    }
    fNumBytesToStream -= fFrameSize;

    // Set the 'presentation time':
    if (fPlayTimePerFrame > 0 && fPreferredFrameSize > 0) {
        if (fPresentationTime.tv_sec == 0 && fPresentationTime.tv_usec == 0) {
            // This is the first frame, so use the current time:
            gettimeofday(&fPresentationTime, NULL);
        } else {
            // Increment by the play time of the previous data:
            unsigned uSeconds	= fPresentationTime.tv_usec + fLastPlayTime;
            fPresentationTime.tv_sec += uSeconds/1000000;
            fPresentationTime.tv_usec = uSeconds%1000000;
        }

        // Remember the play time of this data:
        fLastPlayTime = (fPlayTimePerFrame*fFrameSize)/fPreferredFrameSize;
        fDurationInMicroseconds = fLastPlayTime;
    } else {
        // We don't know a specific play time duration for this data,
        // so just record the current time as being the 'presentation time':
        gettimeofday(&fPresentationTime, NULL);
    }

    nextTask() = envir().taskScheduler().scheduleDelayedTask(0,(TaskFunc*)FramedSource::afterGetting, this);
    // Inform the downstream object that it has data:

}
