#include "AdscBinCtrlObj.h"

using namespace lima::Adsc;

/*******************************************************************
 * \brief BinCtrlObj constructor
 *******************************************************************/
BinCtrlObj::BinCtrlObj(Camera& adsc) :
		m_adsc(adsc)
{
	DEB_CONSTRUCTOR();
}

//-----------------------------------------------------
BinCtrlObj::~BinCtrlObj()
{
	DEB_DESTRUCTOR();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BinCtrlObj::setBin(const Bin& bin)
{
	DEB_MEMBER_FUNCT();
	m_adsc.setBin(bin);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BinCtrlObj::getBin(Bin& bin)
{
	DEB_MEMBER_FUNCT();
	m_adsc.getBin(bin);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BinCtrlObj::checkBin(Bin& bin)
{
	DEB_MEMBER_FUNCT();
	m_adsc.checkBin(bin);
}

