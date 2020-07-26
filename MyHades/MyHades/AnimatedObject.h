#pragma once
#include "ImageObject.h"
class CAnimatedObject :
	public CImageObject
{
public:
	CAnimatedObject();
	virtual ~CAnimatedObject();

public:
	// Inherited via CImageObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CAnimatedObject* Create(const wstring& wstrObjectKey, const wstring& wstrStateKey, const WORD iCnt, const D3DXVECTOR3& vPos,
		const D3DXVECTOR3& vScale = D3DXVECTOR3(1.f, 1.f , 0.f), bool bRenderListPush = false, float fDegree = 0.f, float fSort = 1.f,
		RENDER_TYPE eType = RENDER_OBJECT);
	static CAnimatedObject* Create(const wstring & wstrObjectKey, float fx, float fy, float fScale, RENDER_TYPE eType = RENDER_OBJECT);

public:
	void SetState(const wstring& wstrStateKey, ANIM_TYPE eType = ANIM_REPEAT, float fFrameSpeed = 1 / 30.f);

public:
	const FRAME& GetFrame()			const { return m_tFrame; }
	const bool GetOnceFinish()		const { return m_bOnceFinish; }
	virtual const TEX_INFO* GetTexInfo() const;

private:
	bool Animation();

protected:
	bool	m_bOnceFinish;

	FRAME	m_tFrame;
};

