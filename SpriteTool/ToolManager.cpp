#include "ToolManager.h"
#include "CBitmap.h"
#include "CAnimationClip.h"

ToolManager* ToolManager::m_inst = nullptr;

ToolManager::ToolManager()
{
	m_bitmap = new CBitmap();
	m_animClip = new CAnimationClip();
}

ToolManager::~ToolManager()
{
	delete m_bitmap;
	delete m_animClip;
}
