#pragma once
#include "UISlot.h"
class CShopSlot :
	public CUISlot
{
public:
	CShopSlot();
	virtual ~CShopSlot();

public:
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();




};

