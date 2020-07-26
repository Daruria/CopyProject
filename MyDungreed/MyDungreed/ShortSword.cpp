#include "stdafx.h"
#include "ShortSword.h"
#include "ScrollManager.h"
#include "EffectManager.h"

CShortSword::CShortSword():
	m_bState(true),
	m_fAngle(0.f),
	m_fSwingAngle(0.f),
	m_tDir(POSITION{}),
	m_tOffSet(POSITION{}),
	m_strDownKey("")
{
}


CShortSword::~CShortSword()
{
	Release();
}

void CShortSword::SetDownImage(const string & strDownKey, const wchar_t * pPath)
{
	m_strDownKey = strDownKey;
	INSERT_BITMAP(m_strDownKey, pPath);
}

void CShortSword::Swing()
{
	m_bState ? WeaponDown() : WeaponUp();
}

void CShortSword::AddAttackEffect(float fx, float fy)
{
	float m_fSlashAngle = static_cast<float>((rand() % 360 - 180) * (180 / PI));
	GET_SINGLE(CEffectManager)->AddRotateEffect("Slash", L"Image/Effect/Slash/SlashFX", fx, fy, 168, 168, 4, m_fSlashAngle, 1.f);
}

bool CShortSword::Attack()
{
	//if (m_dwAttackCool + m_dwAttackCoolMax <= GetTickCount())
	{
		Swing();
		SwingEffect();
		m_bAttack = true;
		m_bRenderBack = !m_bRenderBack;
		m_dwAttackCool = GetTickCount();
		GET_SINGLE(CSoundMgr)->PlaySound(L"swing0.wav", CSoundMgr::EFFECT);
	}
	return m_bState;
}

bool CShortSword::Attack(float fAttack)
{
	Swing();
	SwingEffect();
	m_bAttack = true;
	m_bRenderBack = !m_bRenderBack;
	m_dwAttackCool = GetTickCount();
	GET_SINGLE(CSoundMgr)->PlaySound(L"swing0.wav", CSoundMgr::EFFECT);
	
	return m_bState;
}

void CShortSword::Initialize()
{
	CWeapon::Initialize();
	m_bState = true;	//	True : Up, False : Down
	m_tAttackBox.icx = 160;
	m_tAttackBox.icy = 160;
	m_iPrice = 100;

	m_bRenderBack = true;
	m_eWeaponType = WT_MELEE;
	SetType(IT_WEAPON);
	SetAttack(20);
	SetName("Short Sword");
	SetIconImage("ShortSwordIcon", L"Image/Weapon/BasicShortSwordIcon.bmp");
	SetImage("ShortSword", L"Image/Weapon/BasicShortSword.bmp");

	m_bIsSkill		= false;
	m_bCanUseSkill	= false;

	m_dwAttackCool = GetTickCount();
	m_dwAttackCoolMax = 500;
}

int CShortSword::Update()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);
	float fdx = pt.x - tPos.fx;
	float fdy = pt.y - tPos.fy;
	float fDist = sqrtf(fdx * fdx + fdy * fdy);
	m_fAngle = fdx >= 0 ? atanf(fdy / fdx) : fdy >= 0 ? acosf(fdx / fDist) : -acosf(fdx / fDist);
	m_fSwingAngle = static_cast<float>(m_fAngle + PI / 2);

	bool	bDir = fdx < 0 ? m_bState : !m_bState;
	float	f90Degree= (fdx < 0) ^ m_bState ? static_cast<float>(PI / 2 ): static_cast<float>(-PI / 2);
	
	m_fAngle += static_cast<float>(PI * bDir);
	float fSine = sinf(f90Degree);
	float fCosine = cosf(f90Degree);

	m_tDir.fx = fdx / fDist;
	m_tDir.fy = fdy / fDist;

	m_tOffSet.fx = m_tDir.fx * fCosine - m_tDir.fy  * fSine;
	m_tOffSet.fy = m_tDir.fx * fSine + m_tDir.fy  * fCosine;

	if (m_dwAttackCool + 200 <= GetTickCount())
	{
		m_bAttack = false;
	}

	return 0;
}

void CShortSword::LateUpdate()
{
}

