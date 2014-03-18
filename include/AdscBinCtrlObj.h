#ifndef ADSCBINCTRLOBJ_H
#define ADSCBINCTRLOBJ_H

#include "Debug.h"
#include "AdscCompatibility.h"
#include "HwBinCtrlObj.h"
#include "AdscCamera.h"


namespace lima {
namespace Adsc {

//class Camera;

/*******************************************************************
 * \class adscBinCtrlObj
 * \brief Control object providing simulator binning interface
 *******************************************************************/
class LIBADSC_API BinCtrlObj : public HwBinCtrlObj
{
	DEB_CLASS_NAMESPC(DebModCamera, "BinCtrlObj", "Adsc");
public:
	BinCtrlObj(Camera& adsc);
	virtual ~BinCtrlObj();

	virtual void setBin(const Bin& bin);
	virtual void getBin(Bin& bin);
	virtual void checkBin(Bin& bin);

private:
	Camera& m_adsc;
};

} // namespace Adsc
} // namespace lima

#endif //ADSCBINCTRLOBJ_H
