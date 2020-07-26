#include "CollisionMgr.h"
#include "MyMath.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCollisionMgr)

_bool(*CCollisionMgr::m_CheckFunctions[CT_END][CT_END])(const CCollider*, const CCollider*, void*, _int) =
{
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, CCollisionMgr::SphereToSphere, CCollisionMgr::SphereToBox, CCollisionMgr::SphereToMulti },
	{NULL, NULL, CCollisionMgr::BoxToSphere,	CCollisionMgr::BoxToBox,	CCollisionMgr::BoxToMulti },
	{NULL, NULL, CCollisionMgr::MultiToSphere,	CCollisionMgr::MultiToBox,	CCollisionMgr::MultiToMulti }
};

Engine::CCollisionMgr::CCollisionMgr()
{

}

Engine::CCollisionMgr::~CCollisionMgr()
{

}

_int CCollisionMgr::Collision(const _float fTimeDelta)
{
	for (_uint i = 0; i < m_vecFunc.size(); ++i)
	{
		m_vecFunc[i](m_vecTag[i].first, m_vecTag[i].second);
	}
	
	Clear_AllObjectList();

	return 0;
}

void CCollisionMgr::Add_GameObject(const wstring& wstrCollisionListTag, CGameObject * pObj)
{
	auto iter = m_mapObjList.find(wstrCollisionListTag);

	if (m_mapObjList.end() == iter)
	{
		list<CGameObject*> ObjList;
		ObjList.push_back(pObj);

		m_mapObjList.emplace(wstrCollisionListTag, ObjList);
	}
	else
		iter->second.push_back(pObj);
}

list<CGameObject*>* CCollisionMgr::Get_CollisionObjectList(const wstring& wstrCollisionListTag)
{
	auto iter = m_mapObjList.find(wstrCollisionListTag);

	if (m_mapObjList.end() == iter)
		return nullptr;

	return &iter->second;
}

void CCollisionMgr::Add_CollisionFunction(const wstring& wstrSrcTag, const wstring& wstrDestTag, void(pFunc)(const wstring&, const wstring&))
{
	m_vecTag.push_back(make_pair(wstrSrcTag, wstrDestTag));
	m_vecFunc.push_back(pFunc);
}

_bool CCollisionMgr::Check_ByList(CGameObject * pSrc, const wstring & wstrDestTag)
{
	auto iter = m_mapObjList.find(wstrDestTag);

	if (iter == m_mapObjList.end())
		return false;

	CCollider* pSrcCol = dynamic_cast<CCollider*>(pSrc->Get_Component(Engine::ID_DYNAMIC, L"Com_Collider"));
	NULL_CHECK_RETURN(pSrcCol, false);

	for (auto& pObj : iter->second)
	{
		CCollider* pDestCol = dynamic_cast<CCollider*>(pObj->Get_Component(Engine::ID_DYNAMIC, L"Com_Collider"));
		NULL_CHECK_RETURN(pDestCol, false);
		
		if (Check_Collision(pSrcCol, pDestCol, nullptr, 1))
			return true;
	}

	return false;
}

_bool CCollisionMgr::Check_ByList(const _vec3 * pPos, const _float & fRadius, const wstring & wstrDestTag)
{
	auto iter = m_mapObjList.find(wstrDestTag);

	if (iter == m_mapObjList.end())
		return false;

	for (auto& pObj : iter->second)
	{
		CCollider* pDestCol = dynamic_cast<CCollider*>(pObj->Get_Component(Engine::ID_DYNAMIC, L"Com_Collider"));
		NULL_CHECK_RETURN(pDestCol, false);

		if (SphereToBox(pPos, fRadius, pDestCol))
			return true;
	}

	return false;
}


void CCollisionMgr::Clear_AllObjectList()
{
	for (auto& iter : m_mapObjList)
	{
		iter.second.clear();
	}
	m_mapObjList.clear();
}

_bool CCollisionMgr::Check_Collision(CCollider * pSrc, CCollider * pDest, void* pOut, _int iType)
{
	if (!pSrc->Is_Activate() || !pDest->Is_Activate())
		return false;

	COLLIDERTYPE eSrc = pSrc->Get_CollType();
	COLLIDERTYPE eDest = pDest->Get_CollType();

	return m_CheckFunctions[eSrc][eDest](pSrc, pDest, pOut, iType);

}

