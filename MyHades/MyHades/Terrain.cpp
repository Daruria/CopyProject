#include "stdafx.h"
#include "Terrain.h"
#include "MultiTexture.h"
#include "ImageObject.h"
#include "AnimatedObject.h"
#include "NonAnimatedObject.h"
#include "Functor.h"

CTerrain::CTerrain():
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_bShowOption(false)
{
}


CTerrain::~CTerrain()
{
	Release();
}

int CTerrain::GetTileIndex(const D3DXVECTOR3 & vPos)
{
	int iXIndex = vPos.x / TILECX;
	int iYIndex = vPos.y / (TILECY * 0.5f);

	if (iXIndex < 0 || iXIndex >= TILEX ||
		iYIndex < 0 || iYIndex >= TILEY)
		return 0;

	int iYCheckStart = iYIndex - 1 < 0 ? iYIndex : iYIndex - 1;
	int iYCheckEnd = iYIndex + 1 >= TILEX ? iYIndex : iYIndex + 1;

	int iXCheckStart = iXIndex - 1 < 0 ? iXIndex : iXIndex - 1;
	int iXCheckEnd = iXIndex + 1 >= TILEX ? iXIndex : iXIndex + 1;

	for (int y = iYCheckStart; y <= iYCheckEnd; ++y)
	{
		for (int x = iXCheckStart; x <= iXCheckEnd; ++x)
		{
			int iIndex = y * TILEX + x;
			//if (IsPicking(m_vecTile[iIndex]->vPos, vPos))
			if (IsPicking(vPos, iIndex))
			{
				return iIndex;
			}
		}
	}

	return 0;
}

int CTerrain::Update()
{
	m_pObjectMgr->AddLayer(m_eRenderType, this);
	for (auto& pImgObj : m_ImgList)
		pImgObj->Update();
	
	return 0;
}

void CTerrain::LateUpdate()
{
	for (auto& pImgObj : m_ImgList)
		pImgObj->LateUpdate();
}

void CTerrain::Render()
{
	D3DXMATRIX matScale, matTrans;

	const TEX_INFO* pTexInfo = nullptr;
	float fCenterX = 0.f, fCenterY = 0.f;

	int iLeftEdge	= static_cast<int>(CScrollMgr::GetScrollPos().x);
	int iRightEdge	= static_cast<int>(CScrollMgr::GetScrollPos().x) + WINCX;
	int iTopEdge	= static_cast<int>(CScrollMgr::GetScrollPos().y);
	int iBottomEdge = static_cast<int>(CScrollMgr::GetScrollPos().y) + WINCY;

	m_ImgList.sort(CImageObjectSort());
	for (auto& pImgObj : m_ImgList)
	{
		pTexInfo = pImgObj->GetTexInfo();
		NULL_CHECK_CONTINUE(pTexInfo);

		if (iRightEdge < pImgObj->GetInfo().vPos.x - pTexInfo->tImgInfo.Width * 0.5f)	//	left of img
			continue;
		
		if (iBottomEdge < pImgObj->GetInfo().vPos.y - pTexInfo->tImgInfo.Height * 0.5f)	//	top of img
			continue;

		if (iLeftEdge > pImgObj->GetInfo().vPos.x + pTexInfo->tImgInfo.Width* 0.5f)		//	right of img
			continue;

		if (iTopEdge > pImgObj->GetInfo().vPos.y + pTexInfo->tImgInfo.Height * 0.5f)	//	bottom of img
			continue;

		pImgObj->Render();
	}

	if (!m_bShowOption)
		return;

	int iXIndexStart = iLeftEdge / TILECX;
	int iXIndexEnd = iRightEdge / TILECX + 2;

	int iYIndexStart = iTopEdge / (TILECY / 2);
	int iYIndexEnd = iBottomEdge / (TILECY / 2) + 2;

	for (int y = iYIndexStart; y < iYIndexEnd; ++y)
	{
		if (y < 0 || y >= TILEY)
			continue;
		for (int x = iXIndexStart; x < iXIndexEnd; ++x)
		{
			if (x < 0 || x >= TILEX)
				continue;

			int iIndex = y * TILEX + x;

			pTexInfo = m_pTextureMgr->GetTexInfo(L"Terrain", L"Option", m_vecTile[iIndex]->byOption);
			NULL_CHECK_CONTINUE(pTexInfo);

			float fRatioX = (float)TILECX / pTexInfo->tImgInfo.Width;
			float fRatioY = (float)TILECY / pTexInfo->tImgInfo.Height;

			D3DXMatrixScaling(&matScale,
				m_vecTile[iIndex]->vSize.x * fRatioX,
				m_vecTile[iIndex]->vSize.y * fRatioY,
				m_vecTile[iIndex]->vSize.z);
			D3DXMatrixTranslation(&matTrans,
				m_vecTile[iIndex]->vPos.x - CScrollMgr::GetScrollPos().x,
				m_vecTile[iIndex]->vPos.y - CScrollMgr::GetScrollPos().y,
				m_vecTile[iIndex]->vPos.z);

			fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

			m_pDeviceMgr->GetSprite()->SetTransform(&(matScale * matTrans));
			m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			pTexInfo = m_pTextureMgr->GetTexInfo(L"Terrain", L"Tile", m_vecTile[iIndex]->byDrawID);
			NULL_CHECK_CONTINUE(pTexInfo);

			m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

HRESULT CTerrain::Initialize()
{
	m_vecTile.reserve(TILEX * TILEY);

	TILE_INFO* pTile = nullptr;
	float fX = 0.f, fY = 0.f;

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			fX = (j* TILECX) + (i % 2) * (TILECX * 0.5f);
			fY = i * (TILECY * 0.5f);

			pTile = new TILE_INFO;
			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { 1.f, 1.f, 0.f };
			pTile->byDrawID = 2;
			pTile->byOption = 0;

			m_vecTile.push_back(pTile);
		}
	}

	m_eRenderType = RENDER_TERRAIN;

#ifdef _DEBUG
	m_bShowOption = true;
#endif // _DEBUG

	return S_OK;
}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE_INFO*>);
	m_vecTile.clear();

	for_each(m_ImgList.begin(), m_ImgList.end(), SafeDelete<CImageObject*>);
	m_ImgList.clear();
}

