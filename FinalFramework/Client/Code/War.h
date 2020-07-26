#ifndef War_h__
#define War_h__

#include "Character.h"

class CWeapon;
class CWar : public CCharacter
{
private:
	explicit CWar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWar();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	virtual void	Hit(_int iAttack, _vec3* pAtkPos);

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag) override;
	virtual void Stop_Attack();

private:
	virtual void Change_State() override;
	virtual void Update_State(const _float& fTimeDelta) override;
	virtual void KeyInput(const _float& fTimeDelta) override;

private:
	_int Update_Weapon(const _float& fTimeDelta);

private:
	virtual void Update_Dash(const _float& fTimeDelta) override;

	_int Update_AtkHeavy01(const _float& fTimeDelta);
	_int Update_AtkHeavy02(const _float& fTimeDelta);
	_int Update_AtkHeavy03(const _float& fTimeDelta);
	_int Update_AtkChargeStart(const _float& fTimeDelta);
	_int Update_AtkChargeLoop(const _float& fTimeDelta);
	_int Update_AtkChargeEnd(const _float& fTimeDelta);

	_int Update_AtkLight01(const _float& fTimeDelta);
	_int Update_AtkLight02(const _float& fTimeDelta);
	_int Update_AtkLight02b(const _float& fTimeDelta);
	_int Update_AtkLight03(const _float& fTimeDelta);
	_int Update_AtkLight03Single(const _float& fTimeDelta);
	_int Update_AtkLight04(const _float& fTimeDelta);
	_int Update_AtkLight04Single(const _float& fTimeDelta);

	_int Update_AtkFlameBrandStart(const _float& fTimeDelta);
	_int Update_AtkFlameBrandLoop(const _float& fTimeDelta);
	_int Update_AtkFlameBrandEnd(const _float& fTimeDelta);

private:
	void KeyInput_AtkHeavy01(const _float& fTimeDelta);
	void KeyInput_AtkHeavy02(const _float& fTimeDelta);
	void KeyInput_AtkHeavy03(const _float& fTimeDelta);
	void KeyInput_AtkChargeStart(const _float& fTimeDelta);
	void KeyInput_AtkLight01(const _float& fTimeDelta);
	void KeyInput_AtkLight02(const _float& fTimeDelta);
	void KeyInput_AtkLight02b(const _float& fTimeDelta);
	void KeyInput_AtkLight03(const _float& fTimeDelta);
	void KeyInput_AtkLight03Single(const _float& fTimeDelta);
	void KeyInput_AtkLight04(const _float& fTimeDelta);
	void KeyInput_AtkLight04Single(const _float& fTimeDelta);

	void KeyInput_AtkFlameBrandStart(const _float& fTimeDelta);
	void KeyInput_AtkFlameBrandEnd(const _float& fTimeDelta);


private:
	HRESULT		SetUp_AnimIndex();
	void		Turn_ClickPos(const _float& fTimeDelta);

private:
	CWeapon*	m_pEquipedWeapon = nullptr;

	//_bool		m_bEffect = false;
	_bool		m_bEffectSound = false;
	
	_float		m_fChargeTime = 0.f;


public:
	static CWar* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

#endif // War_h__
