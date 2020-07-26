#pragma once
#include "Scene.h"
class CJusinLogo :
	public CScene
{
public:
	CJusinLogo();
	virtual ~CJusinLogo();

public:
	// Inherited via CScene
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	bool m_bIntroSound;

};

