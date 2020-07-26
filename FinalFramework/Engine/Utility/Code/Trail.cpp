#include "Trail.h"

USING(Engine)

CTrail::CTrail()
{
}

CTrail::CTrail(const CTrail & rhs)
{
}

CTrail::~CTrail()
{
}

HRESULT CTrail::Ready_Component()
{
	return S_OK;
}

_int Engine::CTrail::Update_Component(const _float& fTimeDelta)
{
	return 0;
}

CTrail * CTrail::Create()
{
	CTrail*		pInst = new CTrail;

	if (FAILED(pInst->Ready_Component()))
		Safe_Release(pInst);

	return pInst;
}

Engine::CComponent* Engine::CTrail::Clone(void)
{
	return new CTrail(*this);
}

void Engine::CTrail::Free()
{
	throw std::logic_error("The method or operation is not implemented.");
}

