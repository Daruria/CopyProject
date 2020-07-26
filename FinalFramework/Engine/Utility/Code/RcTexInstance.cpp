#include "RcTexInstance.h"

USING(Engine)

#ifndef AXIS_X
#define AXIS_X _vec3(1.f, 0.f, 0.f)
#endif

#ifndef AXIS_Y
#define AXIS_Y _vec3(0.f, 1.f, 0.f)
#endif

#ifndef AXIS_Z
#define AXIS_Z _vec3(0.f, 0.f, 1.f)
#endif

Engine::CRcTexInstance::CRcTexInstance(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_uiCaseNum(0)
	, m_pVBTransform(nullptr)
	, m_pDeclaration(nullptr)
{

}

Engine::CRcTexInstance::CRcTexInstance(const CRcTexInstance& rhs)
	: CVIBuffer(rhs)
	, m_uiCaseNum(rhs.m_uiCaseNum)
	, m_pVBTransform(rhs.m_pVBTransform)
	, m_pDeclaration(rhs.m_pDeclaration)
{
	if (m_pVBTransform)
		m_pVBTransform->AddRef();
	if (m_pDeclaration)
		m_pDeclaration->AddRef();
}

Engine::CRcTexInstance::~CRcTexInstance()
{

}

HRESULT Engine::CRcTexInstance::Ready_Buffer(const _uint& uiCaseNum, const _uint& uiInstNum, const _ulong& dwUCnt, const _ulong& dwVCnt)
{
	m_uiCaseNum = uiCaseNum;
	m_uiInstNum = uiInstNum;

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4 * uiCaseNum;
	m_dwVtxFVF = FVF_TEX;
	m_dwTriCnt = 2 * uiInstNum;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_float fUSize = 1.f / dwUCnt;
	_float fVSize = 1.f / dwVCnt;

	for (_uint i = 0; i < uiCaseNum; ++i)
	{
		_float fULeft = (i % dwUCnt) * fUSize, fVTop = (i / dwUCnt) * fVSize;

		pVertex[i * 4].vPos = { -0.5f, 0.5f, 0.f };
		pVertex[i * 4].vTexUV = { fULeft, fVTop };

		pVertex[i * 4 + 1].vPos = { 0.5f, 0.5f, 0.f };
		pVertex[i * 4 + 1].vTexUV = { fULeft + fUSize, fVTop};

		pVertex[i * 4 + 2].vPos = { 0.5f, -0.5f, 0.f };
		pVertex[i * 4 + 2].vTexUV = { fULeft + fUSize, fVTop + fVSize};

		pVertex[i * 4 + 3].vPos = { - 0.5f, -0.5f, 0.f };
		pVertex[i * 4 + 3].vTexUV = { fULeft, fVTop + fVSize };
	}

	m_pVB->Unlock();

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXMATRIX) * uiInstNum,
		0,
		m_dwVtxFVF,
		D3DPOOL_MANAGED,
		&m_pVBTransform,
		NULL),
		E_FAIL);

	VTXMATRIX* pVtxMatrix = NULL;

	m_pVBTransform->Lock(0, 0, (void**)&pVtxMatrix, 0);

	_matrix matRotY;
	_vec3 vPos, vDir;
	_float fDist = 0.f, fAngle = 0.f;
	for (_uint i = 0; i < m_uiInstNum; ++i)
	{
		fDist = (rand() % 100) * 0.05f;
		fAngle = D3DXToRadian(rand() % 360);
		D3DXMatrixRotationY(&matRotY, fAngle);
		D3DXVec3TransformCoord(&vPos, &_vec3(fDist, 0.f, 0.f), &matRotY);

		D3DXMatrixRotationAxis(&pVtxMatrix[i].Matrix, &_vec3((_float)rand(), (_float)rand(), (_float)rand()), fAngle);

		pVtxMatrix[i].vPos		= _vec4(vPos, 1.f);
		
		vDir = vPos;
		vDir.y += 20.f;
		D3DXVec3Normalize(&vDir, &vDir);
		m_vecDir.push_back(vDir);
	}

	m_pVBTransform->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ulong dwTriCnt = 0;

	for (_uint i = 0; i < m_uiInstNum; ++i)
	{
		_ushort usIndex = (rand() % m_uiCaseNum) * 4;

		// 0
		pIndex[dwTriCnt]._0 = usIndex;
		pIndex[dwTriCnt]._1 = usIndex + 1;
		pIndex[dwTriCnt]._2 = usIndex + 2;
		++dwTriCnt;

		// 1
		pIndex[dwTriCnt]._0 = usIndex;
		pIndex[dwTriCnt]._1 = usIndex + 2;
		pIndex[dwTriCnt]._2 = usIndex + 3;
		++dwTriCnt;
	}

	m_pIB->Unlock();

	D3DVERTEXELEMENT9		Element[MAX_FVF_DECL_SIZE] = {
		// 0번 스트림을 사용할 버텍스의 구성 요소
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0 },

		// 1번 스트림을 사용할 버텍스의 구성 요소

		{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,	1 },
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	2 },
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	3 },
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	4 },
		D3DDECL_END()

	};

	// 위에서 생성한 정점 정보를 기준으로 정점을 대표하는 컴 객체 생성
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexDeclaration(Element, &m_pDeclaration), E_FAIL);

	return S_OK;
}

void CRcTexInstance::Render_Buffer(const _float & fTimeDelta, const _float& fSpeed, const _float& fGravity)
{
	VTXMATRIX* pVtxMatrix = NULL;

	m_pVBTransform->Lock(0, 0, (void**)&pVtxMatrix, 0);

	m_fGravity += fTimeDelta * fGravity;

	//if (m_fGravity > 1.f)
	//	Reset_Instance();

	for (_uint i = 0; i < m_uiInstNum; ++i)
	{
		pVtxMatrix[i].vPos += _vec4(fTimeDelta * fSpeed * m_vecDir[i] + _vec3(0.f, -1.f, 0.f) * m_fGravity, 0.f);
	}

	m_pVBTransform->Unlock();

	m_pGraphicDev->SetVertexDeclaration(m_pDeclaration);
	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_uiInstNum);

	m_pGraphicDev->SetStreamSource(1, m_pVBTransform, 0, sizeof(VTXMATRIX));
	m_pGraphicDev->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

	m_pGraphicDev->SetStreamSourceFreq(0, 1);
	m_pGraphicDev->SetStreamSourceFreq(1, 1);

}

void CRcTexInstance::Reset_Instance()
{
	VTXMATRIX* pVtxMatrix = NULL;

	m_pVBTransform->Lock(0, 0, (void**)&pVtxMatrix, 0);

	m_fGravity = 0.f;

	for (_uint i = 0; i < m_uiInstNum; ++i)
	{
		pVtxMatrix[i].vPos = { 0.f, 0.f, 0.f, 1.f };
	}



	m_pVBTransform->Unlock();
}

Engine::CRcTexInstance* Engine::CRcTexInstance::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& uiCaseNum, const _uint& uiInstNum, const _ulong& dwUCnt, const _ulong& dwVCnt)
{
	CRcTexInstance* pInst = new CRcTexInstance(pGraphicDev);

	if (FAILED(pInst->Ready_Buffer(uiCaseNum, uiInstNum, dwUCnt, dwVCnt)))
		Safe_Release(pInst);

	return pInst;
}

Engine::CResources* Engine::CRcTexInstance::Clone()
{
	return new CRcTexInstance(*this);
}

void Engine::CRcTexInstance::Free()
{
	Safe_Release(m_pVBTransform);
	Safe_Release(m_pDeclaration);
	CVIBuffer::Free();
}

