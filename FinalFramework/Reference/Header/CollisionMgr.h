#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Engine_Defines.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "MultiCollider.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr : public CBase
{
DECLARE_SINGLETON(CCollisionMgr)

private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();

public:
	_int	Collision(const _float fTimeDelta);

public:
	void					Add_GameObject(const wstring& wstrCollisionListTag, CGameObject* pObj);
	list<CGameObject*>*		Get_CollisionObjectList(const wstring& wstrCollisionListTag);
	
	void					Add_CollisionFunction(const wstring& wstrSrcTag, const wstring& wstrDestTag, void(pFunc)(const wstring&, const wstring&));
	_bool					Check_ByList(CGameObject* pSrc, const wstring& wstrDestTag);
	_bool					Check_ByList(const _vec3* pPos, const _float& fRadius, const wstring& wstrDestTag);

private:
	void Clear_AllObjectList();

private:
	map<wstring, list<CGameObject*>>				m_mapObjList;
	
	vector<pair<wstring, wstring>>					m_vecTag;
	vector<void(*)(const wstring&, const wstring&)>	m_vecFunc;

	static _bool(*m_CheckFunctions[CT_END][CT_END])(const CCollider*, const CCollider*, void*, _int);

public:
	static _bool Check_Collision(CCollider* pSrc, CCollider* pDest, void* pOut, _int iType);

	static _bool SphereToSphere	(const CCollider* pSrc, const CCollider* pDest, void* pOut, _int iType);
	static _bool SphereToBox	(const CCollider* pSrc, const CCollider* pDest, void* pOut, _int iType);
	static _bool SphereToBox	(const _vec3* pPos, const _float& fRadius, const CCollider* pDest);
	static _bool SphereToMulti	(const CCollider* pSrc, const CCollider* pDest, void* pOut, _int iType);
	static _bool BoxToSphere	(const CCollider* pSrc, const CCollider* pDest, void* pOut, _int iType);
	static _bool BoxToBox		(const CCollider* pSrc, const CCollider* pDest, void* pOut, _int iType);
	static _bool BoxToMulti		(const CCollider* pSrc, const CCollider* pDest, void* pOut, _int iType);
	static _bool MultiToSphere	(const CCollider* pSrc, const CCollider* pDest, void* pOut, _int iType);
	static _bool MultiToBox		(const CCollider* pSrc, const CCollider* pDest, void* pOut, _int iType);
	static _bool MultiToMulti	(const CCollider* pSrc, const CCollider* pDest, void* pOut, _int iType);

	static _bool CollisionLineToSphere(const LINE & SrcLine, const SPHERE & DestSph);


private:
	virtual void Free()	override;
};
END

#endif // CollisionMgr_h__

