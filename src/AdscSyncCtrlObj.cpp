#include "AdscSyncCtrlObj.h"


using namespace lima;
using namespace lima::Adsc;


/*******************************************************************
 * \brief SyncCtrlObj constructor
 *******************************************************************/
SyncCtrlObj::SyncCtrlObj(Camera& adsc) :
		HwSyncCtrlObj(),
		m_adsc(adsc)
{
	DEB_CONSTRUCTOR();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
SyncCtrlObj::~SyncCtrlObj()
{
	DEB_DESTRUCTOR();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool SyncCtrlObj::checkTrigMode(TrigMode trig_mode)
{
	DEB_MEMBER_FUNCT();
	bool valid_mode = false;
	switch(trig_mode)
	{
		case IntTrig :
			valid_mode = true;
		break;
		default :
			valid_mode = false;
		break;
	}
	return valid_mode;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SyncCtrlObj::setTrigMode(TrigMode trig_mode)
{
	DEB_MEMBER_FUNCT();
	if(!checkTrigMode(trig_mode))
		THROW_HW_ERROR(InvalidValue) << "Invalid " << DEB_VAR1(trig_mode);
	//m_adsc.setTrigMode(trig_mode);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SyncCtrlObj::getTrigMode(TrigMode& trig_mode)
{
	DEB_MEMBER_FUNCT();
	trig_mode = IntTrig;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SyncCtrlObj::setExpTime(double exp_time)
{
	DEB_MEMBER_FUNCT();
	m_adsc.setExpTime(exp_time);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SyncCtrlObj::getExpTime(double& exp_time)
{
	DEB_MEMBER_FUNCT();
	m_adsc.getExpTime(exp_time);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SyncCtrlObj::setLatTime(double lat_time)
{
	DEB_MEMBER_FUNCT();
	m_adsc.setLatTime(lat_time);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SyncCtrlObj::getLatTime(double& lat_time)
{
	DEB_MEMBER_FUNCT();
	m_adsc.getLatTime(lat_time);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SyncCtrlObj::setNbHwFrames(int nb_frames)
{
	DEB_MEMBER_FUNCT();
	m_adsc.setNbFrames(nb_frames);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SyncCtrlObj::getNbHwFrames(int& nb_frames)
{
	DEB_MEMBER_FUNCT();
	m_adsc.getNbFrames(nb_frames);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SyncCtrlObj::getValidRanges(ValidRangesType& valid_ranges)
{
	DEB_MEMBER_FUNCT();
	double min_time = 10e-9;
	double max_time = 1e6;
	valid_ranges.min_exp_time = min_time;
	valid_ranges.max_exp_time = max_time;
	valid_ranges.min_lat_time = min_time;
	valid_ranges.max_lat_time = max_time;
}
