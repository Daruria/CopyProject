#pragma once
#include "GameObject.h"
//#include "ImageObject.h"
class CImageObject;
class CTerrain :
	public CGameObject
{
public:
	enum TILE_OPTION { 
		NO_OPTION, NO_MOVE, 
		NO_LT_PUSH, NO_RT_PUSH, NO_LB_PUSH, NO_RB_PUSH, 
		NO_UP_PUSH, NO_RIGHT_PUSH, NO_DOWN_PUSH, NO_LEFT_PUSH,
		NO_LTR_PUSH, NO_TRB_PUSH, NO_RBL_PUSH, NO_BLT_PUSH,
		OPTION_END };

private:
	CTerrain();

public:
	virtual ~CTerrain();

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

	MACRO_CREATE(CTerrain)

public:
	virtual float GetLayerStd() override { return m_tInfo.vPos.y; }
	virtual void Die() override {};

public:
	int GetTileIndex(const D3DXVECTOR3& vPos);
	vector<TILE_INFO*>* GetVecTile() { return &m_vecTile; };

public:
	void TileChange(const D3DXVECTOR3& vPos, const BYTE& byDrawID, const BYTE& byOption = 0);

private:
	bool IsPicking(const D3DXVECTOR3& vPos, int iIndex);
	bool IsPicking(const D3DXVECTOR3& vScrollPos, const D3DXVECTOR3& vMousePos);
public:
	HRESULT AddImageObject(CImageObject* pImgObj);

public:
	void SaveData(const TCHAR* pFilePath);
	void SaveData(HANDLE hFile, const TCHAR* pFilePath);
	void LoadData(const TCHAR* pFilePath);
	void LoadData(HANDLE hFile, const TCHAR* pFilePath);

private:
	vector<TILE_INFO*>	m_vecTile;
	list<CImageObject*>	m_ImgList;

	CObjectMgr*		m_pObjectMgr;
	CKeyMgr*		m_pKeyMgr;

	int		m_iChangeDraw;
	bool	m_bShowOption;
};

