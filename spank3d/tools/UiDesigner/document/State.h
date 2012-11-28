/*!
 * \file State.h
 * \date 11-28-2012 13:28:19
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __STATE_H__
#define __STATE_H__

#include <wx/string.h>
#include <vector>
#include <map>

class State
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

	typedef std::map<wxString, unsigned int> TM_STRING_UINT_MAP;
	typedef std::vector<wxString> TV_STRING;
	typedef std::vector<unsigned int> TV_UINT;

public:
	static wxString GetStateString(unsigned int state);
	static unsigned int GetStateValue(const wxString& stateName);

private:
	static bool InitStateList();
	static bool InitStateNameMap();

private:
	static TV_UINT m_vStates;
	static TV_STRING m_vStateNames;
	static TM_STRING_UINT_MAP m_StateNameMap;

};

#endif // __STATE_H__
