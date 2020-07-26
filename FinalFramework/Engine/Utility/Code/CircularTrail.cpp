#include "CircularTrail.h"

USING(Engine)

Engine::CCircularTrail::CCircularTrail(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CCircularTrail::CCircularTrail(const CCircularTrail& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CCircularTrail::~CCircularTrail()
{

}

HRESULT Engine::CCircularTrail::Ready_Buffer(const _ulong& dwEdge, const _float& fInnerRadius, const _float& fOuterRadius)
{
	m_dwVtxSize = sizeof(VTXTEX);
	
	m_dwVtxCnt = dwEdge * 2;
	m_dwVtxFVF = FVF_TEX;
	m_dwTriCnt = m_dwVtxCnt - 2;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_vec3 vOuterPos = { 0.f, 0.f, fOuterRadius };
	_vec3 vInnerPos = { 0.f, 0.f, fInnerRadius };

	_matrix matRotY;
	_float fAngle;
	for (_ulong i = 0; i < m_dwVtxCnt; i += 2) 
	{
		fAngle = (D3DX_PI * 2 * i) / m_dwVtxCnt - D3DX_PI;
		D3DXMatrixRotationY(&matRotY, fAngle);
		D3DXVec3TransformCoord(&pVertex[i].vPos, &vOuterPos, &matRotY);
		pVertex[i].vTexUV	= _vec2(0.f, 0.f);

		D3DXVec3TransformCoord(&pVertex[i + 1].vPos, &vInnerPos, &matRotY);
		pVertex[i + 1].vTexUV	= _vec2(0.f, 1.f);
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

	return S_OK;
}

void CCircularTrail::Change_UV(const _float & fTimeDelta, const _float & fSpeed)
{
	m_fProgress += fTimeDelta * fSpeed;
	if (m_fProgress > 2.f)
		m_fProgress = 0.f;
}

void CCircularTrail::Render_Buffer()
{
	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_float fRenderRatio	= m_fRadian / (2 * D3DX_PI);
	_ulong dwStart		= (_ulong)(0.5f * (1.f - fRenderRatio) * m_dwVtxCnt);
	_ulong dwRenderCnt	= (_ulong)(fRenderRatio * m_dwVtxCnt);

	for (_ulong i = dwStart; i < dwStart + dwRenderCnt; i += 2)
	{
		_float fRatio = (_float)(i - dwStart) / dwRenderCnt;
		if (m_fProgress <= 1.f)
		{
			pVertex[i].vTexUV.x		= fRatio < m_fProgress ? fRatio / m_fProgress : 1.f;
			pVertex[i + 1].vTexUV.x = fRatio < m_fProgress ? fRatio / m_fProgress : 1.f;
		}
		else
		{
			pVertex[i].vTexUV.x		= fRatio < (m_fProgress - 1.f) ? 0.f : (fRatio - (m_fProgress - 1.f)) / (2.f - m_fProgress);
			pVertex[i + 1].vTexUV.x = fRatio < (m_fProgress - 1.f) ? 0.f : (fRatio - (m_fProgress - 1.f)) / (2.f - m_fProgress);
		}
	}
	
	m_pVB->Unlock();

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, dwStart, dwRenderCnt, 0, dwStart + dwRenderCnt - 2);
}

CCircularTrail * CCircularTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwEdge)
{
	CCircularTrail* pInst = new CCircularTrail(pGraphicDev);

	if (FAILED(pInst->Ready_Buffer(dwEdge, 0.75f, 1.f)))
		Safe_Release(pInst);

	return pInst;
}

CCircularTrail * CCircularTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong & dwEdge, const _float & fInnerRadius, const _float & fOuterRadius)
{
	CCircularTrail* pInst = new CCircularTrail(pGraphicDev);

	if (FAILED(pInst->Ready_Buffer(dwEdge, fInnerRadius, fOuterRadius)))
		Safe_Release(pInst);

	return pInst;
}


Engine::CResources* Engine::CCircularTrail::Clone()
{
	return new CCircularTrail(*this);
}

void Engine::CCircularTrail::Free()
{
	CVIBuffer::Free();
}

