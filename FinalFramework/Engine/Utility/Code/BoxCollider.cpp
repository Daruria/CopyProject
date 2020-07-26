#include "BoxCollider.h"
#include "Transform.h"
#include "GameObject.h"

#include "Export_Function.h"
#include "Texture.h"

USING(Engine)

CBoxCollider::CBoxCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CCollider(pGraphicDev)
{
}

CBoxCollider::~CBoxCollider()
{
}

HRESULT CBoxCollider::Ready_BoxCollider(CCollider::PARENT_TYPE eParentType, void * pParent, const _matrix * pLocal, const _vec3* pSize, const wstring & wstrName)
{
	m_eType = CT_CUBE;
	m_eParentType = eParentType;
	m_pParent = pParent;
	switch (eParentType)
	{
	case Engine::CCollider::PT_TRANSFORM:
	{	
		CTransform* pTransform = reinterpret_cast<CTransform*>(m_pParent);
		pTransform->AddRef();
	}
		break;
	//case Engine::CCollider::PT_OBJECT:
	//{	
	//	CGameObject* pObj = reinterpret_cast<CGameObject*>(m_pParent);
	//	pObj->AddRef();
	//}
	//	break;
	}
	memcpy(&m_matLocal, pLocal, sizeof(_matrix));
	m_wstrName = wstrName;
	memcpy(&m_vSize, pSize, sizeof(_vec3));

#ifdef _DEBUG
	FAILED_CHECK_RETURN(D3DXCreateBox(m_pGraphicDev, m_vSize.x, m_vSize.y, m_vSize.z, &m_CubeMesh, nullptr), E_FAIL);

	FAILED_CHECK_RETURN(Ready_ColliderTexture(m_pGraphicDev), E_FAIL);
#endif // _DEBUG

	Update_Component(0.f);
	Update_PointsPlanes();

	return S_OK;
}

_int Engine::CBoxCollider::Update_Component(const _float& fTimeDelta)
{
	_int iExit = CCollider::Update_Component(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;
	
	m_bCheckUpdate = false;
#ifdef _DEBUG
	Get_Renderer()->Add_RenderCollider(this);
	m_eColor = m_bActivated ? CC_BLUE : CC_GREEN;
#endif

	return iExit;
}

void Engine::CBoxCollider::Render_Collider()
{
#ifdef _DEBUG
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_eColor);
	m_CubeMesh->DrawSubset(0);
#endif

}

void CBoxCollider::Update_PointsPlanes()
{
	Update_Points();
	Update_Planes();
	m_bCheckUpdate = true;
}

void CBoxCollider::Update_Points()
{
	D3DXVec3TransformCoord(&m_vWorldPoint[BPT_LEFT_BOTTOM_FRONT	], &_vec3(-m_vSize.x * 0.5f, -m_vSize.y * 0.5f, -m_vSize.z * 0.5f),	&m_matWorld);
	D3DXVec3TransformCoord(&m_vWorldPoint[BPT_LEFT_BOTTOM_BACK	], &_vec3(-m_vSize.x * 0.5f, -m_vSize.y * 0.5f, m_vSize.z * 0.5f),	&m_matWorld);
	D3DXVec3TransformCoord(&m_vWorldPoint[BPT_RIGHT_BOTTOM_BACK	], &_vec3(m_vSize.x * 0.5f, -m_vSize.y * 0.5f, m_vSize.z * 0.5f),	&m_matWorld);
	D3DXVec3TransformCoord(&m_vWorldPoint[BPT_RIGHT_BOTTOM_FRONT], &_vec3(m_vSize.x * 0.5f, -m_vSize.y * 0.5f, -m_vSize.z * 0.5f),	&m_matWorld);
	D3DXVec3TransformCoord(&m_vWorldPoint[BPT_LEFT_TOP_FRONT	], &_vec3(-m_vSize.x * 0.5f, m_vSize.y * 0.5f, -m_vSize.z * 0.5f), &m_matWorld);
	D3DXVec3TransformCoord(&m_vWorldPoint[BPT_LEFT_TOP_BACK		], &_vec3(-m_vSize.x * 0.5f, m_vSize.y * 0.5f, m_vSize.z * 0.5f), &m_matWorld);
	D3DXVec3TransformCoord(&m_vWorldPoint[BPT_RIGHT_TOP_BACK]	, &_vec3(m_vSize.x * 0.5f, m_vSize.y * 0.5f, m_vSize.z * 0.5f), &m_matWorld);
	D3DXVec3TransformCoord(&m_vWorldPoint[BPT_RIGHT_TOP_FRONT	], &_vec3(m_vSize.x * 0.5f, m_vSize.y * 0.5f, -m_vSize.z * 0.5f), &m_matWorld);
}

void CBoxCollider::Update_Planes()
{
	D3DXPlaneFromPoints(&m_WorldPlane[BP_TOP],		&m_vWorldPoint[BPT_LEFT_TOP_FRONT],		&m_vWorldPoint[BPT_LEFT_TOP_BACK],		&m_vWorldPoint[BPT_RIGHT_TOP_FRONT]);
	D3DXPlaneFromPoints(&m_WorldPlane[BP_BOTTOM],	&m_vWorldPoint[BPT_LEFT_BOTTOM_FRONT],	&m_vWorldPoint[BPT_RIGHT_BOTTOM_FRONT], &m_vWorldPoint[BPT_RIGHT_BOTTOM_BACK]);
	D3DXPlaneFromPoints(&m_WorldPlane[BP_LEFT],		&m_vWorldPoint[BPT_LEFT_TOP_BACK],		&m_vWorldPoint[BPT_LEFT_TOP_FRONT],		&m_vWorldPoint[BPT_LEFT_BOTTOM_FRONT]);
	D3DXPlaneFromPoints(&m_WorldPlane[BP_RIGHT],	&m_vWorldPoint[BPT_RIGHT_TOP_FRONT],	&m_vWorldPoint[BPT_RIGHT_TOP_BACK],		&m_vWorldPoint[BPT_RIGHT_BOTTOM_BACK]);
	D3DXPlaneFromPoints(&m_WorldPlane[BP_FRONT],	&m_vWorldPoint[BPT_LEFT_TOP_FRONT],		&m_vWorldPoint[BPT_RIGHT_TOP_FRONT],	&m_vWorldPoint[BPT_RIGHT_BOTTOM_FRONT]);
	D3DXPlaneFromPoints(&m_WorldPlane[BP_BACK],		&m_vWorldPoint[BPT_RIGHT_TOP_BACK],		&m_vWorldPoint[BPT_LEFT_TOP_BACK],		&m_vWorldPoint[BPT_LEFT_BOTTOM_BACK]);
}

CBoxCollider * CBoxCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, CCollider::PARENT_TYPE eParentType, void * pParent, const _matrix * pLocal, const _vec3* pSize, const wstring & wstrName)
{
	CBoxCollider* pInst = new CBoxCollider(pGraphicDev);

	if (FAILED(pInst->Ready_BoxCollider(eParentType, pParent, pLocal, pSize, wstrName)))
		Safe_Release(pInst);

	return pInst;
}

void Engine::CBoxCollider::Free()
{
#ifdef _DEBUG
	Safe_Release(m_CubeMesh);
#endif // _DEBUG

	CCollider::Free();
}