_bool CCollisionMgr::SphereToSphere(const CCollider * pSrc, const CCollider * pDest, void* pOut, _int iType)
{
	const CSphereCollider* pSrcSphere = dynamic_cast<const CSphereCollider*>(pSrc);
	NULL_CHECK_RETURN(pSrcSphere, false);
	const CSphereCollider* pDestSphere = dynamic_cast<const CSphereCollider*>(pDest);
	NULL_CHECK_RETURN(pDestSphere, false);

	_vec3 vSrcPos, vDestPos, vDestToSrc;
	pSrcSphere->Get_Pos(&vSrcPos);
	pDestSphere->Get_Pos(&vDestPos);
	vDestPos = vSrcPos - vDestPos;

	_float fDist = D3DXVec3Length(&vDestPos);
	_float fSum = pSrcSphere->Get_Radius() + pDestSphere->Get_Radius();

	D3DXVec3Normalize(&vDestPos, &vDestPos);
	vDestPos *= (fSum - fDist);

	if (0 == iType)
		memcpy(pOut, &vDestPos, sizeof(_vec3));

	return fSum > fDist;
}

_bool CCollisionMgr::SphereToBox(const CCollider * pSrc, const CCollider * pDest, void* pOut, _int iType)
{
	const CSphereCollider* pSrcSphere = dynamic_cast<const CSphereCollider*>(pSrc);
	NULL_CHECK_RETURN(pSrcSphere, false);

	const CBoxCollider*		pDestBox = dynamic_cast<const CBoxCollider*>(pDest);
	NULL_CHECK_RETURN(pDestBox, false);

	_vec3	vBoxSize, vBoxCenter, vSphereCenter, vBoxToSphere;
	pDestBox->Get_Size(&vBoxSize);
	pDestBox->Get_Pos(&vBoxCenter);
	
	pSrcSphere->Get_Pos(&vSphereCenter);

	_float fSphereRadius, fBoxRadius;

	fSphereRadius = pSrcSphere->Get_Radius();
	fBoxRadius = D3DXVec3Length(&vBoxSize) * 0.5f;
	
	vBoxToSphere = vSphereCenter - vBoxCenter;
	_float fCenterDistance = D3DXVec3Length(&vBoxToSphere);
	if (fCenterDistance > fSphereRadius + fBoxRadius)
		return false;

	if (!pDestBox->CheckUpdate())
		const_cast<CBoxCollider*>(pDestBox)->Update_PointsPlanes();

	const D3DXPLANE* pBoxPlanes = pDestBox->Get_WorldPlanes();

	for (_int i = 0; i < CBoxCollider::BP_END; ++i)
	{
		if (D3DXPlaneDotCoord(&pBoxPlanes[i], &vSphereCenter) > fSphereRadius)
			return false;
	}

	_vec2 v2DDist[3] = { {vBoxSize.x, vBoxSize.y}, { vBoxSize.x, vBoxSize.z }, { vBoxSize.y, vBoxSize.z } };

	for (_int i = 0; i < 3; ++i)
	{
		if (fCenterDistance > D3DXVec2Length(&v2DDist[i]) * 0.5f + fSphereRadius)
			return false;
	}

	_float fDot = 0.f, fDotDiv = 0.f, fDotMax = 0.f, fDotDivMax = 0.f;
	_int iClosest = 0;
	for (_int i = 0; i < CBoxCollider::BP_END; ++i)
	{
		fDot = D3DXPlaneDotNormal(&pBoxPlanes[i], &vBoxToSphere);
		fDotDiv = fDot / *((_float*)vBoxSize + (i / 2));
		if (fDotDiv > fDotDivMax)
		{
			iClosest = i;
			fDotDivMax = fDotDiv;
			fDotMax = fDotMax;
		}
	}

	_vec3 vDir;
	memcpy(&vDir, &pBoxPlanes[iClosest], sizeof(_vec3));

	vDir *= (fSphereRadius - D3DXPlaneDotCoord(&pBoxPlanes[iClosest], &vSphereCenter));
	
	if (0 == iType)
		memcpy(pOut, &vDir, sizeof(_vec3));

	return true;
}

_bool CCollisionMgr::SphereToBox(const _vec3 * pPos, const _float & fRadius, const CCollider * pDest)
{
	const CBoxCollider*		pDestBox = dynamic_cast<const CBoxCollider*>(pDest);
	NULL_CHECK_RETURN(pDestBox, false);

	_vec3	vBoxSize, vBoxCenter;
	pDestBox->Get_Size(&vBoxSize);
	pDestBox->Get_Pos(&vBoxCenter);

	_float fBoxRadius;

	fBoxRadius = D3DXVec3Length(&vBoxSize) * 0.5f;

	if (D3DXVec3Length(&(*pPos - vBoxCenter)) > fRadius + fBoxRadius)
		return false;

	if (!pDestBox->CheckUpdate())
		const_cast<CBoxCollider*>(pDestBox)->Update_PointsPlanes();

	const D3DXPLANE* pBoxPlanes = pDestBox->Get_WorldPlanes();

	_float fClosestDist = FLT_MAX;
	_float	fDistance = 0.f;
	_int iCloset = 0;
	for (_int i = 0; i < CBoxCollider::BP_END; ++i)
	{
		if (D3DXPlaneDotCoord(&pBoxPlanes[i], pPos) > fRadius)
			return false;
	}

	return true;
}

