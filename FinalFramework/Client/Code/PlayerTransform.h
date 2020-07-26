#ifndef PlayerTransform_h__
#define PlayerTransform_h__

#include "Defines.h"
#include "Transform.h"

class CPlayerTransform : public Engine::CTransform
{
private:
	explicit CPlayerTransform();
	virtual ~CPlayerTransform();

public:
	virtual HRESULT		Ready_Component();
	virtual _int Update_Component(const _float& fTimeDelta) override;

public:
	const _matrix*	Get_RenderMatrix(_matrix* pOut = nullptr) { if (pOut) { memcpy(pOut, &m_matRender, sizeof(_matrix)); } return &m_matRender; }
	const _matrix*	Get_RenderOffset(_matrix* pOut = nullptr) { if (pOut) { memcpy(pOut, &m_matRenderOffset, sizeof(_matrix)); } return &m_matRenderOffset; }

private:
	_matrix		m_matRenderOffset;
	_matrix		m_matRender;

public:
	static CPlayerTransform* Create();

protected:
	virtual void Free() override;

};

#endif // PlayerTransform_h__
