#include "stdafx.h"
#include "BossSubject.h"
#include "Transform.h"

CBossSubject::CBossSubject()
{
}

CBossSubject::~CBossSubject()
{
}

HRESULT CBossSubject::Ready_Subject()
{
	m_pTransform = Engine::CTransform::Create();
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);

	return S_OK;
}

void CBossSubject::Update_Subject(int iMsg, void * pData)
{
	switch ((EBossData)iMsg)
	{
	case EBossData::POSITION:
		m_pTransform->Set_Pos(reinterpret_cast<_vec3*>(pData));
		break;
	case EBossData::SCALE:
		m_pTransform->Set_Scale(reinterpret_cast<_vec3*>(pData));
		break;
	case EBossData::ANGLE:
		m_pTransform->Set_Angle(reinterpret_cast<_vec3*>(pData));
		break;
	case EBossData::WORLD:
		m_pTransform->Set_WorldMatrix(reinterpret_cast<_matrix*>(pData));
		break;
	case EBossData::TRANSFORM:
		memcpy(m_pTransform, pData, sizeof(Engine::CTransform));
		break;
	case EBossData::HP:
		memcpy(&m_iHP, pData, sizeof(_int));
		break;
	case EBossData::HPMAX:
		memcpy(&m_iHPMax, pData, sizeof(_int));
		break;
	case EBossData::NAVI_INDEX:
		memcpy(&m_dwNaviIndex, pData, sizeof(_ulong));
		break;
	}
}

void CBossSubject::Get_Data(int iMsg, void * pData)
{
	switch ((EBossData)iMsg)
	{
	case EBossData::POSITION:
		m_pTransform->Get_WorldPos(reinterpret_cast<_vec3*>(pData));
		break;
	case EBossData::SCALE:
		m_pTransform->Get_Scale(reinterpret_cast<_vec3*>(pData));
		break;
	case EBossData::ANGLE:
		m_pTransform->Get_Angle(reinterpret_cast<_vec3*>(pData));
		break;
	case EBossData::WORLD:
		m_pTransform->Get_WorldMatrix(reinterpret_cast<_matrix*>(pData));
		break;
	case EBossData::TRANSFORM:
		memcpy(pData, m_pTransform, sizeof(Engine::CTransform));
		break;
	case EBossData::TRANSFORM_POINTER:
		pData = m_pTransform;
		break;
	case EBossData::HP:
		memcpy(pData, &m_iHP, sizeof(_int));
		break;
	case EBossData::HPMAX:
		memcpy(pData, &m_iHPMax, sizeof(_int));
		break;
	case EBossData::NAVI_INDEX:
		memcpy(pData, &m_dwNaviIndex, sizeof(_ulong));
		break;
	}
}

CBossSubject * CBossSubject::Create()
{
	CBossSubject* pInst = new CBossSubject;

	if (FAILED(pInst->Ready_Subject()))
		::Safe_Release(pInst);

	return pInst;
}

void CBossSubject::Free()
{
	::Safe_Release(m_pTransform);
	CSubject::Free();
}
