#ifndef QUEUESERVERMEDIASUBSESSION_H
#define QUEUESERVERMEDIASUBSESSION_H


#include "OnDemandServerMediaSubsession.hh"
#include "zmqframedsource.h"


class QueueServerMediaSubsession : public OnDemandServerMediaSubsession
{
public:
    static QueueServerMediaSubsession* createNew(
            UsageEnvironment& env, Boolean reuseFirstSource,const char * streamID);
    virtual ~QueueServerMediaSubsession();
    void checkForAuxSDPLine1();
    void afterPlayingDummy1();
protected:
    QueueServerMediaSubsession(UsageEnvironment& env, Boolean reuseFirstSource,const char * streamID);


    void setDoneFlag();

protected: // new virtual functions, defined by all subclasses
  virtual FramedSource* createNewStreamSource(unsigned clientSessionId,
                          unsigned& estBitrate);
      // "estBitrate" is the stream's estimated bitrate, in kbps
  virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock,
                    unsigned char rtpPayloadTypeIfDynamic,
                    FramedSource* inputSource);
    virtual char const* getAuxSDPLine(RTPSink* rtpSink,FramedSource* inputSource);
public:
    std::string streamID;
    char* fAuxSDPLine;
    char fDoneFlag; // used when setting up "fAuxSDPLine"
    RTPSink* fDummyRTPSink; // ditto

};

#endif // QUEUESERVERMEDIASUBSESSION_H
