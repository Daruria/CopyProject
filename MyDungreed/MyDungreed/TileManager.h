#pragma once

class CTile;
class CTileManager
{
public:
	CTileManager();
	~CTileManager();

	DECLARE_SINGLE(CTileManager)

private:
	vector<CTile*> m_vecTile;

public:
	vector<CTile*>* GetpVecTile() { return &m_vecTile; }

public:
	const int	GetTileIndex(const INFO& tInfo);
	const int	GetTileIndex(int iX, int iY);


public:
	void SaveData();
	void LoadData();
	void LoadData(const wchar_t* pPath);

public:
	void Initialize();
	void Render(HDC hDC);
	void RenderForEdit(HDC hDC);
	void Release();
};

