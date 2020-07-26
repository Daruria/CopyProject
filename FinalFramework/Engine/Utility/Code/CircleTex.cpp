#include "CircleTex.h"

USING(Engine)

Engine::CCircleTex::CCircleTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CCircleTex::CCircleTex(const CCircleTex& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CCircleTex::~CCircleTex()
{

}

HRESULT Engine::CCircleTex::Ready_Buffer(const _ulong& dwEdge)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = (dwEdge + 1) * 2;
	m_dwVtxFVF = FVF_TEX;
	m_dwTriCnt = dwEdge * 2;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_vec3 vStartPos = { 1.f, 0.f, 0.f };
	_float fRadian = D3DXToRadian(360.f / dwEdge);
	_matrix matRotZ;
	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		if (0 == i % 2)
		{
			_float fAngle = D3DXToRadian((360.f * i) / (m_dwVtxCnt - 2));
			if (fAngle > 2.f * D3DX_PI)
				fAngle = 0.f;
			D3DXMatrixRotationZ(&matRotZ, fAngle);
			D3DXVec3TransformCoord(&pVertex[i].vPos, &vStartPos, &matRotZ);
			pVertex[i].vTexUV	= _vec2((i / 2) / ((_float)(m_dwVtxCnt / 2)), 0.f);
		}
		else
		{
			pVertex[i].vPos		= _vec3(0.f, 0.f, 0.f);
			pVertex[i].vTexUV	= _vec2((i / 2) / ((_float)(m_dwVtxCnt / 2)), 1.f);
		}
	}

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	
	for (_ushort i = 0; i < m_dwTriCnt; i += 2)
	{
		_ushort usLT, usRT, usRB, usLB;
		usLT = i;
		usRT = i + 2;
		usRB = i + 3;
		usLB = i + 1;

		pIndex[i]._0 = usLT;
		pIndex[i]._1 = usRT;
		pIndex[i]._2 = usRB;

		pIndex[i + 1]._0 = usLT;
		pIndex[i + 1]._1 = usRB;
		pIndex[i + 1]._2 = usLB;
	}

	m_pIB->Unlock();

	m_fTop = 0.f;
	m_fBottom = 1.f;

	return S_OK;
}

void CCircleTex::Move_UV_Up(const _float & fTimeDelta, const _float & fSpeed)
{
	//VTXTEX*		pVertex = nullptr;

	//m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	//for (_uint i = 0; i < m_dwVtxCnt; i += 2)
	//{
	//	pVertex[i].vTexUV.y += fTimeDelta * fSpeed;
	//	pVertex[i + 1].vTexUV.y += fTimeDelta * fSpeed;
	//	if (pVertex[i].vTexUV.y > 2.f || pVertex[i + 1].vTexUV.y > 2.f)
	//	{
	//		pVertex[i].vTexUV.y -= 2.f;
	//		pVertex[i + 1].vTexUV.y -= 2.f;
	//	}

	//}

	//m_pVB->Unlock();
	m_fBottom += fTimeDelta * fSpeed;

	if (m_fBottom > 2.f)
		m_fBottom -= 1.f;
	m_fTop = m_fBottom - 1.f;
	
}

void CCircleTex::Render_Buffer()
{
	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		pVertex[i].vTexUV.y = 0 == i % 2 ? m_fTop : m_fBottom;
	

	m_pVB->Unlock();

	CVIBuffer::Render_Buffer();
}

CCircleTex * CCircleTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong & dwEdge)
{
	CCircleTex* pInst = new CCircleTex(pGraphicDev);

	if (FAILED(pInst->Ready_Buffer(dwEdge)))
		Safe_Release(pInst);

	return pInst;
}


Engine::CResources* Engine::CCircleTex::Clone()
{
	return new CCircleTex(*this);
}

void Engine::CCircleTex::Free()
{
	CVIBuffer::Free();
}

