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
#ifndef ADSCINTERFACE_H
#define ADSCINTERFACE_H

#include "lima/Debug.h"
#include "AdscCompatibility.h"
#include "lima/HwInterface.h"
#include "AdscDetInfoCtrlObj.h"
#include "AdscBufferCtrlObj.h"
#include "AdscSyncCtrlObj.h"
#include "AdscBinCtrlObj.h"
#include "AdscCamera.h"

using namespace std;

namespace lima
{
namespace Adsc
{

/*******************************************************************
 * \class Interface
 * \brief Adsc interface
 *******************************************************************/
class LIBADSC_API Interface : public HwInterface
{
	DEB_CLASS_NAMESPC(DebModCamera, "AdscInterface", "Adsc");
public:
	Interface(Camera& adsc);
	virtual ~Interface();

	virtual void 	getCapList(CapList&) const;

	virtual void 	reset(ResetLevel reset_level);
	virtual void 	prepareAcq();
	virtual void 	startAcq();
	virtual void 	stopAcq();
	virtual void 	getStatus(StatusType& status);
	virtual int 	getNbHwAcquiredFrames();

    //! get the camera object to access it directly from client
    Camera& getCamera()
        {return m_adsc;}

    //! Set the header parameters
	void 			setHeaderParameters(const string& header);
    //! Say to ADSC to store or not an image dark
	void			setStoredImageDark(bool value);
    //! Get the StoredImageDark flag
	bool			getStoredImageDark(void);
    //! Set the kind of image to be acquired (1-> ??; 2-> ?? ... 5-> ??)
	void    		setImageKind(int image_kind);
    //! Getter of the ImageKind
	int	    		getImageKind(void);	
    //! Set the Last image (0-> ??; 1-> ??)
	void    		setLastImage(int last_image);
    //! Getter of the last image
	int	    		getLastImage(void);		
    //! Set the file name to be saved
	void 			setFileName(const string& name);
    //! Get the file name
	const string& 	getFileName(void);
    //! Set the image path
	void 			setImagePath(const string& path);
    //! Get the image path
	const string& 	getImagePath(void);
    //! set the timeout
	void 			setTimeout(int TO);
    //! enable the image file reading (the image will be in lima)
	void 			enableReader(void);
    //! disable the image file reading (the image will NOT be in lima)
	void 			disableReader(void);

private:
	Camera& 		m_adsc;
	CapList 		m_cap_list;
	DetInfoCtrlObj 	m_det_info;
	BufferCtrlObj 	m_buffer;
	SyncCtrlObj 	m_sync;
	BinCtrlObj 		m_bin;
};

} // namespace Adsc
} // namespace lima

#endif // ADSCINTERFACE_H
