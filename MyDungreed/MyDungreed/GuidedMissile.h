#pragma once
#include "Bullet.h"

class CGuidedMissile :
	public CBullet
{
public:
	CGuidedMissile();
	virtual ~CGuidedMissile();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	virtual void Dying(float fx, float fy);

private:
	void CreateTail();
	void ChangeAngle();

private:
	int		m_iTailFrame;
};