void CShortSword::Render(HDC hDC)
{
	//string strKey = m_bState ? m_strKey : m_strDownKey;
	string strKey = m_strKey;
	HDC hMemDC = FIND_ROTATED_DC(strKey, m_fAngle);

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	if (hMemDC)
		BOOL bRender = GdiTransparentBlt(hDC, 
			static_cast<int>(tPos.fx - m_tOffSet.fx * 50 - m_tInfo.icx / 2), 
			static_cast<int>(tPos.fy - m_tOffSet.fy * 50 - m_tInfo.icy / 2),
			m_tInfo.icx, m_tInfo.icy,
			hMemDC, 0, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
	else
		CAN_NOT_FOUND_BITMAP(hDC, 
			static_cast<int>(tPos.fx - m_tInfo.icx / 2), static_cast<int>(tPos.fy - m_tInfo.icy / 2), m_tInfo.icx, m_tInfo.icy);

	//hMemDC = FIND_ROTATED_DC("PlayerBasicHand", m_fAngle);
	hMemDC = FIND_BITMAP_DC("PlayerBasicHand");

	SIZE tSize = GET_BITMAP_SIZE("PlayerBasicHand");

	if (hMemDC)
		BOOL bRender = GdiTransparentBlt(hDC, 
			static_cast<int>(tPos.fx - m_tOffSet.fx * 20 - tSize.cx / 2), 
			static_cast<int>(tPos.fy - m_tOffSet.fy * 20 - tSize.cy / 2),
			tSize.cx, tSize.cy,
			hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));
	else
		CAN_NOT_FOUND_BITMAP(hDC, 
			static_cast<int>(tPos.fx - tSize.cx / 2), 
			static_cast<int>(tPos.fy - tSize.cy / 2), tSize.cx, tSize.cy);

	CWeapon::Render(hDC);
}

void CShortSword::Release()
{
}

void CShortSword::DiscRender(HDC hDC, float fx, float fy)
{
	SIZE tWindowSize = {300, 300};

	RECT rc = {
		static_cast<LONG>(fx), 
		static_cast<LONG>(fy), 
		static_cast<LONG>(fx + tWindowSize.cx),
		static_cast<LONG>(fy + tWindowSize.cy)};
	if (fx + tWindowSize.cx >= WINCX)
	{
		rc.left		= WINCX - tWindowSize.cx;
		rc.right	= WINCX;
	}

	if (fy + tWindowSize.cy >= WINCY)
	{
		rc.top		= WINCY - tWindowSize.cy;
		rc.bottom	= WINCY;
	}

	HDC hMemDC = FIND_BITMAP_DC("ItemDiscWind");
	SIZE tSize = GET_BITMAP_SIZE("ItemDiscWind");

	BOOL bRender = GdiTransparentBlt(hDC,
		rc.left,
		rc.top,
		tWindowSize.cx,
		tWindowSize.cy,
		hMemDC,
		0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 50.f, rc.top + 10.f, FT_NORMAL, 3.f, m_strName);

	hMemDC = FIND_BITMAP_DC("IconWhite");
	tSize = GET_BITMAP_SIZE("IconWhite");

	bRender = GdiTransparentBlt(hDC, rc.left + 25, rc.top + 70,
		static_cast<int>(tSize.cx * 4.f),
		static_cast<int>(tSize.cy * 4.f),
		hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	IconRender(hDC, rc.left + 25 + tSize.cx * 4.f * 0.5f, rc.top + 70 + tSize.cy * 4.f * 0.5f);

	string strText = "Damage " + to_string(m_iAttack);
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 120.f, rc.top + 70.f, FT_NORMAL, 2.f, strText);

	//strText = "Attacks per second : " + to_string(m_iAttack);
	//GET_SINGLE(CFontManager)->Render(hDC, fx, fy - 150.f, FT_NORMAL, 1.f, strText);

	strText = "Common";
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 20.f, rc.top + 160.f, FT_NORMAL, 2.f, strText);

	strText = "Main Handed";
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 20.f, rc.top + 200.f, FT_NORMAL, 2.f, strText);

	strText = "\"Light, wielding sword\"";
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 20.f, rc.top + 240.f, FT_NORMAL, 2.f, strText);
}

void CShortSword::SwingEffect()
{
	float fx = m_tInfo.fx + m_tDir.fx * 50;
	float fy = m_tInfo.fy + m_tDir.fy * 50;

	m_tAttackBox.fx = fx;
	m_tAttackBox.fy = fy;
	GET_SINGLE(CEffectManager)->AddRotateEffect("Swing", L"Image/Effect/Swing/Swing", fx, fy, 160, 160, 3, m_fSwingAngle, 1.f);
}
