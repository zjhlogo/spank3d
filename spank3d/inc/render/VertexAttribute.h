/*!
 * \file VertexAttribute.h
 * \date 25-07-2011 21:18:23
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __VERTEXATTRIBUTE_H__
#define __VERTEXATTRIBUTE_H__

#include "../core/IObject.h"

class VertexAttribute : public IObject
{
public:
	enum CONST_DEFINE
	{
		MAX_ATTRIBUTE_NAME_LENGTH = 32,
		MAX_ATTRIBUTE_ITEMS = 8,
	};

	enum ATTRIBUTE_ITEM_TYPE
	{
		AIT_UNKNOWN = 0,
		AIT_BYTE,
		AIT_UNSIGNED_BYTE,
		AIT_SHORT,
		AIT_UNSIGNED_SHORT,
		AIT_FLOAT,
		AIT_FIXED,
		NUM_AIT,
	};

	typedef struct ATTRIBUTE_ITEM_tag
	{
		uint nSize;
		ATTRIBUTE_ITEM_TYPE eItemType;
		uint nOffset;
		char szParamName[MAX_ATTRIBUTE_NAME_LENGTH];
	} ATTRIBUTE_ITEM;

public:
	RTTI_DEF(VertexAttribute, IObject);

	VertexAttribute(const ATTRIBUTE_ITEM* pAttrItems);
	virtual ~VertexAttribute();

	virtual uint GetStride() const;
	virtual int GetNumAttributeItems() const;
	virtual const ATTRIBUTE_ITEM* GetAttributeItem(int nIndex) const;
	virtual bool operator==(const VertexAttribute& VertexAttribute) const;

	static uint GetGlType(ATTRIBUTE_ITEM_TYPE eType);
	static uint GetAttributeItemSize(uint nSize, ATTRIBUTE_ITEM_TYPE eType);

private:
	bool CreateVertexAttribute(const ATTRIBUTE_ITEM* pAttrItems);

private:
	int m_nNumItems;
	ATTRIBUTE_ITEM m_AttributeItems[MAX_ATTRIBUTE_ITEMS+1];

};
#endif // __VERTEXATTRIBUTE_H__
