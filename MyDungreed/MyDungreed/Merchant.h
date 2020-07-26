#pragma once
#include "NPC.h"

class CShopWindow;
class CMerchant :
	public CNPC
{
public:
	CMerchant();
	virtual ~CMerchant();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	virtual void Interaction();

private:
	bool			m_bShowItem;
	CShopWindow*	m_pShopWindow;

};

