#ifndef BYTESTREAMQUEUEBUFFERSOURCE_H
#define BYTESTREAMQUEUEBUFFERSOURCE_H

#include "FramedSource.hh"
//#include <boost/lockfree/spsc_queue.hpp>
//#include <QTcpSocket>


class ByteStreamQueueBufferSource : public FramedSource
{
public:
  static ByteStreamQueueBufferSource* createNew(UsageEnvironment& env,
                         unsigned preferredFrameSize = 0,
                         unsigned playTimePerFrame = 0);
      // "preferredFrameSize" == 0 means 'no preference'
      // "playTimePerFrame" is in microseconds

  bool pushStream(const byte * data,size_t size);
  int getSocketStream(unsigned char * data,size_t size);
protected:
    ByteStreamQueueBufferSource(UsageEnvironment& env,unsigned preferredFrameSize,
                                unsigned playTimePerFrame);
    virtual ~ByteStreamQueueBufferSource();

private:
    // redefined virtual functions:
    virtual void doGetNextFrame();


private:
    bool isConnected;
    QTcpSocket *tcpClient;
  //  boost::lockfree::spsc_queue<byte, boost::lockfree::capacity<1024*1024*2> > spsc_queue;
    unsigned fPreferredFrameSize;
    unsigned fPlayTimePerFrame;

    unsigned fLastPlayTime;

    Boolean fLimitNumBytesToStream;
    u_int64_t fNumBytesToStream; // used iff "fLimitNumBytesToStream" is True

};

#endif // BYTESTREAMQUEUEBUFFERSOURCE_H
