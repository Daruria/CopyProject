#include "Transform.h"

USING(Engine)

Engine::CTransform::CTransform()
	: m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(_vec3) * INFO_END);
}

Engine::CTransform::~CTransform()
{

}

void Engine::CTransform::Set_ParentTransform(CTransform* pTransform)
{
	//Safe_Release(m_pParentTransform);
	m_pParentTransform = pTransform;
	//m_pParentTransform->AddRef();
}

HRESULT	Engine::CTransform::Ready_Component()
{
	D3DXMatrixIdentity(&m_matPreRender);
	D3DXMatrixIdentity(&m_matPreWorld);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matNoScale);
	D3DXMatrixIdentity(&m_matRenderOffset);
	D3DXMatrixIdentity(&m_matRender);

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));


	return S_OK;
}
HRESULT CTransform::Ready_Component(const _ulong & dwFlags)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_dwFlags = dwFlags;

	return S_OK;
}
_int Engine::CTransform::Update_Component(const _float& fTimeDelta)
{
	memcpy(&m_matPreWorld, &m_matWorld, sizeof(_matrix));
	memcpy(&m_matPreRender, &m_matRender, sizeof(_matrix));


	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matNoScale);

	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	//	Scaling
	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	_matrix			matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	m_matNoScale = m_matRot = matRot[ROT_X] * matRot[ROT_Y] * matRot[ROT_Z];

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &m_matRot);
		//for (_uint j = 0; j < ROT_END; ++j)
		//{
		//	D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		//}
	}

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	memcpy(m_matNoScale.m[3], &m_vInfo[INFO_POS], sizeof(_vec3));


	if (m_dwFlags & TS_BONE)
	{
		if (m_pBoneParent)
		{
			m_matWorld		*= *m_pBoneParent;
			m_matNoScale	*= *m_pBoneParent;
		}
	}

	if (m_dwFlags & TS_PARENT)
	{
		if (m_pParentTransform)
		{
			if (m_pParentTransform->Check_Flag(TS_RENDEROFFSET))
			{
				m_matWorld		*= m_pParentTransform->Get_RenderOffsetRef();
				m_matNoScale	*= m_pParentTransform->Get_RenderOffsetRef();
				m_matRot		*= m_pParentTransform->Get_RenderOffsetRef();
			}

			m_matWorld *= m_pParentTransform->Get_WorldMatrixRef();
			m_matNoScale *= m_pParentTransform->Get_NoScaleMatrixRef();
			m_matRot *= m_pParentTransform->Get_RotationMatrixRef();
		}
	}

	if (m_dwFlags & TS_RENDEROFFSET)
	{
		D3DXMatrixIdentity(&m_matRender);

		for (_uint i = 0; i < Engine::INFO_POS; ++i)
			memcpy(&m_matRender.m[i][i], (((_float*)&m_vScale) + i), sizeof(_float));

		m_matRender *= m_matRenderOffset * m_matNoScale;
	}



	return 0;
}

Engine::CTransform* Engine::CTransform::Create(const _ulong& dwFlags)
{
	CTransform*		pInst = new CTransform;

	if (FAILED(pInst->Ready_Component(dwFlags)))
		Safe_Release(pInst);

	return pInst;
}

void Engine::CTransform::Free()
{
	//Safe_Release(m_pParentTransform);
}
