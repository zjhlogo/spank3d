/*!
 * \file UiDesignerApp.h
 * \date 11-21-2012 10:47:15
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __UIDESIGNERAPP_H__
#define __UIDESIGNERAPP_H__

#include <wx/app.h>

class UiDesignerApp : public wxApp
{
public:
	UiDesignerApp();
	virtual ~UiDesignerApp();

	virtual bool OnInit();

private:
	wxLocale m_Local;

};
#endif // __UIDESIGNERAPP_H__
