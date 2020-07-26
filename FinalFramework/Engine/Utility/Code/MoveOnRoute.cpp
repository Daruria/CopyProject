#include "MoveOnRoute.h"

USING(Engine)

Engine::CMoveOnRoute::CMoveOnRoute()
{
}

Engine::CMoveOnRoute::~CMoveOnRoute()
{

}

HRESULT	Engine::CMoveOnRoute::Ready_Component()
{
	return S_OK;
}
_int Engine::CMoveOnRoute::Update_Component(const _float& fTimeDelta)
{

	return 0;
}

void Engine::CMoveOnRoute::Free()
{
}
