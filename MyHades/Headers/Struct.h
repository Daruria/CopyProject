#pragma once

typedef struct tagInfo
{	
	D3DXVECTOR3	vScale;
	D3DXVECTOR3 vPos;	// 위치벡터 (수학에서는 위치벡터라는 개념이 없음)
	D3DXVECTOR3 vDir;	// 방향벡터
	D3DXVECTOR3 vLook;
	D3DXMATRIX matWorld; // 월드행렬 = 스(케일) * 자(전) * 이(동) * 공(전) * 부(모행렬)
}INFO;

typedef struct tagHitBox
{
	D3DXVECTOR3 vOffset;
	D3DXVECTOR3 vSize;
}HIT_BOX;

typedef struct tagTextureInfo
{
	LPDIRECT3DTEXTURE9 pTexture;
	D3DXIMAGE_INFO tImgInfo;
}TEX_INFO;

typedef struct tagTileInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	BYTE byDrawID;
	BYTE byOption;
}TILE_INFO;

typedef struct tagFrame
{
	//float fStartFrame;		//	시작 프레임
	float fCurFrame;		//	현재 프레임
	float fMaxFrame;		//	최대 프레임
	//DWORD	dwCurFrame;
	//DWORD	dwMaxFrame;
	float fFrameSpeed;		//	프레임 전환 속도.
	float fFrameTime;
	ANIM_TYPE eType;
}FRAME;

typedef struct tagSphere
{
	D3DXVECTOR3 vCenter;
	float	fRadius;
}SPHERE;

typedef struct tagPathInfo
{
	wstring wstrObjectKey;
	wstring wstrStateKey;
	wstring wstrRelative;
	int iImgCount;
}PATH_INFO;

typedef struct tagFileInfo
{
	wstring wstrObjectKey;
	wstring wstrStateKey;
	wstring wstrFileName;
	wstring wstrRelative;
}FILE_INFO;

typedef struct tagStatus
{
	int iHp;
	int iMaxHp;
	int iAttack;
}STATUS;

typedef struct tagPosition
{
	float fx;
	float fy;
} _POSITION;

typedef struct _tagLine
{
	_POSITION P1;
	_POSITION P2;
}LINE, *PLINE;

typedef struct tagTileForAstar
{
	D3DXVECTOR3 vPos;
	BYTE byOption;
	int iMyIndex;
	int iParentIndex;
	int iCost = 1;

	tagTileForAstar(TILE_INFO* pTileInfo)
	{
		vPos = pTileInfo->vPos;
		byOption = pTileInfo->byOption;
		iMyIndex = -1;
		iParentIndex = -1;
		iCost = 1;
	}
}ASTAR_TILE;