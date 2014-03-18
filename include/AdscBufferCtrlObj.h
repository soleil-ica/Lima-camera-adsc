#ifndef ADSCBUFFERCTRLOBJ_H
#define ADSCBUFFERCTRLOBJ_H

#include "Debug.h"
#include "AdscCompatibility.h"
#include "HwBufferCtrlObj.h"
#include "HwBufferMgr.h"
//#include "AdscCamera.h"
//#include "AdscReader.h"

namespace lima {
namespace Adsc {

class Camera;
class Reader;

//*******************************************************************
// \class BufferCtrlObj
// \brief Control object providing Marccd buffering interface
//*******************************************************************
class LIBADSC_API BufferCtrlObj : public HwBufferCtrlObj
{
	DEB_CLASS_NAMESPC(DebModCamera, "BufferCtrlObj", "Adsc");

public:
	BufferCtrlObj(Camera& simu);
	virtual ~BufferCtrlObj();

	virtual void setFrameDim(const FrameDim& frame_dim);
	virtual void getFrameDim(FrameDim& frame_dim);

	virtual void setNbBuffers(int nb_buffers);
	virtual void getNbBuffers(int& nb_buffers);

	virtual void setNbConcatFrames(int nb_concat_frames);
	virtual void getNbConcatFrames(int& nb_concat_frames);

	virtual void getMaxNbBuffers(int& max_nb_buffers);

	virtual void *getBufferPtr(int buffer_nb, int concat_frame_nb = 0);
	virtual void *getFramePtr(int acq_frame_nb);

	virtual void getStartTimestamp(Timestamp& start_ts);
	virtual void getFrameInfo(int acq_frame_nb, HwFrameInfoType& info);

	// -- Buffer control object
	BufferCtrlMgr& getBufferMgr()
	{
		return m_buffer_ctrl_mgr;
	}

	StdBufferCbMgr& getBufferCbMgr()
	{
		return m_buffer_cb_mgr;
	}


	virtual void registerFrameCallback(HwFrameCallback& frame_cb);
	virtual void unregisterFrameCallback(HwFrameCallback& frame_cb);

	//Reader stuff
	void start();
	void stop();
	void reset();
	int  getLastAcquiredFrame(void);
	bool isTimeoutSignaled(void);
	bool isRunning(void);
	void setTimeout(int TO);
	void enableReader(void);
	void disableReader(void);
private:
	SoftBufferAllocMgr m_buffer_alloc_mgr;
	StdBufferCbMgr m_buffer_cb_mgr;
	BufferCtrlMgr m_buffer_ctrl_mgr;
	Camera& m_cam;
	Reader* m_reader;
};

} // namespace Adsc
} // namespace lima

#endif // ADSCBUFFERCTRLOBJ_H
