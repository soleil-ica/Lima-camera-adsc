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
#include "AdscCamera.h"
#include "DetconFcns.h"
#include "DetconPar.h"
#include "DetconState.h"

#include <string>
#include <unistd.h>
#include <cmath>

using namespace lima;
using namespace std;

AdscCamera::AdscThread::AdscThread(AdscCamera& adsc)
	: m_adsc(&adsc)
{	
	m_acq_frame_nb = 0;
	m_force_stop = false;
}

void AdscCamera::AdscThread::start()
{
	CmdThread::start();
	waitStatus(Ready);
}

void AdscCamera::AdscThread::init()
{
	setStatus(Ready);
}

void AdscCamera::AdscThread::execCmd(int cmd)
{
	int status = getStatus();
	switch (cmd) {
	case StartAcq:
		if (status != Ready)
			throw LIMA_HW_EXC(InvalidValue,  "Not Ready to StartAcq");
		execStartAcq();
		break;
	}
}

void AdscCamera::AdscThread::execStartAcq()
{
	DEB_MEMBER_FUNCT();
	
	StdBufferCbMgr& buffer_mgr = m_adsc->m_buffer_ctrl_mgr.getBuffer();
	buffer_mgr.setStartTimestamp(Timestamp::now());

	AdscApi& adsc_api = m_adsc->m_adsc_api;
	adsc_api.resetFrameNr();

	int nb_frames = m_adsc->m_nb_frames;
	int& frame_nb = m_acq_frame_nb;
	for (frame_nb = 0; (frame_nb < nb_frames)||(nb_frames==0); frame_nb++) {
		double req_time;
		if(m_force_stop)
		{
			m_force_stop = false;
			setStatus(Ready);
			return;
		}
		req_time = m_adsc->m_exp_time;
		if (req_time > 0) {	
			setStatus(Exposure);
			usleep(long(req_time * 1e6));
		}

		setStatus(Readout);
		void *ptr = buffer_mgr.getFrameBufferPtr(frame_nb);
		typedef unsigned char *BufferPtr;
		adsc_api.getNextFrame(BufferPtr(ptr));

		HwFrameInfoType frame_info;
		frame_info.acq_frame_nb = frame_nb;
		buffer_mgr.newFrameReady(frame_info);

		req_time = m_adsc->m_lat_time;
		if (req_time > 0) {
			setStatus(Latency);
			usleep(long(req_time * 1e6));
		}
	}
	setStatus(Ready);
}

int AdscCamera::AdscThread::getNbAcquiredFrames()
{
	DEB_MEMBER_FUNCT();
	return m_acq_frame_nb;
}

AdscCamera::AdscCamera() : 
  m_thread(*this)
{
	DEB_CONSTRUCTOR();
	
	init();

	m_thread.start();
}

void AdscCamera::init()
{
	DEB_MEMBER_FUNCT();
	m_exp_time = 1.0;
	m_lat_time = 0.0;
	m_nb_frames = 1;
	m_flp_kind = 0;
	m_bin_used = 2;
	m_adc_used = 1;
	m_output_raws = 0;
	m_no_transform = 0;
	m_use_stored_dark = 0;
	m_last_image = 0;
	m_filename = "/home/ccd/sn902r_calib_2006/sim/F6_1_001";

	CCDInitialize();

}

AdscCamera::~AdscCamera()
{
	DEB_DESTRUCTOR();
}

HwBufferCtrlObj* AdscCamera::getBufferMgr()
{
	DEB_MEMBER_FUNCT();
	return &m_buffer_ctrl_mgr;
}

void AdscCamera::getMaxImageSize(Size& max_image_size)
{
	DEB_MEMBER_FUNCT();
	m_adsc_api.getMaxImageSize(max_image_size);
}

void AdscCamera::setNbFrames(int nb_frames)
{
	DEB_MEMBER_FUNCT();
	if (nb_frames < 0)
		throw LIMA_HW_EXC(InvalidValue, "Invalid nb of frames");

	m_nb_frames = nb_frames;
}

void AdscCamera::getNbFrames(int& nb_frames)
{
	DEB_MEMBER_FUNCT();
	nb_frames = m_nb_frames;
}

void AdscCamera::setExpTime(double exp_time)
{
	DEB_MEMBER_FUNCT();
	if (exp_time <= 0)
		throw LIMA_HW_EXC(InvalidValue, "Invalid exposure time");
		
	m_exp_time = exp_time;
}

void AdscCamera::getExpTime(double& exp_time)
{
	DEB_MEMBER_FUNCT();
	exp_time = m_exp_time;
}

void AdscCamera::setLatTime(double lat_time)
{
	DEB_MEMBER_FUNCT();
	if (lat_time < 0)
		throw LIMA_HW_EXC(InvalidValue, "Invalid latency time");
		
	m_lat_time = lat_time;
}

void AdscCamera::getLatTime(double& lat_time)
{
	DEB_MEMBER_FUNCT();
	lat_time = m_lat_time;
}

