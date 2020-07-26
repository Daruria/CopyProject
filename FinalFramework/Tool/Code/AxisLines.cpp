#include "stdafx.h"
#include "AxisLines.h"

CAxisLines::CAxisLines(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CAxisLines::CAxisLines(const CAxisLines& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CAxisLines::~CAxisLines()
{
}

HRESULT CAxisLines::Ready_AxisLines()
{
	FAILED_CHECK_RETURN(D3DXCreateLine(m_pGraphicDev, &m_pD3DXLine), E_FAIL);

	m_vPoint[AP_ORIGIN] = { 0.f, 0.f, 0.f };
	m_vPoint[AP_X]		= { 1.f, 0.f, 0.f };
	m_vPoint[AP_Y]		= { 0.f, 1.f, 0.f };
	m_vPoint[AP_Z]		= { 0.f, 0.f, 1.f };

	return S_OK;
}

void CAxisLines::Change_ActvateAxis(ACT_AXIS eActivateAxis)
{
	m_eActvateAxis = eActivateAxis;
}

void CAxisLines::Render_AxisLines(const _float& fCamDist, const _matrix* pmatWorld)
{
	_vec3	vPoint[AP_ORIGIN][2];
	D3DXCOLOR dwColor[AP_ORIGIN] = { D3DXCOLOR(1.f, 0.f, 0.f, 1.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f), D3DXCOLOR(0.f, 0.f, 1.f, 1.f) };

	
	vPoint[AP_X][0] = vPoint[AP_Y][0] = vPoint[AP_Z][0] = m_vPoint[AP_ORIGIN];
	vPoint[AP_X][1] = m_vPoint[AP_X] * fCamDist * 0.3f;
	vPoint[AP_Y][1] = m_vPoint[AP_Y] * fCamDist * 0.3f;
	vPoint[AP_Z][1] = m_vPoint[AP_Z] * fCamDist * 0.3f;

	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_uint i = 0; i < AP_ORIGIN; ++i)
	{
		for (_uint j = 0; j < 2; ++j)
		{
			D3DXVec3TransformCoord(&vPoint[i][j], &vPoint[i][j], pmatWorld);
			D3DXVec3TransformCoord(&vPoint[i][j], &vPoint[i][j], &matView);
			if (vPoint[i][j].z <= 0.1f)
				vPoint[i][j].z = 0.1f;
			D3DXVec3TransformCoord(&vPoint[i][j], &vPoint[i][j], &matProj);
		}
	}
	
	m_pGraphicDev->EndScene();
	m_pGraphicDev->BeginScene();

	_float	fWidth = 1.f;//sqrtf(fCamDist);
	_matrix matTemp;
	for (_uint i = 0; i < AP_ORIGIN; ++i)
	{
		if (i == m_eActvateAxis)
			m_pD3DXLine->SetWidth(fWidth * 3.f);
		else
			m_pD3DXLine->SetWidth(fWidth);
		m_pD3DXLine->Begin();
		m_pD3DXLine->DrawTransform(vPoint[i], 2, D3DXMatrixIdentity(&matTemp), dwColor[i]);
		m_pD3DXLine->End();
	}


}

CAxisLines * CAxisLines::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAxisLines* pInst = new CAxisLines(pGraphicDev);

	if (FAILED(pInst->Ready_AxisLines()))
		Safe_Release(pInst);

	return pInst;
}

CAxisLines* CAxisLines::Clone()
{
	return new CAxisLines(*this);
}

void CAxisLines::Free()
{
	Safe_Release(m_pD3DXLine);
	Safe_Release(m_pGraphicDev);
}
