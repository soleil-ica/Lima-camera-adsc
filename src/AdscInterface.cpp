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

using namespace lima;
using namespace std;

/*******************************************************************
 * \brief AdscDetInfoCtrlObj constructor
 *******************************************************************/

AdscDetInfoCtrlObj::AdscDetInfoCtrlObj(AdscCamera& adsc)
	: m_adsc(adsc)
{
}

AdscDetInfoCtrlObj::~AdscDetInfoCtrlObj()
{
}

void AdscDetInfoCtrlObj::getMaxImageSize(Size& max_image_size)
{
	FrameDim fdim;
	m_adsc.getFrameDim(fdim);
	max_image_size = fdim.getSize();
}

void AdscDetInfoCtrlObj::getDetectorImageSize(Size& det_image_size)
{
	m_adsc.getMaxImageSize(det_image_size);
}

void AdscDetInfoCtrlObj::getDefImageType(ImageType& def_image_type)
{
	FrameDim fdim;
	m_adsc.getFrameDim(fdim);
	def_image_type = fdim.getImageType();
}

void AdscDetInfoCtrlObj::setCurrImageType(ImageType curr_image_type)
{
	FrameDim fdim;
	m_adsc.getFrameDim(fdim);
	fdim.setImageType(curr_image_type);
	m_adsc.setFrameDim(fdim);
}

void AdscDetInfoCtrlObj::getCurrImageType(ImageType& curr_image_type)
{
	FrameDim fdim;
	m_adsc.getFrameDim(fdim);
	curr_image_type = fdim.getImageType();
}

void AdscDetInfoCtrlObj::getPixelSize(double& pixel_size)
{
	pixel_size = 0.1025880;
}

void AdscDetInfoCtrlObj::getDetectorType(string& det_type)
{
	det_type = "Adsc";
}

void AdscDetInfoCtrlObj::getDetectorModel(string& det_model)
{
	det_model = "Q315r";
}

void AdscDetInfoCtrlObj::registerMaxImageSizeCallback(
						HwMaxImageSizeCallback& cb)
{
	m_mis_cb_gen.registerMaxImageSizeCallback(cb);
}

void AdscDetInfoCtrlObj::unregisterMaxImageSizeCallback(
						HwMaxImageSizeCallback& cb)
{
	m_mis_cb_gen.unregisterMaxImageSizeCallback(cb);
}


void AdscDetInfoCtrlObj::
     MaxImageSizeCallbackGen::setMaxImageSizeCallbackActive(bool cb_active)
{
}

/*******************************************************************
 * \brief AdscSyncCtrlObj constructor
 *******************************************************************/

AdscSyncCtrlObj::AdscSyncCtrlObj(AdscCamera& adsc)
	: HwSyncCtrlObj(), m_adsc(adsc)
{
}

AdscSyncCtrlObj::~AdscSyncCtrlObj()
{
}

bool AdscSyncCtrlObj::checkTrigMode(TrigMode trig_mode)
{
	return (trig_mode == ExtTrigSingle);
}

void AdscSyncCtrlObj::setTrigMode(TrigMode trig_mode)
{
	if (!checkTrigMode(trig_mode))
		throw LIMA_HW_EXC(InvalidValue, "Invalid (external) trigger");
}

void AdscSyncCtrlObj::getTrigMode(TrigMode& trig_mode)
{
	trig_mode = ExtTrigSingle;
}

void AdscSyncCtrlObj::setExpTime(double exp_time)
{
	m_adsc.setExpTime(exp_time);
}

void AdscSyncCtrlObj::getExpTime(double& exp_time)
{
	m_adsc.getExpTime(exp_time);
}

void AdscSyncCtrlObj::setLatTime(double lat_time)
{
	m_adsc.setLatTime(lat_time);
}

void AdscSyncCtrlObj::getLatTime(double& lat_time)
{
	m_adsc.getLatTime(lat_time);
}

void AdscSyncCtrlObj::setNbHwFrames(int nb_frames)
{
	m_adsc.setNbFrames(nb_frames);
}

void AdscSyncCtrlObj::getNbHwFrames(int& nb_frames)
{
	m_adsc.getNbFrames(nb_frames);
}

void AdscSyncCtrlObj::getValidRanges(ValidRangesType& valid_ranges)
{
	double min_time = 10e-9;
	double max_time = 1e6;
	valid_ranges.min_exp_time = min_time;
	valid_ranges.max_exp_time = max_time;
	valid_ranges.min_lat_time = min_time;
	valid_ranges.max_lat_time = max_time;
}


/*******************************************************************
 * \brief AdscBinCtrlObj constructor
 *******************************************************************/

AdscBinCtrlObj::AdscBinCtrlObj(AdscCamera& adsc)
	: m_adsc(adsc)
{
}

AdscBinCtrlObj::~AdscBinCtrlObj()
{
}

void AdscBinCtrlObj::setBin(const Bin& bin)
{
	m_adsc.setBin(bin);
}

void AdscBinCtrlObj::getBin(Bin& bin)
{
	m_adsc.getBin(bin);
}

void AdscBinCtrlObj::checkBin(Bin& bin)
{
	m_adsc.checkBin(bin);
}


/*******************************************************************
 * \brief AdscInterface constructor
 *******************************************************************/

AdscInterface::AdscInterface(AdscCamera& adsc)
	: m_adsc(adsc), m_det_info(adsc),
	  m_sync(adsc), m_bin(adsc)
{
	HwDetInfoCtrlObj *det_info = &m_det_info;
	m_cap_list.push_back(HwCap(det_info));

	m_cap_list.push_back(HwCap(adsc.getBufferMgr()));

	HwSyncCtrlObj *sync = &m_sync;
	m_cap_list.push_back(HwCap(sync));

	HwBinCtrlObj *bin = &m_bin;
	m_cap_list.push_back(HwCap(bin));
}

AdscInterface::~AdscInterface()
{
}

void AdscInterface::getCapList(HwInterface::CapList &aReturnCapList) const
{
  aReturnCapList = m_cap_list;
}

void AdscInterface::reset(ResetLevel reset_level)
{
	m_adsc.reset();
}

void AdscInterface::prepareAcq()
{
}

void AdscInterface::startAcq()
{
	m_adsc.startAcq();
}

void AdscInterface::stopAcq()
{
	m_adsc.stopAcq();
}

void AdscInterface::getStatus(StatusType& status)
{
	AdscCamera::Status adsc_status = m_adsc.getStatus();
	switch (adsc_status) {
	case AdscCamera::Ready:
		status.acq = AcqReady;
		status.det = DetIdle;
		break;
	case AdscCamera::Exposure:
		status.det = DetExposure;
		goto Running;
	case AdscCamera::Readout:
		status.det = DetReadout;
		goto Running;
	case AdscCamera::Latency:
		status.det = DetLatency;
	Running:
		status.acq = AcqRunning;
		break;
	}
	status.det_mask = DetExposure | DetReadout | DetLatency;
}

int AdscInterface::getNbHwAcquiredFrames()
{
	return m_adsc.getNbAcquiredFrames();
}

