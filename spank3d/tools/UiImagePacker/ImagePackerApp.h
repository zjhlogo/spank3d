/*!
 * \file ImagePackerApp.h
 * \date 2-18-2012 9:42:53
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IMAGEPACKERAPP_H__
#define __IMAGEPACKERAPP_H__

#include <wx/app.h>

class ImagePackerApp : public wxApp
{
public:
	ImagePackerApp();
	virtual ~ImagePackerApp();

	virtual bool OnInit();

private:
	wxLocale m_Local;

};
#endif // __IMAGEPACKERAPP_H__
