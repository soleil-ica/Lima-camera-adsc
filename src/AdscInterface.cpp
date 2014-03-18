//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2011
// European Synchrotron Radiation Facility
// BP 220, Grenoble 38043
// FRANCE
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//###########################################################################

#include "AdscInterface.h"

using namespace lima::Adsc;

/*******************************************************************
 * \brief Interface constructor
 *******************************************************************/
Interface::Interface(Camera& adsc) :
		m_adsc(adsc),
		m_det_info(adsc),
		m_buffer(adsc),
		m_sync(adsc),
		m_bin(adsc)
{
	DEB_CONSTRUCTOR();
	HwDetInfoCtrlObj *det_info = &m_det_info;
	m_cap_list.push_back(HwCap(det_info));
	
	HwSyncCtrlObj *sync = &m_sync;
	m_cap_list.push_back(HwCap(sync));
	
	HwBufferCtrlObj *buffer = &m_buffer;
	m_cap_list.push_back(HwCap(buffer));
	
	HwBinCtrlObj *bin = &m_bin;
	m_cap_list.push_back(HwCap(bin));
}

//-----------------------------------------------------
//
//-----------------------------------------------------
Interface::~Interface()
{
	DEB_DESTRUCTOR();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::getCapList(HwInterface::CapList &aReturnCapList) const
{
	DEB_MEMBER_FUNCT();
	aReturnCapList = m_cap_list;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::reset(ResetLevel reset_level)
{
	DEB_MEMBER_FUNCT();
	m_adsc.reset();
	m_buffer.stop();
	Size image_size;
	m_det_info.getMaxImageSize(image_size);
	ImageType image_type;
	m_det_info.getDefImageType(image_type);
	FrameDim frame_dim(image_size, image_type);
	m_buffer.setFrameDim(frame_dim);
	
	m_buffer.setNbConcatFrames(1);
	m_buffer.setNbBuffers(1);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::prepareAcq()
{
	DEB_MEMBER_FUNCT();
	m_buffer.reset();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::startAcq()
{
	DEB_MEMBER_FUNCT();
	m_adsc.startAcq();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::stopAcq()
{
	DEB_MEMBER_FUNCT();
	m_adsc.stopAcq();
	m_buffer.stop();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::getStatus(StatusType& status)
{
	DEB_MEMBER_FUNCT();
	Camera::Status adsc_status = m_adsc.getStatus();
	switch(adsc_status)
	{
		case Camera::Ready :
		{
			status.det = DetIdle;
	        int nbHwFrames = 0;
	        m_sync.getNbHwFrames(nbHwFrames);
	        if( m_buffer.getLastAcquiredFrame() >= nbHwFrames)
	            status.acq = AcqReady;
	        else if(m_buffer.isTimeoutSignaled())
	        	status.acq = AcqFault;
	        else if (m_buffer.isRunning())
	            status.acq = AcqRunning;
	        else
	        	status.acq = AcqReady;
		}
		break;
		case Camera::Exposure :
			status.det = DetExposure;
			status.acq = AcqRunning;
		break;
		case Camera::Readout :
			status.det = DetReadout;
			status.acq = AcqRunning;
		break;
		case Camera::Latency :
			status.det = DetLatency;
			status.acq = AcqRunning;
		break;
	}
	status.det_mask = DetExposure | DetReadout | DetLatency;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
int Interface::getNbHwAcquiredFrames()
{
	DEB_MEMBER_FUNCT();
    int acq_frames = m_buffer.getLastAcquiredFrame();
    return acq_frames;
}


//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::setStoredImageDark(bool value)
{
	DEB_MEMBER_FUNCT();
	m_adsc.setStoredImageDark(value);
		
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool Interface::getStoredImageDark(void)
{
	return m_adsc.getStoredImageDark();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::setImageKind(int image_kind)
{
	DEB_MEMBER_FUNCT();
	m_adsc.setImageKind(image_kind);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
int Interface::getImageKind(void)
{
	DEB_MEMBER_FUNCT();
	return 	m_adsc.getImageKind();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::setLastImage(int last_image)
{
	DEB_MEMBER_FUNCT();
	m_adsc.setImageKind(last_image);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
int Interface::getLastImage(void)
{
	DEB_MEMBER_FUNCT();
	return 	m_adsc.getImageKind();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::setFileName(const std::string& name)
{
	DEB_MEMBER_FUNCT();
	m_adsc.setFileName(name);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
const std::string& Interface::getFileName(void)
{
	DEB_MEMBER_FUNCT();
	return m_adsc.getFileName();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::setImagePath(const std::string& path)
{
	DEB_MEMBER_FUNCT();
	m_adsc.setImagePath(path);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
const std::string& Interface::getImagePath(void)
{
	DEB_MEMBER_FUNCT();
	return m_adsc.getImagePath();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::setHeaderParameters(const std::string& header)
{
	DEB_MEMBER_FUNCT();
	m_adsc.setHeaderParameters(header);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::setTimeout(int TO)
{
    DEB_MEMBER_FUNCT();
    m_buffer.setTimeout(TO);
}
//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::enableReader(void)
{
	m_buffer.enableReader();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void Interface::disableReader(void)
{
	m_buffer.disableReader();
}
//-----------------------------------------------------
//
//-----------------------------------------------------
