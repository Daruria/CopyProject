#ifndef BossUI_h__
#define BossUI_h__

#include "Defines.h"
#include "GameObject.h"
BEGIN(Engine)

class CObserver;
class CRenderer;

END
class CUI;
class CBossUI : public Engine::CGameObject 
{
protected:
	explicit CBossUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossUI();

public:
	virtual HRESULT Ready_GameObject(const wstring& wstrName);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	HRESULT Add_Component();

private:
	void	Update_BossHP();

private:
	Engine::CObserver* m_pBossObserver = nullptr;
	Engine::CRenderer* m_pRendererCom = nullptr;

	CUI*	m_pBossHP			= nullptr;
	CUI*	m_pBossHPFrame		= nullptr;

	_float	m_fHpScale = 1.f;

public:
	static CBossUI* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

protected:
	virtual void Free();
};

#endif // BossUI_h__
