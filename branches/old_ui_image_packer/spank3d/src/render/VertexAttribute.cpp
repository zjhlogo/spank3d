/*!
 * \file VertexAttribute.cpp
 * \date 25-07-2011 21:20:11
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <render/VertexAttribute.h>
#include <memory>
#include <gl/glew.h>

VertexAttribute::VertexAttribute(const ATTRIBUTE_ITEM* pAttrItems)
{
	m_nNumItems = 0;
	memset(m_AttributeItems, 0, sizeof(m_AttributeItems));
	CreateVertexAttribute(pAttrItems);
}

VertexAttribute::~VertexAttribute()
{
	// TODO: 
}

uint VertexAttribute::GetStride() const
{
	return m_AttributeItems[m_nNumItems].nOffset;
}

int VertexAttribute::GetNumAttributeItems() const
{
	return m_nNumItems;
}

const VertexAttribute::ATTRIBUTE_ITEM* VertexAttribute::GetAttributeItem(int nIndex) const
{
	if (nIndex < 0 || nIndex >= m_nNumItems) return NULL;
	return &m_AttributeItems[nIndex];
}

bool VertexAttribute::operator==(const VertexAttribute& VertexAttribute) const
{
	if (m_nNumItems != VertexAttribute.GetNumAttributeItems()) return false;

	for (int i = 0; i < m_nNumItems; ++i)
	{
		const ATTRIBUTE_ITEM* pAttrItem = VertexAttribute.GetAttributeItem(i);

		if (m_AttributeItems[i].nSize != pAttrItem->nSize) return false;
		if (m_AttributeItems[i].eItemType != pAttrItem->eItemType) return false;
		if (strcmp(m_AttributeItems[i].szParamName, pAttrItem->szParamName) != 0) return false;
	}

	return true;
}

uint VertexAttribute::GetGlType(ATTRIBUTE_ITEM_TYPE eType)
{
	static const GLenum s_GlType[NUM_AIT] = 
	{
		GL_FLOAT,			// AT_UNKNOWN,
		GL_BYTE,			// AT_BYTE,
		GL_UNSIGNED_BYTE,	// AT_UNSIGNED_BYTE,
		GL_SHORT,			// AT_SHORT,
		GL_UNSIGNED_SHORT,	// AT_UNSIGNED_SHORT,
		GL_FLOAT,			// AT_FLOAT,
		GL_FIXED,			// AT_FIXED,
	};

	if (eType < AIT_UNKNOWN || eType >= NUM_AIT) return GL_FLOAT;
	return s_GlType[eType];
}

uint VertexAttribute::GetAttributeItemSize(uint nSize, ATTRIBUTE_ITEM_TYPE eType)
{
	static const int s_ItemSizes[NUM_AIT] =
	{
		0, // AIT_UNKNOWN,
		1, // AIT_BYTE,
		1, // AIT_UNSIGNED_BYTE,
		2, // AIT_SHORT,
		2, // AIT_UNSIGNED_SHORT,
		4, // AIT_FLOAT,
		4, // AIT_FIXED,
	};

	if (eType <= AIT_UNKNOWN || eType >= NUM_AIT) return 0;
	return nSize*s_ItemSizes[eType];
}

VertexAttribute::ATTRIBUTE_ITEM_TYPE VertexAttribute::GetAttributeItemType(const tstring& strType)
{
	if (strType == _("AIT_UNKNOWN")) return AIT_UNKNOWN;
	else if (strType == _("AIT_BYTE")) return AIT_BYTE;
	else if (strType == _("AIT_UNSIGNED_BYTE")) return AIT_UNSIGNED_BYTE;
	else if (strType == _("AIT_SHORT")) return AIT_SHORT;
	else if (strType == _("AIT_UNSIGNED_SHORT")) return AIT_UNSIGNED_SHORT;
	else if (strType == _("AIT_FLOAT")) return AIT_FLOAT;
	else if (strType == _("AIT_FIXED")) return AIT_FIXED;
	return AIT_UNKNOWN;
}

bool VertexAttribute::CreateVertexAttribute(const ATTRIBUTE_ITEM* pAttrItems)
{
	int nNumItems = 0;
	const ATTRIBUTE_ITEM* pCurrItem = pAttrItems;
	while (pCurrItem && pCurrItem->eItemType != AIT_UNKNOWN)
	{
		nNumItems++;
		pCurrItem++;
	}

	if (nNumItems <= 0 || nNumItems > MAX_ATTRIBUTE_ITEMS) return false;

	m_nNumItems = nNumItems;
	uint nOffset = 0;
	for (int i = 0; i < nNumItems; ++i)
	{
		m_AttributeItems[i].nSize = pAttrItems[i].nSize;
		m_AttributeItems[i].eItemType = pAttrItems[i].eItemType;
		m_AttributeItems[i].nOffset = nOffset;
		strncpy_s(m_AttributeItems[i].szParamName, pAttrItems[i].szParamName, MAX_ATTRIBUTE_NAME_LENGTH);
		nOffset += GetAttributeItemSize(m_AttributeItems[i].nSize, m_AttributeItems[i].eItemType);
	}
	m_AttributeItems[m_nNumItems].nSize = 0;
	m_AttributeItems[m_nNumItems].eItemType = AIT_UNKNOWN;
	m_AttributeItems[m_nNumItems].nOffset = nOffset;
	m_AttributeItems[m_nNumItems].szParamName[0] = '\0';

	SetOk(true);
	return true;
}
