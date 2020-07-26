#include "ChildTransform.h"

USING(Engine)

CChildTransform::CChildTransform()
{
}

CChildTransform::~CChildTransform()
{
}

HRESULT Engine::CChildTransform::Ready_Component()
{
	FAILED_CHECK_RETURN(CTransform::Ready_Component(), E_FAIL);

	m_pBoneParent = nullptr;

	ZeroMemory(&m_vRevAngle, sizeof(_vec3));

	return S_OK;
}

_int Engine::CChildTransform::Update_Component(const _float& fTimeDelta)
{
	_int iExit = CTransform::Update_Component(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	if (m_vRevAngle.x || m_vRevAngle.y || m_vRevAngle.z)
	{
		_matrix matRotX, matRotY, matRotZ;
		D3DXMatrixRotationX(&matRotX, m_vRevAngle.x);
		D3DXMatrixRotationY(&matRotY, m_vRevAngle.y);
		D3DXMatrixRotationZ(&matRotZ, m_vRevAngle.z);

		m_matWorld *= matRotX * matRotY * matRotZ;
	}

	if (m_pBoneParent)
		m_matWorld *= *m_pBoneParent;

	return 0;
}

CChildTransform * CChildTransform::Create()
{
	CChildTransform* pInst = new CChildTransform;

	if (FAILED(pInst->Ready_Component()))
		Safe_Release(pInst);

	return pInst;
}

void Engine::CChildTransform::Free()
{
	CTransform::Free();
}

