#include "AdscDetInfoCtrlObj.h"
#include "AdscCamera.h"

using namespace lima::Adsc;

/*******************************************************************
 * \brief DetInfoCtrlObj constructor
 *******************************************************************/
DetInfoCtrlObj::DetInfoCtrlObj(Camera& adsc) :
		m_adsc(adsc)
{
	DEB_CONSTRUCTOR();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
DetInfoCtrlObj::~DetInfoCtrlObj()
{
	DEB_DESTRUCTOR();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void DetInfoCtrlObj::getMaxImageSize(Size& max_image_size)
{
	DEB_MEMBER_FUNCT();
	FrameDim fdim;
	m_adsc.getFrameDim(fdim);
	max_image_size = fdim.getSize();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void DetInfoCtrlObj::getDetectorImageSize(Size& det_image_size)
{
	DEB_MEMBER_FUNCT();
	m_adsc.getMaxImageSize(det_image_size);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void DetInfoCtrlObj::getDefImageType(ImageType& def_image_type)
{
	DEB_MEMBER_FUNCT();
	FrameDim fdim;
	m_adsc.getFrameDim(fdim);
	def_image_type = fdim.getImageType();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void DetInfoCtrlObj::setCurrImageType(ImageType curr_image_type)
{
	DEB_MEMBER_FUNCT();
	FrameDim fdim;
	m_adsc.getFrameDim(fdim);
	fdim.setImageType(curr_image_type);
	m_adsc.setFrameDim(fdim);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void DetInfoCtrlObj::getCurrImageType(ImageType& curr_image_type)
{
	DEB_MEMBER_FUNCT();
	FrameDim fdim;
	m_adsc.getFrameDim(fdim);
	curr_image_type = fdim.getImageType();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void DetInfoCtrlObj::getPixelSize(double& x_size,double &y_size)
{
	DEB_MEMBER_FUNCT();
	x_size = 0.1025880;
	y_size = 0.1025880;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void DetInfoCtrlObj::getDetectorType(std::string& det_type)
{
	DEB_MEMBER_FUNCT();
	det_type = "Adsc";
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void DetInfoCtrlObj::getDetectorModel(std::string& det_model)
{
	DEB_MEMBER_FUNCT();
	det_model = "Q315r";
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void DetInfoCtrlObj::registerMaxImageSizeCallback(HwMaxImageSizeCallback& cb)
{
	DEB_MEMBER_FUNCT();
	m_mis_cb_gen.registerMaxImageSizeCallback(cb);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void DetInfoCtrlObj::unregisterMaxImageSizeCallback(HwMaxImageSizeCallback& cb)
{
	DEB_MEMBER_FUNCT();
	m_mis_cb_gen.unregisterMaxImageSizeCallback(cb);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void DetInfoCtrlObj::MaxImageSizeCallbackGen::setMaxImageSizeCallbackActive(bool cb_active)
{
}
