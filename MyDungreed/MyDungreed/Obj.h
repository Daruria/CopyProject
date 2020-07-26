#pragma once
class CObj
{
public:
	CObj();
	virtual ~CObj();

protected:
	bool m_bLife;
	INFO m_tInfo;

public:
	void SetLife(const bool bLife) { m_bLife = bLife; }
	void SetInfo(float fx, float fy, int icx, int icy);
	void SetInfo(const POSITION& tPos);
	void SetInfo(const INFO& tInfo);
	void SetInfo(float fx, float fy);
	void SetInfo(int icx, int icy);
	void SetInfofx(float fx);
	void SetInfofy(float fy);
	void AddInfofx(float fdx);
	void AddInfofy(float fdy);

public:
	const INFO& GetInfo() const { return m_tInfo; }

public:
	const bool IsDie() const { return !m_bLife; }

public:
	virtual void Initialize() = 0;
	virtual int Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render(HDC hDC) = 0;
	virtual void Release() = 0;
};

