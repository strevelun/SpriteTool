#pragma once

class CBitmap;

class CCamera
{
	int m_xpos = 0, m_ypos = 0;

public:
	CCamera();
	~CCamera();

	int GetXPos() const { return m_xpos; }
	int GetYPos() const { return m_ypos; }

	void UpdateXPos(int _xpos) { m_xpos += _xpos; }
	void UpdateYPos(int _ypos) { m_ypos += _ypos; }
};

