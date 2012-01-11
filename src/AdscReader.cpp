#include <yat/threading/Mutex.h>
#include <sstream>
#include <iostream>
#include <string>
#include <math.h>
#include "Debug.h"
#include "Data.h"
#include "AdscReader.h"
#include "AdscInterface.h"

//---------------------------
//- Ctor
//---------------------------
Reader::Reader(Camera& cam, HwBufferCtrlObj& buffer_ctrl)
		: m_cam(cam), m_buffer(buffer_ctrl)
{
	DEB_CONSTRUCTOR();
	try
	{
		m_is_reader_open_image_file = true;
		m_is_running = false;
		m_is_stop_done = true;
		m_image_number = 0;
		m_is_timeout_signaled = false;
		enable_timeout_msg(false);
		enable_periodic_msg(false);
		set_periodic_msg_period(kTASK_PERIODIC_TIMEOUT_MS);
		m_cam.getMaxImageSize(m_image_size);
		m_image = new uint16_t[m_image_size.getWidth() * m_image_size.getHeight()];
		memset((uint16_t*) m_image, 0, m_image_size.getWidth() * m_image_size.getHeight() * 2);
	}
	catch (Exception &e)
	{
		// Error handling
		DEB_ERROR() << e.getErrMsg();
		throw LIMA_HW_EXC(Error, e.getErrMsg());
	}
}

