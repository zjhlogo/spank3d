/*!
 * \file Spank3d.h
 * \date 4-15-2012 11:51:02
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __SPANK3D_H__
#define __SPANK3D_H__

#include "core/Core.h"
#include "core/IResMgr.h"
#include "core/IDevice.h"
#include "core/IApp.h"

#include "ui/IUiRenderer.h"
#include "ui/IUiResMgr.h"
#include "ui/IUiSystemMgr.h"
#include "ui/IUiInputMgr.h"

extern Core* g_pCore;
extern IResMgr* g_pResMgr;
extern IDevice* g_pDevice;
extern IApp* g_pApp;

extern IUiRenderer* g_pUiRenderer;
extern IUiResMgr* g_pUiResMgr;
extern IUiSystemMgr* g_pUiSystemMgr;
extern IUiInputMgr* g_pUiInputMgr;

#endif // __SPANK3D_H__
