#ifndef PlayerUI_h__
#define PlayerUI_h__

#include "Defines.h"
#include "GameObject.h"
BEGIN(Engine)

class CObserver;
class CRenderer;

END
class CUI;
class CPlayerUI : public Engine::CGameObject 
{
protected:
	explicit CPlayerUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerUI();

public:
	virtual HRESULT Ready_GameObject(const wstring& wstrName);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	HRESULT Add_Component();

private:
	void	Update_PlayerHP();

private:
	Engine::CObserver* m_pPlayerObserver = nullptr;
	Engine::CRenderer* m_pRendererCom = nullptr;

	CUI*	m_pWarHP			= nullptr;
	CUI*	m_pWarHPFrame		= nullptr;
	CUI*	m_pWarPortrait		= nullptr;
	CUI*	m_pWarPortraitFrame = nullptr;

	_float	m_fHpScale = 1.f;

public:
	static CPlayerUI* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

protected:
	virtual void Free();
};

#endif // PlayerUI_h__
