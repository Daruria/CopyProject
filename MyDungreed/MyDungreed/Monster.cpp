#include "stdafx.h"
#include "Monster.h"
#include "Particle.h"
#include "RotateParticle.h"
#include "FieldItem.h"
#include "GoldCoin.h"

CMonster::CMonster() :
	m_bDir(true),
	m_tCharInfo(CHARACTER_INFO{}),
	m_strName("")
{
}


CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	CMoveObj::Initialize();
	INSERT_BITMAP("MonsterHpBack", L"Image/MonsterHpBar.bmp");
	INSERT_BITMAP("MonsterLifeGauge", L"Image/MonsterLifeGauge.bmp");
}

int CMonster::Update()
{
	CMoveObj::Update();
	return 0;
}

void CMonster::LateUpdate()
{
	CMoveObj::LateUpdate();
}

void CMonster::Render(HDC hDC)
{
	if (m_tCharInfo.iHP != m_tCharInfo.iHPMax)
	{
		HDC hMemDC = FIND_BITMAP_DC("MonsterHpBack");
		
		POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);
		BOOL bRender = GdiTransparentBlt(hDC, 
			static_cast<int>(tPos.fx + m_tHitBox.fx - 74 / 2), 
			static_cast<int>(tPos.fy + m_tHitBox.fy + m_tHitBox.icy / 2 + 10), 74, 20, hMemDC, 0, 0, 74, 20, RGB(255, 0, 255));

		float fHP = m_tCharInfo.iHP / static_cast<float>(m_tCharInfo.iHPMax);

		hMemDC = FIND_BITMAP_DC("MonsterLifeGauge");
		bRender = GdiTransparentBlt(hDC, 
			static_cast<int>(tPos.fx + m_tHitBox.fx - 66 / 2 + 4), 
			static_cast<int>(tPos.fy + m_tHitBox.fy + m_tHitBox.icy / 2  + 10 + 4), 
			static_cast<int>(66 * fHP), 12, hMemDC, 0, 0, 66, 12, RGB(255, 0, 255));
	}
}

void CMonster::Release()
{
}

void CMonster::SetCharInfo(const CHARACTER_INFO & tCharInfo)
{
	SetCharInfo(tCharInfo.iHP, tCharInfo.iHPMax, tCharInfo.fAttack, tCharInfo.fArmor);
}

void CMonster::SetCharInfo(int iHp, int iHpMax, float fAttack, float fArmor)
{
	m_tCharInfo.iHP = iHp;
	m_tCharInfo.iHPMax = iHpMax;
	m_tCharInfo.fAttack = fAttack;
	m_tCharInfo.fArmor = fArmor;
}

void CMonster::Attack()
{
}

void CMonster::Idle()
{
}

void CMonster::Move()
{
}

void CMonster::Die()
{
}

void CMonster::Hit(const float fAttack, const INFO& tAttackerInfo)
{
	CMoveObj::Hit(fAttack, tAttackerInfo);
	int iDamage = static_cast<int>(fAttack - m_tCharInfo.fArmor);
	m_tCharInfo.iHP -= iDamage;
	POSITION tDamageFontPos = { m_tInfo.fx + rand() % 100 - 25, m_tInfo.fy - rand() % 50 - 25 };

	GET_SINGLE(CEffectManager)->AddFontEffect(tDamageFontPos.fx, tDamageFontPos.fy, tDamageFontPos.fx + 10, tDamageFontPos.fy - 20, 0.5f, 3.f, to_string(iDamage), FT_EDGE);

	GET_SINGLE(CSoundMgr)->PlaySound(L"Hit_Monster.wav", CSoundMgr::EFFECT);
}

void CMonster::AddAttackEffect(float fx, float fy)
{

}

