#pragma once

class CBitmap;
class CAnimationClip;

class ToolManager
{
	static ToolManager* m_inst;

	CBitmap* m_bitmap;
	CAnimationClip* m_animClip;

	ToolManager();
	~ToolManager();
public:
	static ToolManager* GetInst()
	{
		if (!m_inst) m_inst = new ToolManager;
		return m_inst;
	}

	static void DestroyInst()
	{
		if (m_inst)
		{
			delete m_inst;
			m_inst = nullptr;
		}
	}

	CBitmap* GetBitmap() const { return m_bitmap; }
	CAnimationClip* GetAnimClip() const { return m_animClip; }
};

