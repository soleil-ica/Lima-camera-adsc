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
#ifndef ADSCCAMERA_H
#define ADSCCAMERA_H

#include "HwBufferMgr.h"
#include "AdscApi.h"
#include "ThreadUtils.h"
#include "SizeUtils.h"
#include "Debug.h"

#include <ostream>

namespace lima
{

class LIBADSC_API AdscCamera
{
	DEB_CLASS_NAMESPC(DebModCamera, "AdscCamera", "Adsc");
 public:
	enum Status {
		Ready, Exposure, Readout, Latency,
	};

	AdscCamera();
	~AdscCamera();

	HwBufferCtrlObj* getBufferMgr();
	
	void startAcq();
	void stopAcq();

	void setNbFrames(int  nb_frames);
	void getNbFrames(int& nb_frames);

	void setExpTime(double  exp_time);
	void getExpTime(double& exp_time);

	void setLatTime(double  lat_time);
	void getLatTime(double& lat_time);

	void setBin(const Bin& bin);
	void getBin(Bin& bin);
	void checkBin(Bin& bin);

	void setFrameDim(const FrameDim& frame_dim);
	void getFrameDim(FrameDim& frame_dim);
	
	Status getStatus();
	int getNbAcquiredFrames();

	void getMaxImageSize(Size& max_image_size);

	void reset();
	void setImageKind(int image_kind);
	void setLastImage(int last_image);
	void setFilename(std::string filename);

 private:
	class AdscThread : public CmdThread
	{
	DEB_CLASS_NAMESPC(DebModCamera, "AdscThread", "Adsc");
	public:
		enum { // Status
			Ready = MaxThreadStatus, Exposure, Readout, Latency,
		};

		enum { // Cmd 
			StartAcq = MaxThreadCmd, StopAcq,
		};
		
		AdscThread(AdscCamera& adsc);

		virtual void start();
		
		int getNbAcquiredFrames();
		bool m_force_stop;

	protected:
		virtual void init();
		virtual void execCmd(int cmd);
	private:
		void execStartAcq();
		AdscCamera* m_adsc;
		int m_acq_frame_nb;
	};
	friend class AdscThread;

	void init();

	SoftBufferCtrlMgr m_buffer_ctrl_mgr;
	AdscApi m_adsc_api;
	
	double		m_exp_time;
	double		m_lat_time;
	int		m_nb_frames;
	int		m_flp_kind;
	int		m_bin_used;
	int		m_adc_used;
	int		m_output_raws;
	int		m_no_transform;
	int		m_use_stored_dark;
	int		m_last_image;
	std::string	m_filename;	// includes leading pathname
	

	AdscThread m_thread;
};

LIBADSC_API std::ostream& operator <<(std::ostream& os, AdscCamera& adsc);

} // namespace lima

#endif // ADSCCAMERA_H
