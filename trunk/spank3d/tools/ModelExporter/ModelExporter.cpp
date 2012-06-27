/*!
 * \file ModelExporter.cpp
 * \date 1-3-2010 21:00:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "ModelExporter.h"
#include "resource.h"

DWORD WINAPI DummyFunc(LPVOID arg)
{
	return 0;
}

ModelExporter::ModelExporter()
{
	// TODO: 
}

ModelExporter::~ModelExporter()
{
	// TODO: 
}

int ModelExporter::ExtCount()
{
	return 1;
}

const TCHAR* ModelExporter::Ext(int n)
{
	return _T("mesh");
}

const TCHAR* ModelExporter::LongDesc()
{
	return _T("Export to origin engine model file format");
}

const TCHAR* ModelExporter::ShortDesc()
{
	return _T("Origin Engine Model");
}

const TCHAR* ModelExporter::AuthorName()
{
	return _T("zjhlogo");
}

const TCHAR* ModelExporter::CopyrightMessage()
{
	return _T("Copyright (c) 2009, zjhlogo All rights reserved");
}

const TCHAR* ModelExporter::OtherMessage1()
{
	return _T("Other message1");
}

const TCHAR* ModelExporter::OtherMessage2()
{
	return _T("Other message2");
}

unsigned int ModelExporter::Version()
{
	return 100;
}

void ModelExporter::ShowAbout(HWND hWnd)
{
	MessageBox(hWnd, _T("Origin Engine Model Exporter"), _T("Copyright (c) 2009, zjhlogo All rights reserved"), MB_OK);
}

BOOL CALLBACK DlgOptionProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{

		}
		return (TRUE);
	case WM_CLOSE:
		{
			EndDialog(hDlg,0);
		}
		return (TRUE);
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			{
				SendMessage(hDlg, WM_CLOSE, 0, 0);
			}
			return (TRUE);
		case IDOK:
			{

			}
			return (TRUE);
		}
		return (FALSE);
	}
	return (FALSE);
}

int ModelExporter::DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressPrompts /* = FALSE */, DWORD options /* = 0 */)
{
	DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), i->GetMAXHWnd(), DlgOptionProc);
	return TRUE;
}
