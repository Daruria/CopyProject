#include "Frame.h"

USING(Engine)

Engine::CFrame::CFrame()
	: m_fCallLimit(0.f)
	, m_fAccTimeDelta(0.f)
{

}

Engine::CFrame::~CFrame()
{

}

_bool Engine::CFrame::IsPermit_Call(const _float& fTimeDelta)
{
	m_fAccTimeDelta += fTimeDelta;

	if (m_fAccTimeDelta >= m_fCallLimit)
	{
		m_fAccTimeDelta = 0.f;
		return true;
	}

	return false;
}

HRESULT Engine::CFrame::Ready_Frame(const _float& fCallLimit)
{
	m_fCallLimit = 1 / fCallLimit;

	return S_OK;
}

Engine::CFrame* Engine::CFrame::Create(const _float& fCallLimit)
{
	CFrame* pInst = new CFrame;

	if (FAILED(pInst->Ready_Frame(fCallLimit)))
		Safe_Release(pInst);

	return pInst;
}

void Engine::CFrame::Free()
{

}