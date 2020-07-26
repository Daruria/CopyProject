#include "stdafx.h"
#include "ObjectMgr.h"
#include "GameObject.h"
#include "Functor.h"
#include "ImageObject.h"
#include "AnimatedObject.h"
#include "NonAnimatedObject.h"

IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
{
}


CObjectMgr::~CObjectMgr()
{
	Release();
}

HRESULT CObjectMgr::AddObject(OBJECT_TYPE eType, CGameObject * pObject)
{
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_ObjectList[eType].push_back(pObject);

	return S_OK;
}

HRESULT CObjectMgr::AddLayer(RENDER_TYPE eType, CGameObject * pObject)
{
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_RenderList[eType].push_back(pObject);

	return S_OK;
}

void CObjectMgr::RenderListClear()
{
	for (int i = 0; i < RENDER_END; ++i)
	{
		m_RenderList[i].clear();
	}
}

int CObjectMgr::Update()
{
	list<CGameObject*>::iterator iter;

	for (int i = 0; i < OBJECT_END; ++i)
	{
		for (iter = m_ObjectList[i].begin(); iter != m_ObjectList[i].end(); )
		{
			int iEvent = (*iter)->Update();

			if (DEAD_OBJ == iEvent)
			{
				SafeDelete((*iter));
				iter = m_ObjectList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return NO_EVENT;
}

void CObjectMgr::LateUpdate()
{
	for (int i = 0; i < OBJECT_END; ++i)
	{
		for (auto& pObject : m_ObjectList[i])
			pObject->LateUpdate();
	}
}

void CObjectMgr::Render()
{
	for (int i = 0; i < RENDER_END; ++i)
	{
		m_RenderList[i].sort(CLayerSort());

		for (auto& pObject : m_RenderList[i])
			pObject->Render();

		m_RenderList[i].clear();
	}
}

void CObjectMgr::Release()
{
	for (int i = 0; i < OBJECT_END; ++i)
	{
		for_each(m_ObjectList[i].begin(), m_ObjectList[i].end(), SafeDelete<CGameObject*>);
		m_ObjectList[i].clear();
	}

	for (int i = 0; i < RENDER_END; ++i)
		m_RenderList[i].clear();
}

void CObjectMgr::SaveData(HANDLE hFile, const TCHAR * pFilePath, CToolView* pView /*= nullptr*/)
{

	DWORD dwBytes = 0;

	float fSort = 0.f, fDegree = 0.f;
	bool bRenderPush = false;
	INFO tInfo;
	for (auto& pGameObject : m_ObjectList[OBJECT_ENVIRONMENT])
	{
		CImageObject* pImgObj = dynamic_cast<CImageObject*>(pGameObject);

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

void CObjectMgr::LoadData(HANDLE hFile, const TCHAR * pFilePath, CToolView* pView /*= nullptr*/)
{
	DWORD dwBytes = 0;

	if (!m_ObjectList[OBJECT_ENVIRONMENT].empty())
	{
		for (auto& pGameObject : m_ObjectList[OBJECT_ENVIRONMENT])
			SafeDelete(pGameObject);
		m_ObjectList[OBJECT_ENVIRONMENT].clear();
	}

	CImageObject::IMG_OBJ_TYPE eImgObjType;

	INFO tInfo = {};
	float fSort = 0.f, fDegree = 0.f;
	BYTE bySize = 0;
	char szBuffer[MAX_STR] = {};
	string ObjectKey = "";
	string StateKey = "";
	RENDER_TYPE eRenderType = RENDER_OBJECT;
	int iDrawID = 0;
	FRAME tFrame = {};
	bool bRenderPush = false;

	while (true)
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
				static_cast<WORD>(tFrame.fMaxFrame), tInfo.vPos, tInfo.vScale, bRenderPush, fDegree, fSort, eRenderType, pView);
			m_ObjectList[OBJECT_ENVIRONMENT].push_back(pAnimObj);
		}
		break;
		case CImageObject::NON_ANIM_OBJ:
		{
			ReadFile(hFile, &iDrawID, sizeof(int), &dwBytes, nullptr);
			CNonAnimatedObject* pNonAnimObj = CNonAnimatedObject::Create(wstring(ObjectKey.begin(), ObjectKey.end()), wstring(StateKey.begin(), StateKey.end()),
				tInfo.vPos, tInfo.vScale, bRenderPush, fDegree, fSort, iDrawID, eRenderType, pView);
			m_ObjectList[OBJECT_ENVIRONMENT].push_back(pNonAnimObj);
		}
		break;
		}
	}
}
