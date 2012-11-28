/*!
 * \file State.cpp
 * \date 11-28-2012 13:28:23
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "State.h"
#include "../utils/StringUtil.h"

State::TM_STRING_UINT_MAP State::m_StateNameMap;
State::TV_STRING State::m_vStateNames;
State::TV_UINT State::m_vStates;

wxString State::GetStateString(unsigned int state)
{
	static bool s_Initialized = InitStateList();

	TV_STRING names;
	int numStates = int(m_vStates.size());
	for (int i = 0; i < numStates; ++i)
	{
		if ((state & m_vStates[i]) == m_vStates[i]) names.push_back(m_vStateNames[i]);
	}

	return StringUtil::JoinString(names, wxT("|"));
}

unsigned int State::GetStateValue(const wxString& stateName)
{
	static bool s_Initialized = InitStateNameMap();

	TV_STRING names;
	int numNames = StringUtil::SplitString(names, stateName, wxT("|"));

	unsigned int nState = 0;
	for (int i = 0; i < numNames; ++i)
	{
		TM_STRING_UINT_MAP::iterator itFound = m_StateNameMap.find(names[i]);
		if (itFound == m_StateNameMap.end()) continue;
		nState |= itFound->second;
	}

	return nState;
}

bool State::InitStateList()
{
	if (m_vStates.size() > 0) return false;

	m_vStates.push_back(STATE_DEFAULT);
	m_vStates.push_back(STATE_HOVER);
	m_vStates.push_back(STATE_DOWN);
	m_vStates.push_back(STATE_DISABLED);
	m_vStates.push_back(STATE_FOCUS);

	m_vStateNames.push_back(wxT("STATE_DEFAULT"));
	m_vStateNames.push_back(wxT("STATE_HOVER"));
	m_vStateNames.push_back(wxT("STATE_DOWN"));
	m_vStateNames.push_back(wxT("STATE_DISABLED"));
	m_vStateNames.push_back(wxT("STATE_FOCUS"));

	return true;
}

bool State::InitStateNameMap()
{
	if (m_StateNameMap.size() > 0) return false;

	m_StateNameMap.insert(std::make_pair(wxT("STATE_DEFAULT"), STATE_DEFAULT));
	m_StateNameMap.insert(std::make_pair(wxT("STATE_HOVER"), STATE_HOVER));
	m_StateNameMap.insert(std::make_pair(wxT("STATE_DOWN"), STATE_DOWN));
	m_StateNameMap.insert(std::make_pair(wxT("STATE_DISABLED"), STATE_DISABLED));
	m_StateNameMap.insert(std::make_pair(wxT("STATE_FOCUS"), STATE_FOCUS));

	return true;
}
