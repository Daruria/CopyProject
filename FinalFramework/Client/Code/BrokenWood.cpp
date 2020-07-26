#include "stdafx.h"
#include "BrokenWood.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

#include "Export_Function.h"

CBrokenWood::CBrokenWood(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CMapObject(pGraphicDev)
{
}

CBrokenWood::~CBrokenWood()
{
}


HRESULT CBrokenWood::Ready_GameObject()
{
	FAILED_CHECK_RETURN(CStaticObject::Ready_GameObject(), E_FAIL);

	return S_OK;
}

HRESULT CBrokenWood::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName, const _vec3* pDir, const _float& fAttanuation)
{
	FAILED_CHECK_RETURN(CStaticObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);

	m_pTransformCom->Set_Scale(0.004f);

	m_vDir = *pDir;

	m_vRot = { D3DXToRadian(rand() % 360), D3DXToRadian(rand() % 360), D3DXToRadian(rand() % 360) };

	m_fAttanuation = fAttanuation;

	return S_OK;
}

HRESULT CBrokenWood::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags /*= 0*/)
{
	FAILED_CHECK_RETURN(CStaticObject::Add_Component(wContainerIdx, wstrBufferTag, dwFlags), E_FAIL);

	Engine::CComponent* pCom = nullptr;

	Engine::CCollider* pCol = nullptr;
	_matrix matCol;
	D3DXMatrixIdentity(&matCol);
	matCol._42 += 1.f;
	wstring wstrColName = L"Col_" + m_wstrName;
	//pCom = pCol = CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_OBJECT, this, &matCol, 1.f, wstrColName);
	pCom = pCol = CBoxCollider::Create(m_pGraphicDev, Engine::CCollider::PT_OBJECT, this, &matCol, &_vec3(2.f, 2.f, 2.f), wstrColName);
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);
	pCol->Activate();

	pCom = m_pNaviCom = dynamic_cast<Engine::CNavigationMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pCom);

	return S_OK;
}

_int CBrokenWood::Update_GameObject(const _float& fTimeDelta)
{
	Move(fTimeDelta);

	Gravity(fTimeDelta);
	Bounce(fTimeDelta);


	_int iExit = CStaticObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

_int CBrokenWood::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CStaticObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CBrokenWood::Render_GameObject(const _float& fTimeDelta)
{
	CStaticObject::Render_GameObject(fTimeDelta);
}

void CBrokenWood::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
	m_pNaviCom->Set_Index(m_pNaviCom->Get_Index(pPos));
}

void CBrokenWood::Set_Pos(const _vec3 & rPos)
{
	m_pTransformCom->Set_Pos(rPos);
	m_pNaviCom->Set_Index(m_pNaviCom->Get_Index(&rPos));
}

void CBrokenWood::Set_Pos(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Pos(fX, fY, fZ);
	m_pNaviCom->Set_Index(m_pNaviCom->Get_Index(&_vec3(fX, fY, fZ)));
}

void CBrokenWood::Set_Pos(const _vec3 * pPos, const _ulong & dwNaviIndex)
{
	m_pTransformCom->Set_Pos(pPos);
	m_pNaviCom->Set_Index(dwNaviIndex);
	m_pNaviCom->Set_Index(m_pNaviCom->Get_Index(pPos));
}

void CBrokenWood::Set_Pos(const _vec3 & rPos, const _ulong & dwNaviIndex)
{
	m_pTransformCom->Set_Pos(rPos);
	m_pNaviCom->Set_Index(dwNaviIndex);
	m_pNaviCom->Set_Index(m_pNaviCom->Get_Index(&rPos));
}

void CBrokenWood::Set_Pos(const _float & fX, const _float & fY, const _float & fZ, const _ulong & dwNaviIndex)
{
	m_pTransformCom->Set_Pos(fX, fY, fZ);
	m_pNaviCom->Set_Index(dwNaviIndex);
	m_pNaviCom->Set_Index(m_pNaviCom->Get_Index(&_vec3(fX, fY, fZ)));
}

void CBrokenWood::Move(const _float & fTimeDelta)
{
	_vec3 vPos, vDir;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	vDir = m_vDir * fTimeDelta;
	//m_pTransformCom->Move_Pos(m_vDir * fTimeDelta);
	m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNavigationMesh(&vPos, &vDir, nullptr));

	if (m_fUpSpeed < 5.f || vPos.y < -20.f)
		m_bIsDead = true;

	m_pTransformCom->Rotation(Engine::ROT_X, m_vRot.x * fTimeDelta);
	m_pTransformCom->Rotation(Engine::ROT_Y, m_vRot.y * fTimeDelta);
	m_pTransformCom->Rotation(Engine::ROT_Z, m_vRot.z * fTimeDelta);
}

void CBrokenWood::Gravity(const _float & fTimeDelta)
{
	_float fY = m_fUpSpeed * fTimeDelta;

	m_fGravity += fTimeDelta * GRAVITY * 50.f;

	fY -= m_fGravity * fTimeDelta;

	m_pTransformCom->Move_PosY(fY);
}

void CBrokenWood::Bounce(const _float & fTimeDelta)
{
	if (m_pNaviCom->Get_Option() != Engine::CCell::NO_OPTION)
		return;
	
	_vec3 vPos;

	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	_float fY = m_pNaviCom->Get_Height(&vPos);
	

	if (fY > vPos.y)
	{
		m_fGravity = 0.f;
		m_fUpSpeed *= m_fAttanuation;
		m_vRot *= m_fAttanuation;
	}
	
}

CBrokenWood * CBrokenWood::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag, const _vec3* pDir,
	const _float& fAttanuation)
{
	CBrokenWood* pInst = new CBrokenWood(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag, pDir, fAttanuation)))
		::Safe_Release(pInst);
 
	return pInst;
}

void CBrokenWood::Free()
{
	CStaticObject::Free();
}
