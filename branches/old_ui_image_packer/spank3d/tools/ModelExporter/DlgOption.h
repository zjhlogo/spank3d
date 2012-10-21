/*!
 * \file DlgOption.h
 * \date 6-23-2012 10:07:42
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DLGOPTION_H__
#define __DLGOPTION_H__

#include "DllEntry.h"
#include <type/FmtMesh.h>
#include <type/FmtSkeleton.h>

class DlgOption
{
public:
	DlgOption(HWND hWndParent);
	~DlgOption();

	bool Initialize(HWND hWndThis);
	DWORD Show();

	bool IsExportMesh();
	bool IsExportSkeleton();
	DWORD GetVertexAttributes();

private:
	bool OnCommand(DWORD dwCtrlId, DWORD dwEventId, HWND hWndChild);
	void OnOk();
	void OnCancel();

	void UpdateView();
	void UpdateData();

	void CenterWindow(HWND hWnd);
	static BOOL CALLBACK DlgOptionProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hWndParent;
	HWND m_hWndThis;

	DWORD m_Result;
	bool m_bExportMesh;
	bool m_bExportSkeleton;
	DWORD m_dwVertexAttrs;

};
#endif // __DLGOPTION_H__
