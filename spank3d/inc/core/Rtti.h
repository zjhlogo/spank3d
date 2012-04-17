/*!
 * \file Rtti.h
 * \date 4-17-2012 23:25:25
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __RTTI_H__
#define __RTTI_H__

#include <type/BaseType.h>

class Rtti
{
public:
	Rtti(const tstring& strClassName, const Rtti* pBaseRtti);
	~Rtti();

	const tstring& GetClassName() const;
	const Rtti* GetBaseRtti() const;

	bool IsType(const tstring& strClassName) const;
	bool IsDerived(const tstring& strClassName) const;

private:
	tstring m_strClassName;
	const Rtti* m_pBaseRtti;

};

class NoRtti
{
public:
	NoRtti() {};
	~NoRtti() {};

	static const Rtti* __RttiData() {return NULL;};

};

#define RTTI_DEF(name, basename)												\
static const Rtti* __RttiData()													\
{																				\
	static const Rtti s_Rtti(_(#name), basename::__RttiData());					\
	return &s_Rtti;																\
};																				\
virtual const Rtti* GetRtti()													\
{																				\
	return __RttiData();														\
}																				\

#endif // __RTTI_H__
