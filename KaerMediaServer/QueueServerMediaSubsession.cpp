#include "QueueServerMediaSubsession.h"
#include "H264VideoRTPSink.hh"

#include "H264VideoStreamFramer.hh"
QueueServerMediaSubsession *QueueServerMediaSubsession::createNew(
        UsageEnvironment &env, Boolean reuseFirstSource,const char * streamID)
{
    return new QueueServerMediaSubsession(env,reuseFirstSource,streamID);
}

QueueServerMediaSubsession::QueueServerMediaSubsession(
        UsageEnvironment &env, Boolean reuseFirstSource,const char * streamID)
    :OnDemandServerMediaSubsession(env, reuseFirstSource),
      fAuxSDPLine(NULL),fDoneFlag(0),fDummyRTPSink(NULL),streamID(streamID)
{
}

void QueueServerMediaSubsession::setDoneFlag() {
    fDoneFlag = ~0;
}

QueueServerMediaSubsession::~QueueServerMediaSubsession()
{
    delete[] fAuxSDPLine;
}
static void checkForAuxSDPLine(void* clientData) {
    QueueServerMediaSubsession* subsess = (QueueServerMediaSubsession*)clientData;
    subsess->checkForAuxSDPLine1();
}
void QueueServerMediaSubsession::checkForAuxSDPLine1()
{
    char const* dasl;

    if (fAuxSDPLine != NULL) {
        // Signal the event loop that we're done:
        setDoneFlag();
    } else if (fDummyRTPSink != NULL && (dasl = fDummyRTPSink->auxSDPLine()) != NULL) {
        fAuxSDPLine = strDup(dasl);
        fDummyRTPSink = NULL;
        // Signal the event loop that we're done:
        setDoneFlag();
    } else {
        // try again after a brief delay:
        int uSecsToDelay = 100000; // 100 ms
        nextTask() = envir().taskScheduler().scheduleDelayedTask(
                    uSecsToDelay,(TaskFunc*)checkForAuxSDPLine, this);
    }
}

static void afterPlayingDummy(void* clientData) {
    QueueServerMediaSubsession* subsess = (QueueServerMediaSubsession*)clientData;
    subsess->afterPlayingDummy1();
}
void QueueServerMediaSubsession::afterPlayingDummy1()
{
    // Unschedule any pending 'checking' task:
    envir().taskScheduler().unscheduleDelayedTask(nextTask());
    // Signal the event loop that we're done:
    setDoneFlag();
}

FramedSource *QueueServerMediaSubsession::createNewStreamSource(
        unsigned clientSessionId, unsigned &estBitrate)
{
    estBitrate = 500; // kbps, estimate

    // Create the video source:
    //  if(fQueueInputSource == NULL)
    ZmqFramedSource * inputSource = ZmqFramedSource::createNew(envir(),streamID.c_str());
    if (inputSource == NULL) return NULL;
    // Create a framer for the Video Elementary Stream:
    return H264VideoStreamFramer::createNew(envir(), inputSource);
}

RTPSink *QueueServerMediaSubsession::createNewRTPSink(
        Groupsock *rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic,
        FramedSource *inputSource)
{
    return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
}

const char *QueueServerMediaSubsession::getAuxSDPLine(
        RTPSink *rtpSink, FramedSource *inputSource)
{
    if (fAuxSDPLine != NULL) return fAuxSDPLine; // it's already been set up (for a previous client)

    if (fDummyRTPSink == NULL) { // we're not already setting it up for another, concurrent stream
        // Note: For H264 video files, the 'config' information ("profile-level-id" and "sprop-parameter-sets") isn't known
        // until we start reading the file.  This means that "rtpSink"s "auxSDPLine()" will be NULL initially,
        // and we need to start reading data from our file until this changes.
        fDummyRTPSink = rtpSink;

        // Start reading the file:
        fDummyRTPSink->startPlaying(*inputSource, afterPlayingDummy, this);

        // Check whether the sink's 'auxSDPLine()' is ready:
        checkForAuxSDPLine(this);
    }

    envir().taskScheduler().doEventLoop(&fDoneFlag);

    return fAuxSDPLine;
}
