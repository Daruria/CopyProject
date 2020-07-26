#pragma once
class CGameObject;
class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();
	
public:
	static const D3DXVECTOR3& GetScrollPos() { return m_vScrollPos; }

public:
	static void MoveScrollPos(const D3DXVECTOR3& vMove) { m_vScrollPos += vMove; }
	static void SetScrollPos(float fx, float fy) { m_vScrollPos = {fx, fy, 0.f}; }

public:
	static void Scrolling(CGameObject* pTargetObj);
	static void Scrolling(const D3DXVECTOR3& vPos);
	static void Shaking();

private:
	static D3DXVECTOR3 m_vScrollPos;
	static D3DXVECTOR3 m_vScrollShake;

	static bool	 m_bShaking;
	static float m_fShakeTime;
};

