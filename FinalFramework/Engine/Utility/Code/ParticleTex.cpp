#include "ParticleTex.h"

#ifndef GRAVITY
#define GRAVITY 9.8f
#endif
USING(Engine)

Engine::CParticleTex::CParticleTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_pVBTransform(NULL)
	, m_pDeclaration(NULL)
{

}

Engine::CParticleTex::CParticleTex(const CParticleTex& rhs)
	: CVIBuffer(rhs)
	, m_pVBTransform(rhs.m_pVBTransform)
	, m_pDeclaration(rhs.m_pDeclaration)
{
	m_pVBTransform->AddRef();
	m_pDeclaration->AddRef();
}

Engine::CParticleTex::~CParticleTex()
{

}

HRESULT CParticleTex::Ready_Buffer(const _ulong & dwVtxCnt)
{
	m_dwVtxCnt = dwVtxCnt;

	m_dwVtxSize = sizeof(VTXPTTEX);

	m_dwVtxFVF = 0;
	m_dwTriCnt = dwVtxCnt;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXPTTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		pVertex[i].vPos = _vec3(0.f, 0.f, 0.f);
		pVertex[i].fPSize = 1.f;
		pVertex[i].vTexUV = _vec2(0.f, 0.f);
	}

	m_pVB->Unlock();

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXMATRIX) * m_dwVtxCnt,
		0,
		m_dwVtxFVF,
		D3DPOOL_MANAGED,
		&m_pVBTransform,
		NULL),
		E_FAIL);

	VTXMATRIX* pVtxMatrix = NULL;

	m_pVBTransform->Lock(0, 0, (void**)&pVtxMatrix, 0);

	_vec3 vAngle = {};
	_matrix matRotX, matRotY, matRotZ;
	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		vAngle = { (_float)(rand() % 100), (_float)(rand() % 100), (_float)(rand() % 100) };
		D3DXMatrixRotationAxis(&pVtxMatrix[i].Matrix, D3DXVec3Normalize(&vAngle, &vAngle), (_float)(rand() % 10000));
		ZeroMemory(&pVtxMatrix[i].vPos, sizeof(_vec3));
	}

	m_pVBTransform->Unlock();

	D3DVERTEXELEMENT9		Element[MAX_FVF_DECL_SIZE] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0 },
		{ 0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_PSIZE,		0 },
		{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0 },

		{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,	1 },
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	2 },
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	3 },
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	4 },
		D3DDECL_END()
	};

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexDeclaration(Element, &m_pDeclaration), E_FAIL);

	m_fSpeed = 100.f;

	return S_OK;
}

void CParticleTex::Render_Buffer(const _float& fTimeDelta)
{
	VTXMATRIX* pVtxMatrix = nullptr;

	m_pVBTransform->Lock(0, 0, (void**)&pVtxMatrix, 0);

	m_fGravity += fTimeDelta * GRAVITY * 0.1f;
	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		pVtxMatrix[i].vPos += pVtxMatrix[i].vLook * fTimeDelta * m_fSpeed;
		pVtxMatrix[i].vPos += _vec4(0.f, -1.f, 0.f, 0.f) * m_fGravity;
	}

	m_pVBTransform->Unlock();

	m_pGraphicDev->SetVertexDeclaration(m_pDeclaration);

	m_pGraphicDev->SetStreamSourceFreq(0, 1);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	m_pGraphicDev->SetStreamSourceFreq(1, 1);
	m_pGraphicDev->SetStreamSource(1, m_pVBTransform, 0, sizeof(VTXMATRIX));

	m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, 0, m_dwVtxCnt);

	m_pGraphicDev->SetStreamSourceFreq(0, 1);
	m_pGraphicDev->SetStreamSourceFreq(1, 1);

}

void CParticleTex::Reset_Buffer()
{
	VTXMATRIX* pVtxMatrix = NULL;

	m_pVBTransform->Lock(0, 0, (void**)&pVtxMatrix, 0);

	_vec3 vAngle = {};
	_matrix matRotX, matRotY, matRotZ;
	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		vAngle = { (_float)(rand() % 100), (_float)(rand() % 100), (_float)(rand() % 100) };
		D3DXMatrixRotationAxis(&pVtxMatrix[i].Matrix, D3DXVec3Normalize(&vAngle, &vAngle), (_float)(rand() % 10000));
		ZeroMemory(&pVtxMatrix[i].vPos, sizeof(_vec3));
	}
	m_fGravity = 0.f;

	m_pVBTransform->Unlock();
}

Engine::CParticleTex* Engine::CParticleTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwVtxCnt)
{
	CParticleTex* pInst = new CParticleTex(pGraphicDev);

	if (FAILED(pInst->Ready_Buffer(dwVtxCnt)))
		Safe_Release(pInst);

	return pInst;
}


Engine::CResources* Engine::CParticleTex::Clone()
{
	return new CParticleTex(*this);
}

void Engine::CParticleTex::Free()
{
	Safe_Release(m_pDeclaration);
	Safe_Release(m_pVBTransform);

	CVIBuffer::Free();
}

