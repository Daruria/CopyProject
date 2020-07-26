#include "CylinderTex.h"

USING(Engine)

Engine::CCylinderTex::CCylinderTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CCylinderTex::CCylinderTex(const CCylinderTex& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CCylinderTex::~CCylinderTex()
{

}

HRESULT Engine::CCylinderTex::Ready_Buffer(const _ulong& dwSlices, const _ulong& dwStacks, const _float& fUpperRadius, const _float& fLowerRadius, const _float& fHeight)
{
	if (dwStacks < 2)
	{
		wstring wstrMsg = L"dwStacks : " + to_wstring(dwStacks) + L", not enough stacks.";

		MessageBox(NULL, wstrMsg.c_str(), L"CylinderTex Ready Failed.", MB_OK);

		return E_FAIL;
	}

	m_dwStacks = dwStacks;
	m_dwSlices = dwSlices;

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = (dwSlices + 1) * dwStacks;
	m_dwVtxFVF = FVF_TEX;
	m_dwTriCnt = dwSlices * (dwStacks - 1) * 2;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_float	fDeltaRadius = fUpperRadius - fLowerRadius;
	_float	fStartRadius = fLowerRadius;

	for (_uint i = 0; i < dwStacks; ++i)
	{
		_vec3 vStartPos = { 0.f, (fHeight * (_float)i / dwStacks) - 0.5f * fHeight, fDeltaRadius * i / (_float)dwStacks + fStartRadius };
		_matrix matRotY;
		_float fAngle = 0.f;
		for (_uint j = 0; j < dwSlices + 1; ++j)
		{
			fAngle = j == dwSlices ? 0.f : ((_float)j / dwSlices) * D3DX_PI * 2.f;
			D3DXMatrixRotationY(&matRotY, fAngle);
			D3DXVec3TransformCoord(&pVertex[i * (dwSlices + 1) + j].vPos, &vStartPos, &matRotY);
			pVertex[i * (dwSlices + 1) + j].vTexUV = _vec2((_float)j / dwSlices, (_float) i / (dwStacks - 1));
		}
	}

	m_pVB->Unlock();

	INDEX16*	pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ushort usLT, usRT, usLD, usRD;
	_ulong dwTriCnt = 0;
	for (_uint i = 0; i < dwStacks - 1; ++i)
	{
		for (_uint j = 0; j < dwSlices; ++j)
		{
			usLD = (_ushort)(j + i *( dwSlices + 1));
			//usRD = (usLD + 1) % dwSlices == 0 ? usLD + 1 - dwSlices : usLD + 1;
			usRD = usLD + 1;
			usLT = (_ushort)(usLD + (dwSlices + 1));
			usRT = (_ushort)(usRD + (dwSlices + 1));

			pIndex[dwTriCnt]._0 = usLT;
			pIndex[dwTriCnt]._1 = usRT;
			pIndex[dwTriCnt]._2 = usRD;
			++dwTriCnt;

			pIndex[dwTriCnt]._0 = usLT;
			pIndex[dwTriCnt]._1 = usRD;
			pIndex[dwTriCnt]._2 = usLD;
			++dwTriCnt;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

void CCylinderTex::Render_Buffer(const _float & fTimeDelta, const _float & fSpeed, const _vec2* pUV)
{
	_vec2 vDeltaUV = *pUV * fSpeed * fTimeDelta;
	
	m_vUV += vDeltaUV;
	
	if (vDeltaUV.x > 1.f)
	{
		m_vUV.x -= 1.f;
		vDeltaUV.x -= 1.f;
	}
	if (vDeltaUV.y > 1.f)
	{
		m_vUV.y -= 1.f;
		vDeltaUV.y -= 1.f;
	}

	VTXTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_uint i = 0; i < m_dwStacks; ++i)
	{
		for (_uint j = 0; j < m_dwSlices + 1; ++j)
		{
			pVertex[i * (m_dwSlices + 1) + j].vTexUV += vDeltaUV;
		}
	}

	m_pVB->Unlock();

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void CCylinderTex::Render_Buffer()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

Engine::CCylinderTex* Engine::CCylinderTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwSlices, const _ulong& dwStacks, 
													const _float& fUpperRadius, const _float& fLowerRadius, const _float& fHeight)
{
	CCylinderTex* pInst = new CCylinderTex(pGraphicDev);

	if (FAILED(pInst->Ready_Buffer(dwSlices, dwStacks, fUpperRadius, fLowerRadius, fHeight)))
		Safe_Release(pInst);

	return pInst;
}

Engine::CResources* Engine::CCylinderTex::Clone()
{
	return new CCylinderTex(*this);
}

void Engine::CCylinderTex::Free()
{
	CVIBuffer::Free();
}

