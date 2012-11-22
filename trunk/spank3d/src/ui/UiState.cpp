/*!
 * \file UiState.cpp
 * \date 9-20-2012 10:02:35
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/UiState.h>
#include <util/StringUtil.h>

UiState::TV_STRING UiState::m_vStateNames;
UiState::TV_UINT UiState::m_vStates;
UiState::TM_STRING_UINT_MAP UiState::m_StateNameMap;

tstring UiState::GetStateString(uint state)
{
	static bool s_Initialized = InitStateList();

	TV_STRING names;
	int numStates = int(m_vStates.size());
	for (int i = 0; i < numStates; ++i)
	{
		if ((state & m_vStates[i]) == m_vStates[i]) names.push_back(m_vStateNames[i]);
	}

	return StringUtil::joinString(names, _("|"));
}

uint UiState::GetStateValue(const tstring& stateName)
{
	static bool s_Initialized = InitStateNameMap();

	TV_STRING names;
	int numNames = StringUtil::splitString(names, stateName, _("|"));

	uint nState = 0;
	for (int i = 0; i < numNames; ++i)
	{
		TM_STRING_UINT_MAP::iterator itFound = m_StateNameMap.find(names[i]);
		if (itFound == m_StateNameMap.end()) continue;
		nState |= itFound->second;
	}

	return nState;
}

bool UiState::InitStateList()
{
	if (m_vStates.size() > 0) return false;

	m_vStates.push_back(STATE_DEFAULT);
	m_vStates.push_back(STATE_HOVER);
	m_vStates.push_back(STATE_DOWN);
	m_vStates.push_back(STATE_DISABLED);
	m_vStates.push_back(STATE_FOCUS);

	m_vStateNames.push_back(_("STATE_DEFAULT"));
	m_vStateNames.push_back(_("STATE_HOVER"));
	m_vStateNames.push_back(_("STATE_DOWN"));
	m_vStateNames.push_back(_("STATE_DISABLED"));
	m_vStateNames.push_back(_("STATE_FOCUS"));

	return true;
}

bool UiState::InitStateNameMap()
{
	if (m_StateNameMap.size() > 0) return false;

	m_StateNameMap.insert(std::make_pair(_("STATE_DEFAULT"), STATE_DEFAULT));
	m_StateNameMap.insert(std::make_pair(_("STATE_HOVER"), STATE_HOVER));
	m_StateNameMap.insert(std::make_pair(_("STATE_DOWN"), STATE_DOWN));
	m_StateNameMap.insert(std::make_pair(_("STATE_DISABLED"), STATE_DISABLED));
	m_StateNameMap.insert(std::make_pair(_("STATE_FOCUS"), STATE_FOCUS));

	return true;
}
