#ifndef ADSCSYNCHCTRLOBJ_H
#define ADSCSYNCHCTRLOBJ_H

#include "lima/Debug.h"
#include "AdscCompatibility.h"
#include "lima/HwSyncCtrlObj.h"
#include "lima/HwInterface.h"
#include "AdscCamera.h"

namespace lima {
namespace Adsc {


/*******************************************************************
 * \class SyncCtrlObj
 * \brief Control object providing simulator synchronization interface
 *******************************************************************/
class LIBADSC_API SyncCtrlObj : public HwSyncCtrlObj
{
	DEB_CLASS_NAMESPC(DebModCamera, "SyncCtrlObj", "Adsc");
public:
	SyncCtrlObj(Camera& adsc);
	virtual ~SyncCtrlObj();

	virtual bool checkTrigMode(TrigMode trig_mode);
	virtual void setTrigMode(TrigMode trig_mode);
	virtual void getTrigMode(TrigMode& trig_mode);

	virtual void setExpTime(double exp_time);
	virtual void getExpTime(double& exp_time);

	virtual void setLatTime(double lat_time);
	virtual void getLatTime(double& lat_time);

	virtual void setNbHwFrames(int nb_frames);
	virtual void getNbHwFrames(int& nb_frames);

	virtual void getValidRanges(ValidRangesType& valid_ranges);

private:
	Camera& m_adsc;
};

} // namespace Adsc
} // namespace lima

#endif // ADSCSYNCHCTRLOBJ_H
