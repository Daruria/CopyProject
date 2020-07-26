#pragma once
class CMiniMap
{
public:
	CMiniMap();
	~CMiniMap();

public:
	void Initialize();
	int Update();
	void Render(HDC hDC);

private:
	void Release();

private:
	float	m_fRatio;
	INFO	m_tInfo;

public:
	void SetRatio(float fRatio)							{ m_fRatio = fRatio; }
	void SetInfo(float fx, float fy)					{ m_tInfo.fx = fx; m_tInfo.fy = fy; }
	void SetInfo(int icx, int icy)						{ m_tInfo.icx = icx; m_tInfo.icy = icy; }
	void SetInfo(float fx, float fy, int icx, int icy)	{ m_tInfo.fx = fx; m_tInfo.fy = fy; m_tInfo.icx = icx; m_tInfo.icy = icy; }

public:
	void CreateMiniMap();
	
private:
	void RenderObject(HDC hDC, const string& strKey, OBJECT_TYPE eType);


};

