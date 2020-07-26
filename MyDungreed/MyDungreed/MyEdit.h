#pragma once

#include "Scene.h"
class CMyButton;
class CSelectTile;
class CMyEdit : public CScene
{
public:
	CMyEdit();
	virtual ~CMyEdit();

private:
	bool m_bShowButton;
	bool m_bSelectTile;
	bool m_bSelectOptionTile;
	int m_iDrawID;
	int m_iOptionID;
	POINT m_tButtonPos;

private:
	CMyButton*		m_pTileSelectButton;
	CMyButton*		m_pOptionSelectButton;
	CSelectTile*	m_pTileSelect;
	CSelectTile*	m_pOptionSelect;

private:
	void ChangeTile();
	void ShowButtons();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();
};

