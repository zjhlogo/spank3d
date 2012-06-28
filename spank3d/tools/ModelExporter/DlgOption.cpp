/*!
 * \file DlgOption.cpp
 * \date 6-23-2012 10:07:51
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "DlgOption.h"
#include "resource.h"
#include <map>

typedef std::map<HWND, DlgOption*> TM_HWND_DIALOG;
static TM_HWND_DIALOG g_DialogMap;

static DlgOption* HWndToDialog(HWND hWnd)
{
	TM_HWND_DIALOG::iterator it = g_DialogMap.find(hWnd);
	if (it != g_DialogMap.end()) return it->second;
	return NULL;
}

DlgOption::DlgOption(HWND hWndParent)
{
	m_hWndParent = hWndParent;
	m_hWndThis = NULL;

	m_Result = IDCANCEL;
	m_bExportMesh = true;
	m_bExportSkeleton = false;

	m_dwVertexAttrs = FmtMesh::VA_POSITION|FmtMesh::VA_TEXCOORD0|FmtMesh::VA_NORMAL;
}

DlgOption::~DlgOption()
{
	// TODO: 
}

bool DlgOption::Initialize(HWND hWndThis)
{
	m_hWndThis = hWndThis;
	UpdateView();
	return true;
}

DWORD DlgOption::Show()
{
	DialogBoxParam(GetDllInstance(), MAKEINTRESOURCE(IDD_DIALOG_OPTION), m_hWndParent, DlgOptionProc, (LPARAM)this);
	return m_Result;
}

bool DlgOption::IsExportMesh()
{
	return m_bExportMesh;
}

bool DlgOption::IsExportSkeleton()
{
	return m_bExportSkeleton;
}

DWORD DlgOption::GetVertexAttributes()
{
	return m_dwVertexAttrs;
}

bool DlgOption::OnCommand(DWORD dwCtrlId, DWORD dwEventId, HWND hWndChild)
{
	switch (dwCtrlId)
	{
	case IDOK:
		OnOk();
		break;
	case IDCANCEL:
		OnCancel();
		break;
	case IDC_EXPORT_MESH:
	case IDC_POSITION:
	case IDC_TEXCOORD0:
	case IDC_TEXCOORD1:
	case IDC_TEXCOORD2:
	case IDC_TEXCOORD3:
	case IDC_TEXCOORD4:
	case IDC_TEXCOORD5:
	case IDC_TEXCOORD6:
	case IDC_TEXCOORD7:
	case IDC_COLOR:
	case IDC_NORMAL:
	case IDC_TANGENT:
	case IDC_BINORMAL:
	case IDC_SKELETON:
	case IDC_EXPORT_SKELETON:
		UpdateData();
		UpdateView();
		break;
	default:
		return false;
		break;
	}

	return true;
}

void DlgOption::OnOk()
{
	m_Result = IDOK;
	SendMessage(m_hWndThis, WM_CLOSE, 0, 0);
}

void DlgOption::OnCancel()
{
	m_Result = IDCANCEL;
	SendMessage(m_hWndThis, WM_CLOSE, 0, 0);
}

BOOL CALLBACK DlgOption::DlgOptionProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			// init dialog
			g_DialogMap.insert(std::make_pair(hDlg, (DlgOption*)lParam));
			DlgOption* pDlgOption = HWndToDialog(hDlg);
			if (pDlgOption) (pDlgOption->*(&DlgOption::Initialize))(hDlg);
		}
		break;
	case WM_CLOSE:
		{
			// remove dialog
			EndDialog(hDlg, 0);
			g_DialogMap.erase(g_DialogMap.find(hDlg));
		}
		break;
	case WM_COMMAND:
		{
			// command event
			DWORD dwCtrlId = LOWORD(wParam);
			DWORD dwEventId = HIWORD(wParam);
			HWND hChildWnd = (HWND)lParam;
			DlgOption* pDlgOption = HWndToDialog(hDlg);
			if (pDlgOption)
			{
				if ((pDlgOption->*(&DlgOption::OnCommand))(dwCtrlId, dwEventId, hChildWnd))
				{
					return (TRUE);
				}
			}
		}
		break;
	default:
		return (FALSE);
		break;
	}

	return (TRUE);
}

void DlgOption::UpdateView()
{
	DWORD dwCheckState = BST_UNCHECKED;
	if (m_bExportMesh) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_EXPORT_MESH, BM_SETCHECK, dwCheckState, 0);

	dwCheckState = BST_UNCHECKED;
	if (m_dwVertexAttrs & FmtMesh::VA_POSITION) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_POSITION, BM_SETCHECK, dwCheckState, 0);

	dwCheckState = BST_UNCHECKED;
	if (m_dwVertexAttrs & FmtMesh::VA_TEXCOORD0) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD0, BM_SETCHECK, dwCheckState, 0);

	dwCheckState = BST_UNCHECKED;
	if (m_dwVertexAttrs & FmtMesh::VA_TEXCOORD1) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD1, BM_SETCHECK, dwCheckState, 0);

	dwCheckState = BST_UNCHECKED;
	if (m_dwVertexAttrs & FmtMesh::VA_TEXCOORD2) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD2, BM_SETCHECK, dwCheckState, 0);

	dwCheckState = BST_UNCHECKED;
	if (m_dwVertexAttrs & FmtMesh::VA_TEXCOORD3) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD3, BM_SETCHECK, dwCheckState, 0);

	dwCheckState = BST_UNCHECKED;
	if (m_dwVertexAttrs & FmtMesh::VA_TEXCOORD4) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD4, BM_SETCHECK, dwCheckState, 0);

	dwCheckState = BST_UNCHECKED;
	if (m_dwVertexAttrs & FmtMesh::VA_TEXCOORD5) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD5, BM_SETCHECK, dwCheckState, 0);

	dwCheckState = BST_UNCHECKED;
	if (m_dwVertexAttrs & FmtMesh::VA_TEXCOORD6) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD6, BM_SETCHECK, dwCheckState, 0);

	dwCheckState = BST_UNCHECKED;
	if (m_dwVertexAttrs & FmtMesh::VA_TEXCOORD7) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD7, BM_SETCHECK, dwCheckState, 0);

	dwCheckState = BST_UNCHECKED;
	if (m_dwVertexAttrs & FmtMesh::VA_COLOR) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_COLOR, BM_SETCHECK, dwCheckState, 0);

	dwCheckState = BST_UNCHECKED;
	if (m_dwVertexAttrs & FmtMesh::VA_NORMAL) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_NORMAL, BM_SETCHECK, dwCheckState, 0);

	dwCheckState = BST_UNCHECKED;
	if (m_dwVertexAttrs & FmtMesh::VA_TANGENT) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_TANGENT, BM_SETCHECK, dwCheckState, 0);

	dwCheckState = BST_UNCHECKED;
	if (m_dwVertexAttrs & FmtMesh::VA_BINORMAL) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_BINORMAL, BM_SETCHECK, dwCheckState, 0);

	dwCheckState = BST_UNCHECKED;
	if (m_dwVertexAttrs & FmtMesh::VA_SKELETON) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_SKELETON, BM_SETCHECK, dwCheckState, 0);

	BOOL bEnableWindow = FALSE;
	if (m_bExportMesh) bEnableWindow = TRUE;

	EnableWindow(GetDlgItem(m_hWndThis, IDC_POSITION), bEnableWindow);
	EnableWindow(GetDlgItem(m_hWndThis, IDC_TEXCOORD0), bEnableWindow);
	EnableWindow(GetDlgItem(m_hWndThis, IDC_TEXCOORD1), bEnableWindow);
	EnableWindow(GetDlgItem(m_hWndThis, IDC_TEXCOORD2), bEnableWindow);
	EnableWindow(GetDlgItem(m_hWndThis, IDC_TEXCOORD3), bEnableWindow);
	EnableWindow(GetDlgItem(m_hWndThis, IDC_TEXCOORD4), bEnableWindow);
	EnableWindow(GetDlgItem(m_hWndThis, IDC_TEXCOORD5), bEnableWindow);
	EnableWindow(GetDlgItem(m_hWndThis, IDC_TEXCOORD6), bEnableWindow);
	EnableWindow(GetDlgItem(m_hWndThis, IDC_TEXCOORD7), bEnableWindow);
	EnableWindow(GetDlgItem(m_hWndThis, IDC_COLOR), bEnableWindow);
	EnableWindow(GetDlgItem(m_hWndThis, IDC_NORMAL), bEnableWindow);
	EnableWindow(GetDlgItem(m_hWndThis, IDC_TANGENT), bEnableWindow);
	EnableWindow(GetDlgItem(m_hWndThis, IDC_BINORMAL), bEnableWindow);
	EnableWindow(GetDlgItem(m_hWndThis, IDC_SKELETON), bEnableWindow);

	if (m_bExportSkeleton) dwCheckState = BST_CHECKED;
	SendDlgItemMessage(m_hWndThis, IDC_EXPORT_SKELETON, BM_SETCHECK, dwCheckState, 0);
}

void DlgOption::UpdateData()
{
	m_bExportMesh = false;
	if (SendDlgItemMessage(m_hWndThis, IDC_EXPORT_MESH, BM_GETCHECK, 0, 0) == BST_CHECKED) m_bExportMesh = true;

	m_bExportSkeleton = false;
	if (SendDlgItemMessage(m_hWndThis, IDC_EXPORT_SKELETON, BM_GETCHECK, 0, 0) == BST_CHECKED) m_bExportSkeleton = true;

	m_dwVertexAttrs = 0;
	if (SendDlgItemMessage(m_hWndThis, IDC_POSITION, BM_GETCHECK, 0, 0) == BST_CHECKED) m_dwVertexAttrs |= FmtMesh::VA_POSITION;
	if (SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD0, BM_GETCHECK, 0, 0) == BST_CHECKED) m_dwVertexAttrs |= FmtMesh::VA_TEXCOORD0;
	if (SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD1, BM_GETCHECK, 0, 0) == BST_CHECKED) m_dwVertexAttrs |= FmtMesh::VA_TEXCOORD1;
	if (SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD2, BM_GETCHECK, 0, 0) == BST_CHECKED) m_dwVertexAttrs |= FmtMesh::VA_TEXCOORD2;
	if (SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD3, BM_GETCHECK, 0, 0) == BST_CHECKED) m_dwVertexAttrs |= FmtMesh::VA_TEXCOORD3;
	if (SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD4, BM_GETCHECK, 0, 0) == BST_CHECKED) m_dwVertexAttrs |= FmtMesh::VA_TEXCOORD4;
	if (SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD5, BM_GETCHECK, 0, 0) == BST_CHECKED) m_dwVertexAttrs |= FmtMesh::VA_TEXCOORD5;
	if (SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD6, BM_GETCHECK, 0, 0) == BST_CHECKED) m_dwVertexAttrs |= FmtMesh::VA_TEXCOORD6;
	if (SendDlgItemMessage(m_hWndThis, IDC_TEXCOORD7, BM_GETCHECK, 0, 0) == BST_CHECKED) m_dwVertexAttrs |= FmtMesh::VA_TEXCOORD7;
	if (SendDlgItemMessage(m_hWndThis, IDC_COLOR, BM_GETCHECK, 0, 0) == BST_CHECKED) m_dwVertexAttrs |= FmtMesh::VA_COLOR;
	if (SendDlgItemMessage(m_hWndThis, IDC_NORMAL, BM_GETCHECK, 0, 0) == BST_CHECKED) m_dwVertexAttrs |= FmtMesh::VA_NORMAL;
	if (SendDlgItemMessage(m_hWndThis, IDC_TANGENT, BM_GETCHECK, 0, 0) == BST_CHECKED) m_dwVertexAttrs |= FmtMesh::VA_TANGENT;
	if (SendDlgItemMessage(m_hWndThis, IDC_BINORMAL, BM_GETCHECK, 0, 0) == BST_CHECKED) m_dwVertexAttrs |= FmtMesh::VA_BINORMAL;
	if (SendDlgItemMessage(m_hWndThis, IDC_SKELETON, BM_GETCHECK, 0, 0) == BST_CHECKED) m_dwVertexAttrs |= FmtMesh::VA_SKELETON;
}
