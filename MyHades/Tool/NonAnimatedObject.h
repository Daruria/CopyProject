#pragma once
#include "ImageObject.h"

class CToolView;
class CNonAnimatedObject :
	public CImageObject
{
private:
	CNonAnimatedObject();

public:
	virtual ~CNonAnimatedObject();

public:
	// Inherited via CImageObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CNonAnimatedObject* Create(const wstring& wstrObjectKey, const wstring& wstrStateKey, const D3DXVECTOR3& vPos, 
		const D3DXVECTOR3& vScale, bool bRenderListPush, float fDegree = 0.f, float fSort = 1.f, int iDrawID = 0, RENDER_TYPE eRenderType = RENDER_OBJECT, CToolView* pView = nullptr);

public:
	virtual const TEX_INFO* GetTexInfo() const;
public:
	const int GetDrawID() const { return m_iDrawID; }
	
private:
	int m_iDrawID;
	CToolView* m_pView;
};