void CTerrain::TileChange(const D3DXVECTOR3& vPos, const BYTE& byDrawID, const BYTE& byOption)
{
	int iIndex = GetTileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = byDrawID;
	m_vecTile[iIndex]->byOption = byOption;
}

bool CTerrain::IsPicking(const D3DXVECTOR3 & vPos, int iIndex)
{
	// 내적을 이용한 방식 (데카르트 기준)
	// 마름모 꼭지점 설정 12시, 3시, 6시, 9시 순서
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + TILECY * 0.5f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - TILECY * 0.5f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f)
	};

	// 각 꼭지점으로부터 시계방향으로 방향벡터들을 구함.
	D3DXVECTOR3 vDir[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3],
	};

	// 위에서 구한 방향벡터들의 각 법선벡터들을 구한다.
	D3DXVECTOR3 vNormal[4] = {};

	for (int i = 0; i < 4; ++i)
		D3DXVec3Cross(&vNormal[i], &D3DXVECTOR3(0.f, 0.f, 1.f), &vDir[i]);

	// 각 꼭지점에서부터 마우스위치로의 방향벡터를 구한다.
	D3DXVECTOR3 vMouseDir[4] =
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	// vMouseDir과 vNormal을 각각 내적해서 모두 둔각(음수)이 나오면 TRUE!
	for (int i = 0; i < 4; ++i)
	{
		if (0 < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

bool CTerrain::IsPicking(const D3DXVECTOR3 & vScrollPos, const D3DXVECTOR3 & vMousePos)
{
	if (vMousePos.x < vScrollPos.x - TILECX * 0.5f ||
		vMousePos.x > vScrollPos.x + TILECX * 0.5f ||
		vMousePos.y < vScrollPos.y - TILECY * 0.5f ||
		vMousePos.y > vScrollPos.y + TILECY * 0.5f)
		return false;

	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3{ vScrollPos.x, vScrollPos.y - TILECY * 0.5f, 0.f },	//	Top
		D3DXVECTOR3{ vScrollPos.x + TILECX * 0.5f, vScrollPos.y, 0.f },	//	Right
		D3DXVECTOR3{ vScrollPos.x, vScrollPos.y + TILECY * 0.5f, 0.f },	//	Bottom
		D3DXVECTOR3{ vScrollPos.x - TILECX * 0.5f, vScrollPos.y, 0.f }	//	Left
	};

	D3DXVECTOR3 vDir[4] = {};
	for (int i = 0; i < 4; ++i)
		vDir[i] = vPoint[i + 1] - vPoint[i];

	D3DXVECTOR3 vPointToMouse[4] = {};

	for (int i = 0; i < 4; ++i)
		vPointToMouse[i] = vMousePos - vPoint[i];

	D3DXVECTOR3 vNormalDir[4] = {};
	for (int i = 0; i < 4; ++i)
		D3DXVec3Cross(&vNormalDir[i], &vDir[i], &D3DXVECTOR3{ 0.f, 0.f, 1.f });

	for (int i = 0; i < 4; ++i)
	{
		float fDot = D3DXVec3Dot(&vNormalDir[i], &vPointToMouse[i]);

		if (fDot > 0.f)
			return false;
	}

	return true;
}

HRESULT CTerrain::AddImageObject(CImageObject * pImgObj)
{
	NULL_CHECK_MSG_RETURN(pImgObj, L"ImageObject push Failed", E_FAIL);
	m_ImgList.push_back(pImgObj);

	return S_OK;
}

void CTerrain::SaveData(const TCHAR * pFilePath)
{
	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_WRITE, 0, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	SaveData(hFile, pFilePath);

	CloseHandle(hFile);
}

void CTerrain::SaveData(HANDLE hFile, const TCHAR * pFilePath)
{
	DWORD dwBytes = 0;

	for (size_t i = 0; i < m_vecTile.size(); ++i)
		WriteFile(hFile, m_vecTile[i], sizeof(TILE_INFO), &dwBytes, nullptr);

	float fSort = 0.f, fDegree = 0.f;
	bool bRenderPush = false;
	INFO tInfo;
	int iSize = m_ImgList.size();
	WriteFile(hFile, &iSize, sizeof(iSize), &dwBytes, nullptr);

	for (auto& pImgObj : m_ImgList)
	{
		//	Info Save
		tInfo = pImgObj->GetInfo();
		WriteFile(hFile, &tInfo, sizeof(INFO), &dwBytes, nullptr);

		//	bRenderPush
		bRenderPush = pImgObj->GetRenderListPush();
		WriteFile(hFile, &bRenderPush, sizeof(bool), &dwBytes, nullptr);

		//	fSort Save
		fSort = pImgObj->GetSort();
		WriteFile(hFile, &fSort, sizeof(float), &dwBytes, nullptr);

		//	fDegree Save
		fDegree = pImgObj->GetDegree();
		WriteFile(hFile, &fDegree, sizeof(float), &dwBytes, nullptr);

		//	ObjectKey Save
		string strTemp(pImgObj->GetObjectKey().begin(), pImgObj->GetObjectKey().end());
		BYTE bySize = static_cast<BYTE>(strTemp.size());
		WriteFile(hFile, &bySize, sizeof(bySize), &dwBytes, nullptr);
		for (BYTE i = 0; i < bySize; ++i)
			WriteFile(hFile, &strTemp[i], sizeof(char), &dwBytes, nullptr);

		//	StateKey Save
		strTemp.clear();
		strTemp = string(pImgObj->GetStateKey().begin(), pImgObj->GetStateKey().end());
		bySize = static_cast<BYTE>(strTemp.size());
		WriteFile(hFile, &bySize, sizeof(bySize), &dwBytes, nullptr);
		for (BYTE i = 0; i < bySize; ++i)
			WriteFile(hFile, &strTemp[i], sizeof(char), &dwBytes, nullptr);

		//	RenderType Save
		RENDER_TYPE eRenderType = pImgObj->GetRenderType();
		WriteFile(hFile, &eRenderType, sizeof(RENDER_TYPE), &dwBytes, nullptr);

		//	Type Save
		CImageObject::IMG_OBJ_TYPE eImgObjType = pImgObj->GetImgType();
		WriteFile(hFile, &eImgObjType, sizeof(CImageObject::IMG_OBJ_TYPE), &dwBytes, nullptr);

		switch (eImgObjType)
		{
		case CImageObject::ANIM_OBJ:
		{
			FRAME tFrame = dynamic_cast<CAnimatedObject*>(pImgObj)->GetFrame();
			WriteFile(hFile, &tFrame, sizeof(FRAME), &dwBytes, nullptr);
		}
		break;
		case CImageObject::NON_ANIM_OBJ:
		{
			int iDrawID = dynamic_cast<CNonAnimatedObject*>(pImgObj)->GetDrawID();
			WriteFile(hFile, &iDrawID, sizeof(int), &dwBytes, nullptr);
		}
		break;
		}
	}
}

void CTerrain::LoadData(const TCHAR * pFilePath)
{
	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	LoadData(hFile, pFilePath);

	CloseHandle(hFile);
}

void CTerrain::LoadData(HANDLE hFile, const TCHAR * pFilePath)
{
	//	Tile Load
	if (!m_vecTile.empty())
	{
		for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE_INFO*>);
		m_vecTile.clear();
	}

	DWORD dwBytes = 0;
	TILE_INFO tTile = {};

	for (int i = 0; i < TILEX * TILEY; ++i)
	{
		ReadFile(hFile, &tTile, sizeof(TILE_INFO), &dwBytes, nullptr);

		if (0 == dwBytes)
			break;

		m_vecTile.push_back(new TILE_INFO(tTile));
	}

	if (!m_ImgList.empty())
	{
		for (auto& pImgObj : m_ImgList)
			SafeDelete(pImgObj);
		m_ImgList.clear();
	}

	CImageObject::IMG_OBJ_TYPE eImgObjType;

	INFO tInfo = {};
	float fSort = 0.f, fDegree = 0.f;
	BYTE bySize = 0;
	char szBuffer[MAX_STR] = {};
	string ObjectKey = "";
	string StateKey = "";
	RENDER_TYPE eRenderType = RENDER_END;
	int iDrawID = 0;
	FRAME tFrame = {};
	bool bRenderPush = false;
	int iSize = 0;
	ReadFile(hFile, &iSize, sizeof(int), &dwBytes, nullptr);

	if (0 == dwBytes)
		return;

	for (int i = 0; i < iSize; ++i)
	{
		//	Info Load
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwBytes, nullptr);
		if (0 == dwBytes)
			break;

		//	bRenderPush
		ReadFile(hFile, &bRenderPush, sizeof(bool), &dwBytes, nullptr);

		//	fSort Load
		ReadFile(hFile, &fSort, sizeof(float), &dwBytes, nullptr);

		//	fDegree Load
		ReadFile(hFile, &fDegree, sizeof(float), &dwBytes, nullptr);

		//	ObjectKey Load

		ZeroMemory(szBuffer, MAX_STR);
		ReadFile(hFile, &bySize, sizeof(bySize), &dwBytes, nullptr);
		ReadFile(hFile, szBuffer, bySize, &dwBytes, nullptr);
		ObjectKey = szBuffer;

		//	StateKey Load
		ZeroMemory(szBuffer, MAX_STR);
		ReadFile(hFile, &bySize, sizeof(bySize), &dwBytes, nullptr);
		ReadFile(hFile, szBuffer, bySize, &dwBytes, nullptr);
		StateKey.clear();
		StateKey = szBuffer;

		//	RenderType Load
		ReadFile(hFile, &eRenderType, sizeof(RENDER_TYPE), &dwBytes, nullptr);

		//	Type Load
		ReadFile(hFile, &eImgObjType, sizeof(CImageObject::IMG_OBJ_TYPE), &dwBytes, nullptr);

		switch (eImgObjType)
		{
		case CImageObject::ANIM_OBJ:
		{
			ReadFile(hFile, &tFrame, sizeof(FRAME), &dwBytes, nullptr);
			CAnimatedObject* pAnimObj = CAnimatedObject::Create(wstring(ObjectKey.begin(), ObjectKey.end()), wstring(StateKey.begin(), StateKey.end()),
				static_cast<WORD>(tFrame.fMaxFrame), tInfo.vPos, tInfo.vScale, bRenderPush, fDegree, fSort, eRenderType);
			m_ImgList.push_back(pAnimObj);
		}
		break;
		case CImageObject::NON_ANIM_OBJ:
		{
			ReadFile(hFile, &iDrawID, sizeof(int), &dwBytes, nullptr);
			CNonAnimatedObject* pNonAnimObj = CNonAnimatedObject::Create(wstring(ObjectKey.begin(), ObjectKey.end()), wstring(StateKey.begin(), StateKey.end()),
				tInfo.vPos, tInfo.vScale, bRenderPush, fDegree, fSort, iDrawID, eRenderType);
			m_ImgList.push_back(pNonAnimObj);
		}
		break;
		}
	}
}
