/*!
 * \file ModelExporter.h
 * \date 1-3-2010 21:00:28
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MODELEXPORTER_H__
#define __MODELEXPORTER_H__

#include <max.h>
#include <IGame/IGame.h>
#include <IGame/IGameModifier.h>

class ModelExporter : public SceneExport
{
public:
	ModelExporter();
	virtual ~ModelExporter();

	virtual int ExtCount();						// Number of extensions supported
	virtual const TCHAR* Ext(int n);			// Extension #n (i.e. "3DS")
	virtual const TCHAR* LongDesc();			// Long ASCII description (i.e. "Autodesk 3D Studio File")
	virtual const TCHAR* ShortDesc();			// Short ASCII description (i.e. "3D Studio")
	virtual const TCHAR* AuthorName();			// ASCII Author name
	virtual const TCHAR* CopyrightMessage();	// ASCII Copyright message
	virtual const TCHAR* OtherMessage1();		// Other message #1
	virtual const TCHAR* OtherMessage2();		// Other message #2
	virtual unsigned int Version();				// Version number * 100 (i.e. v3.01 = 301)
	virtual void ShowAbout(HWND hWnd);			// Show DLL's "About..." box
	virtual int DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressPrompts = FALSE, DWORD options = 0);	// Export file

};
#endif // __MODELEXPORTER_H__
