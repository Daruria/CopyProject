#include "stdafx.h"
#include "PlayerTransform.h"

CPlayerTransform::CPlayerTransform()
{
}

CPlayerTransform::~CPlayerTransform()
{
}

HRESULT CPlayerTransform::Ready_Component()
{
	FAILED_CHECK_RETURN(CTransform::Ready_Component(), E_FAIL);

	D3DXMatrixIdentity(&m_matRender);

	D3DXMatrixIdentity(&m_matRenderOffset);
	D3DXMatrixRotationY(&m_matRenderOffset, D3DXToRadian(90.f));

	return S_OK;
}

_int CPlayerTransform::Update_Component(const _float& fTimeDelta)
{
	//D3DXMatrixIdentity(&m_matWorld);
	//D3DXMatrixIdentity(&m_matRender);

	//for (_uint i = 0; i < Engine::INFO_POS; ++i)
	//	memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	////	Scaling
	//for (_uint i = 0; i < Engine::INFO_POS; ++i)
	//{
	//	D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
	//	m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	//	memcpy(&m_matRender.m[i][i], (((_float*)&m_vScale) + i), sizeof(_float));
	//}


	//_matrix			matRot[Engine::ROT_END];

	//D3DXMatrixRotationX(&matRot[Engine::ROT_X], m_vAngle.x);
	//D3DXMatrixRotationY(&matRot[Engine::ROT_Y], m_vAngle.y);
	//D3DXMatrixRotationZ(&matRot[Engine::ROT_Z], m_vAngle.z);

	//m_matRender *= m_matRenderOffset;

	//for (_uint i = 0; i < Engine::INFO_POS; ++i)
	//{
	//	for (_uint j = 0; j < Engine::ROT_END; ++j)
	//	{
	//		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
	//	}
	//}

	//for (_uint i = 0; i < Engine::ROT_END; ++i)
	//	m_matRender *= matRot[i];

	//for (_uint i = 0; i < Engine::INFO_END; ++i)
	//	memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	//memcpy(&m_matRender.m[3][0], &m_vInfo[Engine::INFO_POS], sizeof(_vec3));



	_int iExit = Engine::CTransform::Update_Component(fTimeDelta);

	D3DXMatrixIdentity(&m_matRender);

	for (_uint i = 0; i < Engine::INFO_POS; ++i)
		memcpy(&m_matRender.m[i][i], (((_float*)&m_vScale) + i), sizeof(_float));

	m_matRender *= m_matRenderOffset * m_matNoScale;

	return 0;
}

CPlayerTransform * CPlayerTransform::Create()
{
	CPlayerTransform* pInst = new CPlayerTransform;

	if (FAILED(pInst->Ready_Component()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CPlayerTransform::Free()
{
	CTransform::Free();
}

