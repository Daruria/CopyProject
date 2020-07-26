#include "stdafx.h"
#include "PlayerSubject.h"
#include "Transform.h"

CPlayerSubject::CPlayerSubject()
{
}

CPlayerSubject::~CPlayerSubject()
{
}

HRESULT CPlayerSubject::Ready_Subject()
{
	m_pTransform = Engine::CTransform::Create();
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);

	return S_OK;
}

void CPlayerSubject::Update_Subject(int iMsg, void * pData)
{
	switch ((EPlayerData)iMsg)
	{
	case EPlayerData::POSITION:
		m_pTransform->Set_Pos(reinterpret_cast<_vec3*>(pData));
		break;
	case EPlayerData::SCALE:
		m_pTransform->Set_Scale(reinterpret_cast<_vec3*>(pData));
		break;
	case EPlayerData::ANGLE:
		m_pTransform->Set_Angle(reinterpret_cast<_vec3*>(pData));
		break;
	case EPlayerData::WORLD:
		m_pTransform->Set_WorldMatrix(reinterpret_cast<_matrix*>(pData));
		break;
	case EPlayerData::TRANSFORM:
		memcpy(m_pTransform, pData, sizeof(Engine::CTransform));
		break;
	case EPlayerData::HP:
		memcpy(&m_iHP, pData, sizeof(_int));
		break;
	case EPlayerData::HPMAX:
		memcpy(&m_iHPMax, pData, sizeof(_int));
		break;
	case EPlayerData::NAVI_INDEX:
		memcpy(&m_dwNaviIndex, pData, sizeof(_ulong));
		break;
	}
}

void CPlayerSubject::Get_Data(int iMsg, void * pData)
{
	switch ((EPlayerData)iMsg)
	{
	case EPlayerData::POSITION:
		m_pTransform->Get_WorldPos(reinterpret_cast<_vec3*>(pData));
		break;
	case EPlayerData::SCALE:
		m_pTransform->Get_Scale(reinterpret_cast<_vec3*>(pData));
		break;
	case EPlayerData::ANGLE:
		m_pTransform->Get_Angle(reinterpret_cast<_vec3*>(pData));
		break;
	case EPlayerData::WORLD:
		m_pTransform->Get_WorldMatrix(reinterpret_cast<_matrix*>(pData));
		break;
	case EPlayerData::TRANSFORM:
		memcpy(pData, m_pTransform, sizeof(Engine::CTransform));
		break;
	case EPlayerData::TRANSFORM_POINTER:
		pData = m_pTransform;
		break;
	case EPlayerData::HP:
		memcpy(pData, &m_iHP, sizeof(_int));
		break;
	case EPlayerData::HPMAX:
		memcpy(pData, &m_iHPMax, sizeof(_int));
		break;
	case EPlayerData::NAVI_INDEX:
		memcpy(pData, &m_dwNaviIndex, sizeof(_ulong));
		break;
	}
}

CPlayerSubject * CPlayerSubject::Create()
{
	CPlayerSubject* pInst = new CPlayerSubject;

	if (FAILED(pInst->Ready_Subject()))
		::Safe_Release(pInst);

	return pInst;
}

void CPlayerSubject::Free()
{
	::Safe_Release(m_pTransform);
	CSubject::Free();
}