//---------------------------
//- Dtor
//---------------------------
Reader::~Reader()
{
	DEB_DESTRUCTOR();
	try
	{
		if (m_image != 0)
		{
			delete m_image;
			m_image = 0;
		}
	}
	catch (Exception &e)
	{
		// Error handling
		DEB_ERROR() << e.getErrMsg();
		throw LIMA_HW_EXC(Error, e.getErrMsg());
	}
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Reader::setTimeout(int TO)
{
    DEB_MEMBER_FUNCT();
    DEB_PARAM() << DEB_VAR1(TO);
    m_timeout = TO;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Reader::enableReader(void)
{
    DEB_MEMBER_FUNCT();
	m_is_reader_open_image_file = true;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Reader::disableReader(void)
{
    DEB_MEMBER_FUNCT();
	m_is_reader_open_image_file = false;
}

//---------------------------
//- Reader::start()
//---------------------------
void Reader::start()
{
	DEB_MEMBER_FUNCT();
	try
	{
		this->post(new yat::Message(ADSC_START_MSG), kPOST_MSG_TMO);
	}
	catch (yat::Exception& ex)
	{
		DEB_ERROR() << "Error : " << ex.errors[0].desc;
		throw LIMA_HW_EXC(Error, ex.errors[0].desc);
	}
}

//---------------------------
//- Reader::stop()
//---------------------------
void Reader::stop()
{
	DEB_MEMBER_FUNCT();
	try
	{
		this->post(new yat::Message(ADSC_STOP_MSG), kPOST_MSG_TMO);
	}
	catch (yat::Exception& ex)
	{
		DEB_ERROR() << "Error : " << ex.errors[0].desc;
		throw LIMA_HW_EXC(Error, ex.errors[0].desc);
	}
}

//---------------------------
//- Reader::reset()
//---------------------------
void Reader::reset()
{
	DEB_MEMBER_FUNCT();
	try
	{
		this->post(new yat::Message(ADSC_RESET_MSG), kPOST_MSG_TMO);
	}
	catch (yat::Exception& ex)
	{
		DEB_ERROR() << "Error : " << ex.errors[0].desc;
		throw LIMA_HW_EXC(Error, ex.errors[0].desc);
	}
}

//---------------------------
//- Reader::getLastAcquiredFrame()
//---------------------------
int Reader::getLastAcquiredFrame(void)
{
	DEB_MEMBER_FUNCT();
	yat::MutexLock scoped_lock(m_lock);
	return m_image_number;
}

//---------------------------
//- Reader::isTimeoutSignaled()
//---------------------------
bool Reader::isTimeoutSignaled()
{
	DEB_MEMBER_FUNCT();
	yat::MutexLock scoped_lock(m_lock);
	return m_is_timeout_signaled;
}


//---------------------------
//- Reader::isRunning()
//---------------------------
bool Reader::isRunning(void)
{
	DEB_MEMBER_FUNCT();
	yat::MutexLock scoped_lock(m_lock);
	return m_is_running;
}

//---------------------------
//
//---------------------------
void Reader::handle_message(yat::Message& msg) throw (yat::Exception)
{
	DEB_MEMBER_FUNCT();
	try
	{
		switch (msg.type())
		{
			//-----------------------------------------------------
			case yat::TASK_INIT:
			{
				DEB_TRACE() << "Reader::->TASK_INIT";
			}
			break;
				//-----------------------------------------------------
			case yat::TASK_EXIT:
			{
				DEB_TRACE() << "Reader::->TASK_EXIT";
			}
			break;
				//-----------------------------------------------------
			case yat::TASK_TIMEOUT:
			{
				DEB_TRACE() << "Reader::->TASK_TIMEOUT";
			}
			break;
				//-----------------------------------------------------
			case yat::TASK_PERIODIC:
			{
				DEB_TRACE() << "Reader::->TASK_PERIODIC";
			}
			break;
				//-----------------------------------------------------
			case ADSC_START_MSG:
			{
				DEB_TRACE() << "Reader::->ADSC_START_MSG";
				yat::MutexLock scoped_lock(m_lock);
				{
					m_image_number = 0;
					m_is_stop_done = false;
					m_is_running = true;
					m_is_timeout_signaled = false;
				}
			}
			break;
				//-----------------------------------------------------
			case ADSC_STOP_MSG:
			{
				DEB_TRACE() << "Reader::->ADSC_STOP_MSG";
				if(!m_is_stop_done)//sequence must be always (start->stop, start->stop, ....)
				{
					std::string full_file_name = "";
					m_is_stop_done = true;
					if (m_is_reader_open_image_file)
					{
						full_file_name = m_cam.getImagePath()+m_cam.getFileName();
						int m_elapsed_ms_from_stop = 0;
						//TODO //////////////////////////
						//Until now we must wait a xxx ms, because camera can be standby but file is not correctly present in the disk !!!!
						//perhaps   we have to test if(acquired frame == requested frames) too !
						//perhaps   we have to test if file exist on disk too !
						//certainly we have to test if file exist on disk using DI::DiffractionImage !
						/////////////////////////////////
						while(m_elapsed_ms_from_stop<m_timeout)
						{
							yat::ThreadingUtilities::sleep(0, TIME_SLEEP*1E6);//unit of yat::sleep is nano
							m_elapsed_ms_from_stop+=TIME_SLEEP;
							DEB_TRACE() << "Elapsed time since stop() = " << m_elapsed_ms_from_stop << " ms";
						}
					}
					else
					{
						full_file_name = "SIMULATED_IMAGE_FILE.XXX";
					}

					DEB_TRACE() << "Exposure SUCCEEDED received from camera !"; //all images are acquired !
					DEB_TRACE() << "file = " << full_file_name;
					//copy image file in a new frame buffer and signal it to the clients
					addNewFrame(full_file_name);
					yat::MutexLock scoped_lock(m_lock);
					{
						m_is_timeout_signaled = false;
						m_is_running = false;
					}

				}
			}
			break;
				//-----------------------------------------------------
			case ADSC_RESET_MSG:
			{
				DEB_TRACE() << "Reader::->ADSC_RESET_MSG";
			}
			break;
				//-----------------------------------------------------
		}
	}
	catch (yat::Exception& ex)
	{
		DEB_ERROR() << "Error : " << ex.errors[0].desc;
		throw LIMA_HW_EXC(Error, ex.errors[0].desc);
	}

}

//---------------------------
//- Reader::addNewFrame()
//---------------------------
void Reader::addNewFrame(std::string filename)
{
	DEB_MEMBER_FUNCT();
	try
	{
		StdBufferCbMgr& buffer_mgr = ((reinterpret_cast<BufferCtrlObj&>(m_buffer)).getBufferCbMgr());
		bool continueAcq = false;
		int buffer_nb, concat_frame_nb;

		DEB_TRACE() << "image#" << m_image_number << " acquired !";
		buffer_mgr.setStartTimestamp(Timestamp::now());
		buffer_mgr.acqFrameNb2BufferNb(m_image_number, buffer_nb, concat_frame_nb);

		//prepare the buffer !
		DEB_TRACE() << "-- prepare image buffer";
		void *ptr = buffer_mgr.getBufferPtr(buffer_nb, concat_frame_nb);

		//use Diffraction image library or not according to "m_is_reader_open_image_file"
		if (m_is_reader_open_image_file)
		{
			//read image file using diffractionImage library
			DEB_TRACE() << "-- create Diffraction Image object attached to image";
			DI::DiffractionImage di(const_cast<char*>(filename.c_str()));

			if(	m_image_size.getWidth()!=di.getWidth() || m_image_size.getHeight()!=di.getHeight())
				throw LIMA_HW_EXC(Error, "Image size in file is different from the expected image size of this detector !");

			DEB_TRACE() << "-- copy image in buffer";
			for (int j = 0; j < di.getWidth() * di.getHeight(); j++)
			{
				((uint16_t*) ptr)[j] = (uint16_t)(di.getImage()[j]);
			}
		}
		else
		{
			//use simulated image (all pixels are set to 0)
			DEB_TRACE() << "-- copy simulated image in buffer";
			memcpy((uint16_t *) ptr, (uint16_t *) (m_image), m_image_size.getWidth() * m_image_size.getHeight() * 2); //*2 because 16bits
		}

		DEB_TRACE() << "-- newFrameReady";
		HwFrameInfoType frame_info;
		frame_info.acq_frame_nb = m_image_number;
		continueAcq = buffer_mgr.newFrameReady(frame_info);
	}
	catch (yat::Exception& ex)
	{
		// Error handling
		DEB_ERROR() << ex.errors[0].desc;
		throw LIMA_HW_EXC(Error, ex.errors[0].desc);
	}
	return;
}
//---------------------------
