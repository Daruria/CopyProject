#pragma once
class CMyBitmap;
class CBitmapManager
{
public:
	CBitmapManager();
	~CBitmapManager();

	DECLARE_SINGLE(CBitmapManager)

public:
	void Initialize();
	void Release();

private:
	unordered_map<string, CMyBitmap*> m_ImageMap;

public:
	void InsertBitmap(const string& strKey, const wchar_t* pPath);
	CMyBitmap* FindBitmap(const string& strKey);
	HDC	FindBitmapDC(const string& strKey);
	HDC FindRotatedDC(const string& strKey, const float fRadians);
	SIZE GetBitmapSize(const string& strKey);

};

