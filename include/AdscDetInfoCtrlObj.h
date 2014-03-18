#ifndef ADSCDETINFOCTRLOBJ_H
#define ADSCDETINFOCTRLOBJ_H

#include "Debug.h"
#include "AdscCompatibility.h"
#include "HwDetInfoCtrlObj.h"
#include "AdscCamera.h"


namespace lima {
namespace Adsc {

/*******************************************************************
 * \class DetInfoCtrlObj
 * \brief Control object providing simulator detector info interface
 *******************************************************************/
class LIBADSC_API DetInfoCtrlObj : public HwDetInfoCtrlObj
{
	DEB_CLASS_NAMESPC(DebModCamera, "DetInfoCtrlObj", "Adsc");
public:
	DetInfoCtrlObj(Camera& adsc);
	virtual ~DetInfoCtrlObj();

	virtual void getMaxImageSize(Size& max_image_size);
	virtual void getDetectorImageSize(Size& det_image_size);

	virtual void getDefImageType(ImageType& def_image_type);
	virtual void getCurrImageType(ImageType& curr_image_type);
	virtual void setCurrImageType(ImageType curr_image_type);

	virtual void getPixelSize(double& x_size,double &y_size);
	virtual void getDetectorType(std::string& det_type);
	virtual void getDetectorModel(std::string& det_model);

	virtual void registerMaxImageSizeCallback(HwMaxImageSizeCallback& cb);
	virtual void unregisterMaxImageSizeCallback(HwMaxImageSizeCallback& cb);

private:
	class MaxImageSizeCallbackGen : public HwMaxImageSizeCallbackGen
	{
	protected:
		virtual void setMaxImageSizeCallbackActive(bool cb_active);
	};

	Camera& m_adsc;
	MaxImageSizeCallbackGen m_mis_cb_gen;
};

} // namespace lima
} // namespace Adsc

#endif //ADSCDETINFOCTRLOBJ_H
