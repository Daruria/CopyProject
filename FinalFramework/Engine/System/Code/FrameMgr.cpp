#include "FrameMgr.h"

USING(Engine)

IMPLEMENT_SINGLETON(CFrameMgr)

Engine::CFrameMgr::CFrameMgr()
{

}

Engine::CFrameMgr::~CFrameMgr()
{
	Free();
}

_bool Engine::CFrameMgr::IsPermit_Call(const wstring& wstrFrameTag, const _float& fTimeDelta)
{
	CFrame* pInst = Find_Frame(wstrFrameTag);
	NULL_CHECK_RETURN(pInst, false);

	return pInst->IsPermit_Call(fTimeDelta);
}

HRESULT Engine::CFrameMgr::Ready_Frame(const wstring& wstrFrameTag, const _float& fCallLimit)
{
	CFrame* pInst = Find_Frame(wstrFrameTag);
	if (nullptr != pInst)
		return E_FAIL;

	pInst = CFrame::Create(fCallLimit);
	NULL_CHECK_RETURN(pInst, E_FAIL);

	m_mapFrame.emplace(wstrFrameTag, pInst);

	return S_OK;
}

Engine::CFrame* Engine::CFrameMgr::Find_Frame(const wstring& wstrFrameTag)
{
	auto iter = m_mapFrame.find(wstrFrameTag);

	if (m_mapFrame.end() == iter)
		return nullptr;

	return iter->second;
}

void Engine::CFrameMgr::Free()
{
	for_each(m_mapFrame.begin(), m_mapFrame.end(), CDeleteMap());
	m_mapFrame.clear();
}