void CMonster::Dying(float fx, float fy)
{
	GET_SINGLE(CEffectManager)->AddEffect("DieFX", L"Image/Effect/Die/DieFX.bmp", m_tInfo.fx, m_tInfo.fy, 256,256, 11);
	GET_SINGLE(CResult)->MonsterCount();
	GET_SINGLE(CSoundMgr)->PlaySound(L"MonsterDie.wav", CSoundMgr::EFFECT);
}

void CMonster::AddGold(const int iGold)
{
	CFieldItem* pFieldCoin = new CFieldItem;
	pFieldCoin->Initialize();
	if (iGold != 100)
	{
		INSERT_BITMAP("GoldCoin", L"Image/Item/Gold/GoldCoinSprite.bmp");
		pFieldCoin->SetInfo(m_tInfo.fx, m_tInfo.fy, 7, 7);
		pFieldCoin->SetKey("GoldCoin");
		pFieldCoin->SetHitBox(0.f, 0.f, static_cast<int>(7 * 4.f), static_cast<int>(7 * 4.f));
		pFieldCoin->SetMaxFrame(8);
	}
	else
	{
		INSERT_BITMAP("Bullion", L"Image/Item/Bullion/BullionSprite.bmp");
		pFieldCoin->SetInfo(m_tInfo.fx, m_tInfo.fy, 20, 9);
		pFieldCoin->SetKey("Bullion");
		pFieldCoin->SetHitBox(0.f, 0.f, static_cast<int>(20 * 4.f), static_cast<int>(9 * 4.f));
		pFieldCoin->SetMaxFrame(7);
	}
	CGoldCoin* pGold = new CGoldCoin;
	pGold->SetType(IT_GOLD_COIN);
	pGold->SetGold(iGold);
	pFieldCoin->SetItem(pGold);
	pFieldCoin->SetFrame(0);
	pFieldCoin->SetRatio(4.f);
	GET_SINGLE(CObjectManager)->AddObj(OT_FIELD_ITEM, pFieldCoin);
}

void CMonster::AddParticle(const string & strKey, const wchar_t * pPath, float fDirX, float fDirY, float fRatio, float fFriction, float fFallingSpeed, float fJumpForce, bool bBounce)
{
	INSERT_BITMAP(strKey, pPath);
	CParticle* pParticle = new CParticle;
	SIZE tSize = GET_BITMAP_SIZE(strKey);
	pParticle->Initialize();
	pParticle->SetInfo(m_tInfo.fx, m_tInfo.fy, tSize.cx, tSize.cy);
	pParticle->SetKey(strKey);
	pParticle->SetDrawStartPos(0.f, 0.f);
	pParticle->SetHitBox(0, 0, static_cast<int>(tSize.cx * fRatio), static_cast<int>(tSize.cy * fRatio));
	pParticle->SetDirection(fDirX, fDirY);
	pParticle->SetFriction(fFriction);
	pParticle->SetRatio(fRatio);
	pParticle->SetFallingSpeed(fFallingSpeed);
	if (fJumpForce >= 0.f)
		pParticle->SetJumpForce(fJumpForce);
	pParticle->SetBounce(bBounce);
	GET_SINGLE(CObjectManager)->AddObj(OT_MONSTER_PARTICLE, pParticle);
}

void CMonster::AddRotateParticle(const string & strKey, const wchar_t * pPath, float fDirX, float fDirY)
{
	CRotateParticle *pRotateParticle = new  CRotateParticle;
	pRotateParticle->SetImage(strKey, pPath);
	SIZE tSize = GET_BITMAP_SIZE(strKey);
	pRotateParticle->Initialize();
	pRotateParticle->SetInfo(m_tInfo.fx, m_tInfo.fy, tSize.cx, tSize.cy);
	pRotateParticle->SetDir(fDirX > 0);
	pRotateParticle->SetDirection(fDirX, fDirY);
	pRotateParticle->SetHitBox(0, 0, tSize.cx, tSize.cy);
	GET_SINGLE(CObjectManager)->AddObj(OT_MONSTER_PARTICLE, pRotateParticle);
}
