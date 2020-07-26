#pragma once

typedef struct tagInfo
{	
	D3DXVECTOR3	vScale;
	D3DXVECTOR3 vPos;	// ��ġ���� (���п����� ��ġ���Ͷ�� ������ ����)
	D3DXVECTOR3 vDir;	// ���⺤��
	D3DXVECTOR3 vLook;
	D3DXMATRIX matWorld; // ������� = ��(����) * ��(��) * ��(��) * ��(��) * ��(�����)
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
	//float fStartFrame;		//	���� ������
	float fCurFrame;		//	���� ������
	float fMaxFrame;		//	�ִ� ������
	//DWORD	dwCurFrame;
	//DWORD	dwMaxFrame;
	float fFrameSpeed;		//	������ ��ȯ �ӵ�.
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