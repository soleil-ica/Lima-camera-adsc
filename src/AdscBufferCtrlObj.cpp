#include "AdscBufferCtrlObj.h"
#include "AdscReader.h"

using namespace lima::Adsc;


/*******************************************************************
 * \brief BufferCtrlObj constructor
 *******************************************************************/

BufferCtrlObj::BufferCtrlObj(Camera& cam) :
		m_buffer_cb_mgr(m_buffer_alloc_mgr),
		m_buffer_ctrl_mgr(m_buffer_cb_mgr),
		m_cam(cam)
{
	DEB_CONSTRUCTOR();
	m_reader = new Reader(cam, *this);
	m_reader->go(2000);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
BufferCtrlObj::~BufferCtrlObj()
{
	DEB_DESTRUCTOR();
	m_reader->reset();
	m_reader->exit();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::setFrameDim(const FrameDim& frame_dim)
{
	DEB_MEMBER_FUNCT();
	m_buffer_ctrl_mgr.setFrameDim(frame_dim);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::getFrameDim(FrameDim& frame_dim)
{
	DEB_MEMBER_FUNCT();
	m_buffer_ctrl_mgr.getFrameDim(frame_dim);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::setNbBuffers(int nb_buffers)
{
	DEB_MEMBER_FUNCT();
	m_buffer_ctrl_mgr.setNbBuffers(nb_buffers);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::getNbBuffers(int& nb_buffers)
{
	DEB_MEMBER_FUNCT();
	m_buffer_ctrl_mgr.getNbBuffers(nb_buffers);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::setNbConcatFrames(int nb_concat_frames)
{
	DEB_MEMBER_FUNCT();
	m_buffer_ctrl_mgr.setNbConcatFrames(nb_concat_frames);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::getNbConcatFrames(int& nb_concat_frames)
{
	DEB_MEMBER_FUNCT();
	m_buffer_ctrl_mgr.getNbConcatFrames(nb_concat_frames);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::getMaxNbBuffers(int& max_nb_buffers)
{
	DEB_MEMBER_FUNCT();
	m_buffer_ctrl_mgr.getMaxNbBuffers(max_nb_buffers);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void *BufferCtrlObj::getBufferPtr(int buffer_nb, int concat_frame_nb)
{
	DEB_MEMBER_FUNCT();
	return m_buffer_ctrl_mgr.getBufferPtr(buffer_nb, concat_frame_nb);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void *BufferCtrlObj::getFramePtr(int acq_frame_nb)
{
	DEB_MEMBER_FUNCT();
	return m_buffer_ctrl_mgr.getFramePtr(acq_frame_nb);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::getStartTimestamp(Timestamp& start_ts)
{
	DEB_MEMBER_FUNCT();
	m_buffer_ctrl_mgr.getStartTimestamp(start_ts);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::getFrameInfo(int acq_frame_nb, HwFrameInfoType& info)
{
	DEB_MEMBER_FUNCT();
	m_buffer_ctrl_mgr.getFrameInfo(acq_frame_nb, info);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::registerFrameCallback(HwFrameCallback& frame_cb)
{
	DEB_MEMBER_FUNCT();
	m_buffer_ctrl_mgr.registerFrameCallback(frame_cb);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::unregisterFrameCallback(HwFrameCallback& frame_cb)
{
	DEB_MEMBER_FUNCT();
	m_buffer_ctrl_mgr.unregisterFrameCallback(frame_cb);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::start()
{
	DEB_MEMBER_FUNCT();

}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::stop()
{
	DEB_MEMBER_FUNCT();
	m_reader->start();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::reset()
{
	DEB_MEMBER_FUNCT();
	m_reader->reset();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
int BufferCtrlObj::getLastAcquiredFrame()
{
    return m_reader->getLastAcquiredFrame();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool BufferCtrlObj::isTimeoutSignaled()
{
    return m_reader->isTimeoutSignaled();
}


//-----------------------------------------------------
//
//-----------------------------------------------------
bool BufferCtrlObj::isRunning()
{
    return m_reader->isRunning();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::setTimeout(int TO)
{
    DEB_MEMBER_FUNCT();
    m_reader->setTimeout(TO);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::enableReader(void)
{
    DEB_MEMBER_FUNCT();
	m_reader->enableReader();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BufferCtrlObj::disableReader(void)
{
    DEB_MEMBER_FUNCT();
	m_reader->disableReader();
}
