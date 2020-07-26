#pragma once

class CSelectTile
{
public:
	CSelectTile();
	~CSelectTile();

private:
	int			m_iTileCX;
	int			m_iTileCY;
	int			m_iTileTotalCX;
	int			m_iTileTotalCY;
	string		m_strKey;
	string		m_strSelectBoxKey;
	POINT		m_tMousePos;
	TILE_TYPE	m_eType;

public:
	void SetKey(const string& strKey) { m_strKey = strKey; }
	void SetTileCX(const int iTileCX) { m_iTileCX = iTileCX; }
	void SetTileCY(const int iTileCY) { m_iTileCY = iTileCY;}
	void SetTileTotalCX(const int iTileTotalCX) { m_iTileTotalCX = iTileTotalCX; }
	void SetTileTotalCY(const int iTileTotalCY) { m_iTileTotalCY = iTileTotalCY; }

public:
	const string GetKey() { return m_strKey; }
	const int	GetTileCX() { return m_iTileCX; }
	const int	GetTileCY() { return m_iTileCY;}
	const int	GetTileTotalCX() { return m_iTileTotalCX; }
	const int	GetTileTotalCY() { return m_iTileTotalCY; }

public:
	const int	SelectID();

private:
	const POINT ScreenToIndex();

public:
	void Initialize(const string& strKey, const wchar_t* pPath, int iTileCX, int iTileCY, int iTileTotalCX, int iTileTotalCY);
	int Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();
};

