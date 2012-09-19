/*!
 * \file IStyle.h
 * \date 9-19-2012 22:34:09
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __ISTYLE_H__
#define __ISTYLE_H__

#include "../../core/IObject.h"

class IStyle : public IObject
{
public:
	RTTI_DEF(IStyle, IObject);

	IStyle(const tstring& id);
	virtual ~IStyle();

	const tstring& GetId() const;

private:
	tstring m_strId;

};
#endif // __ISTYLE_H__
