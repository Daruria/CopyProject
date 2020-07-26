#pragma once
#include "Item.h"
class CAccessory :
	public CItem
{
public:
	CAccessory();
	virtual ~CAccessory();

public:
	// Inherited via CItem
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	


};

