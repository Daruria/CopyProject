#pragma once
#include "StaticObject.h"
class CImageObject :
	public CStaticObject
{
public:
	enum IMG_OBJ_TYPE { ANIM_OBJ, NON_ANIM_OBJ, IMG_OBJ_END };

public:
	CImageObject();
	virtual ~CImageObject();

public:	// Inherited via CGameObject
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;

protected:
	virtual HRESULT Initialize() PURE;
	virtual void Release() PURE;

public:
	void Die() { m_bIsDead = true; }
	void SetSort(float fSort) { m_fSort = fSort; }
	void SetRenderListPush(bool bPush) { m_bRenderListPush = bPush; }
	void SetDegree(float fDegree) { m_fDegree = fDegree; }

public:
	const wstring& GetObjectKey()	const { return m_wstrObjectKey; }
	const wstring& GetStateKey()	const { return m_wstrStateKey; }
	virtual const TEX_INFO* GetTexInfo() const PURE;
	const bool GetIsDead()			const { return m_bIsDead; }
	const float GetSort()			const { return m_fSort; }
	const IMG_OBJ_TYPE GetImgType() const { return m_eImgObjType; }
	const bool GetRenderListPush()	const { return m_bRenderListPush; }
	const float GetDegree()			const { return m_fDegree; }

protected:
	bool	m_bIsDead;
	float	m_fSort;
	float	m_fDegree;

	IMG_OBJ_TYPE m_eImgObjType;

	wstring m_wstrObjectKey;
	wstring m_wstrStateKey;

	bool	m_bRenderListPush;

	D3DCOLOR m_d3dColor;
};

