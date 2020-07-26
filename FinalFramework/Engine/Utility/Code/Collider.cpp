#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"

#include "Export_Utility.h"

USING(Engine)

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CCollider::CCollider(const CCollider & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_eType(rhs.m_eType)
	, m_wstrName(rhs.m_wstrName)
	, m_matLocal(rhs.m_matLocal)
	, m_matWorld(rhs.m_matWorld)
	, m_eParentType(rhs.m_eParentType)
{
	m_pGraphicDev->AddRef();

	switch (m_eParentType)
	{
	case Engine::CCollider::PT_TRANSFORM:
	{
		CTransform* pTransform = reinterpret_cast<CTransform*>(m_pParent);
		pTransform->AddRef();
	}
	break;
	//case Engine::CCollider::PT_OBJECT:
	//{
	//	CGameObject* pObj = reinterpret_cast<CGameObject*>(m_pParent);
	//	pObj->AddRef();
	//}
	//break;
	//case Engine::CCollider::PT_COLLIDER:
	//{
	//	CCollider* pColl = reinterpret_cast<CCollider* >(m_pParent);
	//	pColl->AddRef();
	//}
	//break;
	}
}

CCollider::~CCollider()
{
}

_int CCollider::Update_Component(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pParent, -1);

	switch (m_eParentType)
	{
	case Engine::CCollider::PT_BONE:
	{
		_vec3	vParentWorldPos;
		_matrix matParentNoScale, BoneRender;
		BoneRender = *m_pParentBone * *reinterpret_cast<CTransform*>(m_pParent)->Get_RenderMatrix();
		reinterpret_cast<CTransform*>(m_pParent)->Get_WorldPos(&vParentWorldPos);
		reinterpret_cast<CTransform*>(m_pParent)->Get_NoScaleMatrix(&matParentNoScale);
		memcpy(matParentNoScale.m[3], BoneRender.m[3], sizeof(_vec3));
		m_matWorld = m_matLocal * matParentNoScale;
	}
		break;
	case Engine::CCollider::PT_TRANSFORM:
	{
		_vec3	vParentWorldPos;
		_matrix matParentNoScale;
		reinterpret_cast<CTransform*>(m_pParent)->Get_WorldPos(&vParentWorldPos);
		reinterpret_cast<CTransform*>(m_pParent)->Get_NoScaleMatrix(&matParentNoScale);
		memcpy(matParentNoScale.m[3], &vParentWorldPos, sizeof(_vec3));
		m_matWorld = m_matLocal * matParentNoScale;
	}
		break;
	case Engine::CCollider::PT_OBJECT:
	{
		CGameObject* pObj = reinterpret_cast<CGameObject*>(m_pParent);
		CTransform* pTransform = dynamic_cast<CTransform*>(pObj->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		NULL_CHECK_RETURN(pTransform, -1);

		_vec3	vParentWorldPos;
		_matrix matParentNoScale;

		pTransform->Get_WorldPos(&vParentWorldPos);
		pTransform->Get_NoScaleMatrix(&matParentNoScale);
		memcpy(matParentNoScale.m[3], &vParentWorldPos, sizeof(_vec3));
		m_matWorld = m_matLocal * matParentNoScale;
	}
		break;
	case Engine::CCollider::PT_COLLIDER:
	{
		CCollider* pColl = reinterpret_cast<CCollider*>(m_pParent);
		m_matWorld = m_matLocal * *pColl->Get_WorldlMatrix();
	}
	break;
	}

	return 0;
}

void CCollider::Render_Collider()
{
}

COLLISIONTYPE CCollider::Check_CollisionType(void* pOpponent, _bool bCollision)
{
	_bool bAlreadyExist = false;

	auto iter = m_CollidedList.begin();
	auto iterEnd = m_CollidedList.end();

	for (; iter !=iterEnd; ++iter)
	{
		if (*iter == pOpponent)
		{
			bAlreadyExist = true;
			break;
		}
	}

	if (bAlreadyExist)
	{
		if (bCollision)
			return COLL_STAY;
		else
		{
			m_CollidedList.erase(iter);
			return COLL_LEAVE;
		}
	}
	else
	{
		if (bCollision)
		{
			m_CollidedList.push_back(pOpponent);
			return COLL_ENTER;
		}
		else
		{
			return COLL_AWAY;
		}
	}

	return COLL_AWAY;
}

#ifdef _DEBUG
HRESULT CCollider::Ready_ColliderTexture(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pTextureCom = dynamic_cast<CTexture*>(::Clone(RESOURCE_STATIC, L"Texture_ColliderColor"));
	if (!m_pTextureCom)
	{
		LPDIRECT3DTEXTURE9	pTexture[CC_END] = { nullptr, };

		D3DXCOLOR dwColor[CC_END] = { { 1.f, 0.f, 0.f, 1.f },{ 0.f, 1.f, 0.f, 1.f },{ 0.f, 0.f, 1.f, 1.f },{ 1.f, 0.f, 1.f, 1.f },{ 1.f, 1.f, 0.f, 1.f } };

		for (_int i = 0; i < CC_END; ++i)
		{
			pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture[i], NULL);

			D3DLOCKED_RECT	tLockRect;
			ZeroMemory(&tLockRect, sizeof(D3DLOCKED_RECT));

			pTexture[i]->LockRect(0, &tLockRect, NULL, NULL);

			*((_ulong*)tLockRect.pBits) = dwColor[i];

			pTexture[i]->UnlockRect(0);
		}

		Ready_Texture(pGraphicDev, RESOURCE_STATIC, L"Texture_ColliderColor", TEX_NORMAL, pTexture, CC_END);

		for (_int i = 0; i < CC_END; ++i)
			Safe_Release(pTexture[i]);

		m_pTextureCom = dynamic_cast<CTexture*>(::Clone(RESOURCE_STATIC, L"Texture_ColliderColor"));
	}



	return S_OK;
}
#endif // _DEBUG

void CCollider::Free()
{
	switch (m_eParentType)
	{
	case Engine::CCollider::PT_BONE:
		break;
	case Engine::CCollider::PT_TRANSFORM:
	{
		CTransform*& pTransform = reinterpret_cast<CTransform* &>(m_pParent);
		Safe_Release(pTransform);
	}
		break;
	//case Engine::CCollider::PT_OBJECT:
	//	Safe_Release(reinterpret_cast<CGameObject* &>(m_pParent));
	//	break;
	//case Engine::CCollider::PT_COLLIDER:
	//	Safe_Release(reinterpret_cast<CCollider* &>(m_pParent));
	//	break;
	}
#ifdef _DEBUG
	Safe_Release(m_pTextureCom);
#endif // _DEBUG


	Safe_Release(m_pGraphicDev);
}