_bool CCollisionMgr::SphereToMulti(const CCollider * pSrc, const CCollider * pDest, void* pOut, _int iType)
{
	const CMultiCollider* pDestMulti = dynamic_cast<const CMultiCollider*>(pDest);
	NULL_CHECK_RETURN(pDestMulti, false);

	for (auto& pChild : *pDestMulti->Get_ChildList())
	{
		COLLIDERTYPE eChild = pChild->Get_CollType();
		if (m_CheckFunctions[CT_SPHERE][eChild](pSrc, pChild, pOut, iType))
			return true;
	}

	return false;
}

_bool CCollisionMgr::BoxToSphere(const CCollider * pSrc, const CCollider * pDest, void* pOut, _int iType)
{
	return SphereToBox(pDest, pSrc, pOut, iType);
}

_bool CCollisionMgr::CollisionLineToSphere(const LINE & SrcLine, const SPHERE & DestSph)
{
	_vec3 SphCenter_To_LinePos1= SrcLine.vPos1 - DestSph.vPos;

	_float fC = D3DXVec3Dot(&SphCenter_To_LinePos1,	&SphCenter_To_LinePos1) - DestSph.fRadius * DestSph.fRadius;

	if (fC <= 0.0f)
		return true;

	_vec3 vLineDir = SrcLine.vPos2 - SrcLine.vPos1;
	_float fLineLength = D3DXVec3Length(&vLineDir);
	if (0.f == fLineLength)
		return false;

	_vec3 vLineDirNorm = vLineDir / fLineLength;

	_float fB_Prime = D3DXVec3Dot(&SphCenter_To_LinePos1, &vLineDirNorm);

	if (fB_Prime > 0.0f)
		return false;

	_float fSqrtDiscriminant = sqrt(fB_Prime * fB_Prime - fC);

	_float fT1 = -fB_Prime + fSqrtDiscriminant;
	if (fT1 >= 0.0f && fT1 <= fLineLength)
		return true;

	_float fT2 = -fB_Prime - fSqrtDiscriminant;
	if (fT2 >= 0.0f && fT2 <= fLineLength)
		return true;

	return false;
}

void Engine::CCollisionMgr::Free()
{
	for (auto& iter : m_mapObjList)
	{
		for_each(iter.second.begin(), iter.second.end(), CDeleteObj());
		iter.second.clear();
	}
	m_mapObjList.clear();
}

_bool CCollisionMgr::BoxToBox(const CCollider * pSrc, const CCollider * pDest, void* pOut, _int iType)
{
	


	return _bool();
}

_bool CCollisionMgr::BoxToMulti(const CCollider * pSrc, const CCollider * pDest, void* pOut, _int iType)
{
	const CMultiCollider* pDestMulti = dynamic_cast<const CMultiCollider*>(pDest);
	NULL_CHECK_RETURN(pDestMulti, false);

	for (auto& pChild : *pDestMulti->Get_ChildList())
	{
		COLLIDERTYPE eChild = pChild->Get_CollType();
		if (m_CheckFunctions[CT_CUBE][eChild](pSrc, pChild, pOut, iType))
			return true;
	}

	return false;
}

_bool CCollisionMgr::MultiToSphere(const CCollider * pSrc, const CCollider * pDest, void* pOut, _int iType)
{
	return SphereToMulti(pDest, pSrc, pOut, iType);
}

_bool CCollisionMgr::MultiToBox(const CCollider * pSrc, const CCollider * pDest, void* pOut, _int iType)
{
	return BoxToMulti(pDest, pSrc, pOut, iType);
}

_bool CCollisionMgr::MultiToMulti(const CCollider * pSrc, const CCollider * pDest, void* pOut, _int iType)
{
	const CMultiCollider* pSrcMulti = dynamic_cast<const CMultiCollider*>(pSrc);
	NULL_CHECK_RETURN(pSrcMulti, false);
	const CMultiCollider* pDestMulti = dynamic_cast<const CMultiCollider*>(pDest);
	NULL_CHECK_RETURN(pDestMulti, false);

	for (auto& pSrcChild : *pSrcMulti->Get_ChildList())
	{
		COLLIDERTYPE eSrcChild = pSrcChild->Get_CollType();
		for (auto& pDestChild : *pDestMulti->Get_ChildList())
		{
			COLLIDERTYPE eDestChild = pDestChild->Get_CollType();
			if (m_CheckFunctions[eSrcChild][eDestChild](pSrcChild, pDestChild, pOut, iType))
				return true;
		}
	}

	return false;
}