void AdscCamera::setBin(const Bin& bin)
{
	DEB_MEMBER_FUNCT();
	m_adsc_api.setBin(bin);
}

void AdscCamera::getBin(Bin& bin)
{
	DEB_MEMBER_FUNCT();
	m_adsc_api.getBin(bin);
}

void AdscCamera::checkBin(Bin& bin)
{
	DEB_MEMBER_FUNCT();
	m_adsc_api.checkBin(bin);
}

void AdscCamera::setFrameDim(const FrameDim& frame_dim)
{
	DEB_MEMBER_FUNCT();
	m_adsc_api.setFrameDim(frame_dim);
}

void AdscCamera::getFrameDim(FrameDim& frame_dim)
{
	DEB_MEMBER_FUNCT();
	m_adsc_api.getFrameDim(frame_dim);
}

void AdscCamera::reset()
{
	DEB_MEMBER_FUNCT();
	stopAcq();

	init();
}

void AdscCamera::setImageKind(int image_kind)
{
	DEB_MEMBER_FUNCT();
	m_flp_kind = image_kind;
}

void AdscCamera::setFilename(std::string filename)
{
	DEB_MEMBER_FUNCT();
	m_filename = filename;
}

void AdscCamera::setLastImage(int last_image)
{
	DEB_MEMBER_FUNCT();
	m_last_image = last_image;
}

AdscCamera::Status AdscCamera::getStatus()
{
	DEB_MEMBER_FUNCT();
	int thread_status = m_thread.getStatus();
	switch (thread_status) {
	case AdscThread::Ready:
		return AdscCamera::Ready;
	case AdscThread::Exposure:
		return AdscCamera::Exposure;
	case AdscThread::Readout:
		return AdscCamera::Readout;
	case AdscThread::Latency:
		return AdscCamera::Latency;
	default:
		throw LIMA_HW_EXC(Error, "Invalid thread status");
	}
}

void AdscCamera::startAcq()
{

	DEB_MEMBER_FUNCT();

	float	f_stat_time;

	m_thread.m_force_stop = false;		//ugly but works
	m_buffer_ctrl_mgr.getBuffer().setStartTimestamp(Timestamp::now());

//
//	interface to the ccd library, starting image.
//
//	These items will change from image to image
//
	f_stat_time = m_exp_time;
	CCDSetFilePar(FLP_TIME, (char *) &f_stat_time);
	CCDSetFilePar(FLP_FILENAME,(char *) m_filename.c_str());
	CCDSetFilePar(FLP_KIND, (char *) &m_flp_kind);
//
//	These items do not change (at the present time)
//
	CCDSetHwPar(HWP_BIN, (char *) &m_bin_used);
	CCDSetHwPar(HWP_ADC, (char *) &m_adc_used);
	CCDSetHwPar(HWP_SAVE_RAW, (char *) &m_output_raws);
	CCDSetHwPar(HWP_NO_XFORM, (char *) &m_no_transform);
	CCDSetHwPar(HWP_STORED_DARK, (char *) &m_use_stored_dark);

	CCDStartExposure();
	while(DTC_STATE_EXPOSING != CCDState())
	{
		if(DTC_STATE_ERROR == CCDState())
		{
			fprintf(stdout,"Error returned from CCDStartExposure()\n");
			cout << "Error returned from CCDStartExposure()" << endl;
			return;
		}
	}

	m_thread.sendCmd(AdscThread::StartAcq);
	m_thread.waitNotStatus(AdscThread::Ready);
}

void AdscCamera::stopAcq()
{		
	DEB_MEMBER_FUNCT();
	m_thread.m_force_stop = true;		//ugly but works
	m_thread.sendCmd(AdscThread::StopAcq);
	m_thread.waitStatus(AdscThread::Ready);

//
//	Interface to CCD library here.
//
	CCDStopExposure();
	while(DTC_STATE_IDLE != CCDState())
	{
		if(DTC_STATE_ERROR == CCDState())
		{
			fprintf(stdout,"Error returned from CCDStartExposure()\n");
			cout << "Error returned from CCDStartExposure()" << endl;
			return;
		}
	}
	if(5 == m_flp_kind)
		m_last_image = 1;
	else
		m_last_image = 0;

	CCDSetFilePar(FLP_LASTIMAGE, (char *) &m_last_image);
	CCDGetImage();
}

int AdscCamera::getNbAcquiredFrames()
{
	DEB_MEMBER_FUNCT();
	return m_thread.getNbAcquiredFrames();
}

ostream& lima::operator <<(ostream& os, AdscCamera& adsc)
{
	string status;
	switch (adsc.getStatus()) {
	case AdscCamera::Ready:
		status = "Ready"; break;
	case AdscCamera::Exposure:
		status = "Exposure"; break;
	case AdscCamera::Readout:
		status = "Readout"; break;
	case AdscCamera::Latency:
		status = "Latency"; break;
	default:
		status = "Unknown";
	}
	os << "<status=" << status << ">";
	return os;
}
