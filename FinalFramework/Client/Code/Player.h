#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CNavigationMesh;

END

class CPlayerTransform;
class CCharacter;

class CPlayer : public Engine::CGameObject
{
private:
	enum class ECharacter { WAR, STRIFE, EEND };

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready_GameObject() override;

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

public:
	void KeyInput_General(const _float& fTimeDelta);

private:
	HRESULT Ready_War();
	HRESULT Ready_Strife();

	HRESULT Change_Character(ECharacter eCharacter);

private:
	vector<CCharacter*> m_vecCharacters;
	CCharacter*			m_pActiveChar = nullptr;
	

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

#endif // Player_h__

