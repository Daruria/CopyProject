#pragma once

#ifndef __DEFINES_H__

#define SAFE_DELETE(p)			if(p) {delete p; p = nullptr;}
#define SAFE_DELETE_ARRAY(p)	if(p) {delete[] p; p = nullptr;}

#define DECLARE_SINGLE(Name)\
	private:\
		static Name* m_pInst;\
	public:\
		static Name* GetInst()\
		{\
			if (!m_pInst)\
				m_pInst = new Name;\
			return m_pInst;\
		}\
		static void DestroyInst()\
		{\
			SAFE_DELETE(m_pInst);\
		}
#define DEFINITION_SINGLE(Name) Name* Name::m_pInst = nullptr

#define GET_SINGLE(Name)		Name::GetInst()
#define DESTROY_SINGLE(Name)	Name::DestroyInst()

#define WINCX 1024
#define WINCY 768

//#define WINCX 800
//#define WINCY 600

#define TILECX 64
#define TILECY 64

#define TILEX	130	//	a Number of TileX
#define TILEY	60	//	a Number of TileY

#define TILEMAPX 32	//	Resource Map Tile Number
#define TILEMAPY 32	

#define GRAVITY	9.8f
#define PI 3.14159265358979f

#define TRANS_VALUE RGB(255, 0, 255)
#define CAN_NOT_FOUND_BITMAP(hDC, iStartX, iStartY, iCX, iCY)	CMath::NotFoundImage(hDC, iStartX, iStartY, iCX, iCY)
#define INSERT_BITMAP(strKey, pPath)		GET_SINGLE(CBitmapManager)->InsertBitmap(strKey, pPath)
#define FIND_BITMAP_DC(strKey)				GET_SINGLE(CBitmapManager)->FindBitmapDC(strKey)
#define FIND_ROTATED_DC(strKey, fRadians)	GET_SINGLE(CBitmapManager)->FindRotatedDC(strKey, fRadians)
#define GET_BITMAP_SIZE(strKey)				GET_SINGLE(CBitmapManager)->GetBitmapSize(strKey)

#define KEYPRESS(iKey)	GET_SINGLE(CKeyManager)->KeyPressing(iKey)
#define KEYDOWN(iKey)	GET_SINGLE(CKeyManager)->KeyDown(iKey)
#define KEYUP(iKey)		GET_SINGLE(CKeyManager)->KeyUp(iKey)

#define FONT_STYLE_X1 0x01
#define FONT_STYLE_X2 0x02
#define FONT_STYLE_X3 0x03
#define FONT_STYLE_X4 0x04

#define FONT_STYLE_SIZE 0x0F

#define FONT_STYLE_NORMAL	0x10
#define FONT_STYLE_EDGE		0x20

#define __DEFINES_H__

#endif
