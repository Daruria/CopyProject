#include "stdafx.h"
#include "Door.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

#include "Export_Function.h"
#include "CutSceneCam.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CDynamicObject(pGraphicDev)
{
}

CDoor::~CDoor()
{
}

HRESULT CDoor::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName, vector<_ulong>* pNaviIndices)
{
	FAILED_CHECK_RETURN(CDynamicObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);

	m_pTransformCom->Set_Scale(0.01f);
	m_vecIndices = *pNaviIndices;
	m_eCurState = CLOSE;

	Close();

	return S_OK;
}

HRESULT CDoor::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags /*= 0*/)
{
	FAILED_CHECK_RETURN(CDynamicObject::Add_Component(wContainerIdx, wstrBufferTag, dwFlags), E_FAIL);

	Engine::CComponent* pCom = nullptr;

	pCom = m_pNaviCom = DC_ENG(CNavigationMesh, Engine::Clone(Engine::RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pCom);

	return S_OK;
}

_int CDoor::Update_GameObject(const _float& fTimeDelta)
{
	switch (m_eCurState)
	{
	case CDoor::CLOSE:
		if (m_pMeshCom->Is_AnimationSetEnd())
			m_eCurState = CLOSED;
		break;
	case CDoor::OPEN:
		if (m_pMeshCom->Is_AnimationSetEnd())
		{
			m_eCurState = OPENED;
			Engine::Change_MainCamera(Engine::CCameraMgr::MAIN_CAM_QUATER);
		}
		break;
	}



	_int iExit = CDynamicObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	//Engine::Add_GameObject_To_CollisionList(L"MapObject", this);

	return 0;
}

_int CDoor::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CDynamicObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CDoor::Render_GameObject(const _float& fTimeDelta)
{
	switch (m_eCurState)
	{
	case CDoor::CLOSE:
	case CDoor::OPEN:
		m_pMeshCom->Play_Animation(fTimeDelta);
		break;
	}
	
	CDynamicObject::Render_GameObject(fTimeDelta);
}

void CDoor::Open()
{
	if (m_eCurState != OPENED)
		m_eCurState = OPEN;
	
	m_pMeshCom->Set_AnimationSet(1, 0.05);
	for (auto& Index : m_vecIndices)
	{
		m_pNaviCom->Change_CellOption(Index, Engine::CCell::NO_OPTION);
	}
	FAILED_CHECK_RETURN(Engine::Change_MainCamera(Engine::CCameraMgr::MAIN_CAM_CUT_SCENE), );
	
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	vPos.y += 2.5f;
	dynamic_cast<CCutSceneCam*>(Engine::Get_MainCamera())->Set_At(&vPos);
	dynamic_cast<CCutSceneCam*>(Engine::Get_MainCamera())->Set_Distance(15.f);
	dynamic_cast<CCutSceneCam*>(Engine::Get_MainCamera())->Set_Angle(&_vec3(0.f, D3DX_PI * 0.5f, 0.f));


	Engine::Play_Sound(L"prop_drawbridge.ogg", Engine::CSoundMgr::EFFECT);
	
}

void CDoor::Close()
{
	if (m_eCurState != CLOSED)
		m_eCurState = CLOSE;
	m_pMeshCom->Set_AnimationSet(0, 0.05);
	for (auto& Index : m_vecIndices)
	{
		m_pNaviCom->Change_CellOption(Index, Engine::CCell::NO_MOVE);
	}
}

CDoor * CDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag, vector<_ulong>* pNaviIndices)
{
	CDoor* pInst = new CDoor(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag, pNaviIndices)))
		::Safe_Release(pInst);
 
	return pInst;
}

void CDoor::Free()
{
	CDynamicObject::Free();
}
