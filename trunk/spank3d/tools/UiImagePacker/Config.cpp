/*!
 * \file Config.cpp
 * \date 2-17-2012 10:20:11
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Config.h"

Config::Config()
{
	m_strAbout = _("UiImagePacker version 1.1.0\nauthor: zjhlogo\ncontact: zjhlogo@gmail.com\n\nspecial thanks to:\nQingzhongZhang (artist, give me some advices and offer some icons)\nBaoqingWu (programmer, assist me codeing some models)");
}

Config::~Config()
{
	// TODO: 
}

Config& Config::GetInstance()
{
	static Config s_Config;
	return s_Config;
}

const wxString& Config::GetAbout()
{
	return m_strAbout;
}
