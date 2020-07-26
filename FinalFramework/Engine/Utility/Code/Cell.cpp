#include "Cell.h"
#include "Line.h"

USING(Engine)

Engine::CCell::CCell(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_pNeighbor, sizeof(CCell*) * NEIGHBOR_END);
}

Engine::CCell::~CCell(void)
{

}

HRESULT Engine::CCell::Ready_Cell(const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, CELL_OPTION eOption /*= NO_OPTION*/)
{
	m_dwIndex = dwIndex;

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	m_pLine[LINE_AB] = CLine::Create(&_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z), 
									 &_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z));

	m_pLine[LINE_BC] = CLine::Create(&_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z),
									 &_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z));

	m_pLine[LINE_CA] = CLine::Create(&_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z),
									 &_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z));

	m_eOption = eOption;

#ifdef _DEBUG
	FAILED_CHECK_RETURN(D3DXCreateLine(m_pGraphicDev, &m_pD3DXLine), E_FAIL);

#endif
	return S_OK;
}

_bool Engine::CCell::Compare_Point(const _vec3* pPointF, const _vec3* pPointS, CCell* pCell)
{
	if (m_vPoint[POINT_A] == *pPointF)
	{
		if (m_vPoint[POINT_B] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}
		if (m_vPoint[POINT_C] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	if (m_vPoint[POINT_B] == *pPointF)
	{
		if (m_vPoint[POINT_A] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}
		if (m_vPoint[POINT_C] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
	}

	if (m_vPoint[POINT_C] == *pPointF)
	{
		if (m_vPoint[POINT_B] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
		if (m_vPoint[POINT_A] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}


	return false;
}

void Engine::CCell::Render_Cell(void)
{
	_vec3	vPoint[4];

	vPoint[0] = m_vPoint[POINT_A];
	vPoint[1] = m_vPoint[POINT_B];
	vPoint[2] = m_vPoint[POINT_C];
	vPoint[3] = m_vPoint[POINT_A];

	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_ulong i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

		if (vPoint[i].z <= 0.1f)
			vPoint[i].z = 0.1f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	m_pD3DXLine->SetWidth(3.f);	// 라인의 굵기를 결정하는 함수
	m_pGraphicDev->EndScene();
	m_pGraphicDev->BeginScene();

	m_pD3DXLine->Begin();

	_matrix matTemp;

	D3DXCOLOR dwColor;
	switch (m_eOption)
	{
	case NO_OPTION:	dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f); break;
	case FALL:		dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f); break;
	case NO_MOVE:	dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f); break;
	default:		dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f); break;
	}
	m_pD3DXLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), dwColor);

	m_pD3DXLine->End();

}

Engine::CCell* Engine::CCell::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, CELL_OPTION eOption /*= NO_OPTION*/)
{
	CCell*	pInstance = new CCell(pGraphicDev);

	if (FAILED(pInstance->Ready_Cell(dwIndex, pPointA, pPointB, pPointC, eOption)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCell::Free(void)
{
	for (_uint i = 0; i < LINE_END; ++i)
		Safe_Release(m_pLine[i]);

	Safe_Release(m_pD3DXLine);
	Safe_Release(m_pGraphicDev);
}

_bool CCell::IsInside(const _vec3 * pPos)
{
	_vec2 vPos = { pPos->x, pPos->z };

	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (CLine::COMPARE_LEFT == m_pLine[i]->Compare(&vPos))
			return false;
	}

	return true;
}

Engine::CCell::COMPARE Engine::CCell::Compare(const _vec3* pEndPos, _ulong* pIndex)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (CLine::COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		{
			if (nullptr == m_pNeighbor[i])
				return COMPARE_STOP;

			else
			{
				*pIndex = *m_pNeighbor[i]->Get_Index();
				return COMPARE_MOVE;
			}
		}
	}

	return COMPARE_MOVE;
}

Engine::CCell::COMPARE CCell::Compare(const _vec3 * pTargetPos, const _vec3* pTargetDir , _ulong * pIndex, _vec3 * pOut)
{
	static _int s_iCampareCount = 0;

	if (s_iCampareCount > 30)
	{
		s_iCampareCount = 0;
		return CCell::COMPARE_STOP;
	}

	++s_iCampareCount;
	_vec3 vEndPos = *pTargetPos + *pTargetDir;
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (CLine::COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(vEndPos.x, vEndPos.z)))
		{
			if (nullptr == m_pNeighbor[i])
			{
				_vec3 vDir;
				m_pLine[i]->Get_Direction(&vDir);

				_float fDot = D3DXVec3Dot(&vDir, pTargetDir);
				vDir *= fDot;
				*pOut = *pTargetPos + vDir;

				return Compare(pTargetPos, &vDir, pIndex, pOut);
			}
			else
			{
				if (m_pNeighbor[i]->IsInside(&vEndPos))
				{
					switch (m_pNeighbor[i]->Get_Option())
					{
					case CCell::NO_OPTION:
					case CCell::FALL:
					{

						*pIndex = *m_pNeighbor[i]->Get_Index();

						memcpy(pOut, &vEndPos, sizeof(_vec3));
						s_iCampareCount = 0;

						return COMPARE_MOVE;
					}
						break;
					case CCell::NO_MOVE:
					{
						_vec3 vDir;
						m_pLine[i]->Get_Direction(&vDir);

						_float fDot = D3DXVec3Dot(&vDir, pTargetDir);
						vDir *= fDot;
						*pOut = *pTargetPos + vDir;

						return Compare(pTargetPos, &vDir, pIndex, pOut);
					}
						break;
					}
				}
				else
					return m_pNeighbor[i]->Compare(pTargetPos, pTargetDir, pIndex, pOut);
			}
		}
	}

	s_iCampareCount = 0;
	memcpy(pOut, &vEndPos, sizeof(_vec3));

	return COMPARE_MOVE;
}

void CCell::Get_Center(_vec3 * pOut) const
{
	*pOut = (m_vPoint[POINT_A] + m_vPoint[POINT_B] + m_vPoint[POINT_C]) / 3;
}

_float CCell::Get_Height(const _vec3 * pPos)
{
	D3DXPLANE Plane;
	D3DXPlaneFromPoints(&Plane, &m_vPoint[POINT_A], &m_vPoint[POINT_B], &m_vPoint[POINT_C]);

	return	-(Plane.a * pPos->x + Plane.c * pPos->z + Plane.d) / Plane.b;
}

_bool CCell::Get_AdjacentIndex(CELL_OPTION eOption, _ulong * pOut)
{
	for (_int i = 0; i < LINE_END; ++i)
	{
		if (nullptr == m_pNeighbor[i])
			continue;
		if (m_pNeighbor[i]->Get_Option() == eOption)
		{
			*pOut = *m_pNeighbor[i]->Get_Index();
			return true;
		}
	}
	return false;
}

