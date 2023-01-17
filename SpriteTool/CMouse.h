#pragma once

#include <d2d1.h>

class CMouse
{
	int m_mouseX = 0, m_mouseY = 0;
	int m_startMouseX = 0, m_startMouseY = 0;
	bool m_isClicked = false;


public:
	void UpdateMousePos(int _xpos, int _ypos) { if (m_isClicked) { m_mouseX = _xpos, m_mouseY = _ypos; }}
	void UpdateMouseStartPos(int _xpos, int _ypos) { m_startMouseX = _xpos; m_startMouseY = _ypos; }
	void UpdateClickState(bool _isClicked) { m_isClicked = _isClicked; }
	void Render(ID2D1HwndRenderTarget* _pRenderTarget, ID2D1SolidColorBrush* _pBlackBrush)
	{
		if(m_isClicked)
			_pRenderTarget->DrawRectangle(
				D2D1::RectF(m_startMouseX, m_startMouseY, m_mouseX, m_mouseY), _pBlackBrush);
	}
};

