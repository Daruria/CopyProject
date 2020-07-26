#pragma once
#include "Scene.h"
class CIntro :
	public CScene
{
public:
	CIntro();
	virtual ~CIntro();

public:// Inherited via CScene
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

	MACRO_CREATE(CIntro)

private:
	static unsigned __stdcall LoadingThreadFunc(void* pParam);

private:
	float		m_fLoadingRatio;
	HANDLE		m_hLoadingThread;
};

