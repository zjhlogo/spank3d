/*!
 * \file UiState.h
 * \date 9-20-2012 9:57:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __UISTATE_H__
#define __UISTATE_H__

#include "../type/BaseType.h"
#include <map>
#include <vector>

class UiState
{
public:
	enum CONST_DEFINE
	{
		STATE_DEFAULT		= 0x00000001,
		STATE_HOVER			= 0x00000002,
		STATE_DOWN			= 0x00000004,
		STATE_DISABLED		= 0x00000008,
		STATE_FOCUS			= 0x00000010,

		STATE_MASK			= STATE_DEFAULT|STATE_HOVER|STATE_DOWN|STATE_DISABLED|STATE_FOCUS,
	};

	typedef std::map<tstring, uint> TM_STRING_UINT_MAP;
	typedef std::vector<tstring> TV_STRING;
	typedef std::vector<uint> TV_UINT;

public:
	static tstring GetStateString(uint state);
	static uint GetStateValue(const tstring& stateName);

private:
	static bool InitStateList();
	static bool InitStateNameMap();

private:
	static TV_UINT m_vStates;
	static TV_STRING m_vStateNames;
	static TM_STRING_UINT_MAP m_StateNameMap;

};

#endif // __UISTATE_H__
