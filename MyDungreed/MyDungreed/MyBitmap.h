#pragma once
class CMyBitmap
{
public:
	CMyBitmap();
	~CMyBitmap();

private:
	HDC m_hMemDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;

private:
	HDC m_hRotatedDC;
	HBITMAP m_hRotatedBitmap;
	HBITMAP m_hOldRotatedBitmap;

private:
	HBITMAP m_hTempBitmap;	//	For Rotation.

public:
	void Initialize();
	void Release();

public:
	HDC GetMemDC()				{ return m_hMemDC; }
	HDC GetRotatedDC()			{ return m_hRotatedDC; }
	HBITMAP GetBitmap()			{ return m_hBitmap; }
	HBITMAP GetRotatedBitmap()	{ return m_hRotatedBitmap; }

private:
	HBITMAP GetRotatedBitmap(HDC hdc, HBITMAP hBitmap, int dest_width, int dest_height, float angle);
	
public:
	void SetImage(const wchar_t* pPath);
	void RotateDC(const float fRadians);

};

