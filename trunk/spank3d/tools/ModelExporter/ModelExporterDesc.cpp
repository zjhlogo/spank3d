/*!
 * \file ModelExporterDesc.cpp
 * \date 1-3-2010 21:01:34
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "ModelExporter.h"
#include "ModelExporterDesc.h"

ModelExporterDesc::ModelExporterDesc()
{
	// TODO: 
}

ModelExporterDesc::~ModelExporterDesc()
{
	// TODO: 
}

int ModelExporterDesc::IsPublic()
{
	return 1;
}

void* ModelExporterDesc::Create(BOOL loading /*= FALSE*/)
{
	return new ModelExporter();
}

const TCHAR* ModelExporterDesc::ClassName()
{
	return _T("ModelExporterDesc");
}

SClass_ID ModelExporterDesc::SuperClassID()
{
	return SCENE_EXPORT_CLASS_ID;
}

Class_ID ModelExporterDesc::ClassID()
{
	return Class_ID(0x6ebf4c58, 0x796f25f4);
}

const TCHAR* ModelExporterDesc::Category()
{
	return _T("scene export");
}
