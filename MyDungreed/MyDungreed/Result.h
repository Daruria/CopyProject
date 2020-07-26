#pragma once

enum RESULT_PHASE{ RP_END_OF_ADV, RP_TITLE_MOVE_UP, RP_RESULT, RP_LEVEL, RP_EAT_ITEM, RP_EXIT, RP_END };
class CInvenSlot;
class CAnimatedBGObject;
class CMyButton;
class CGoldCoin;
class CBGImage;
class CResult
{
	DECLARE_SINGLE(CResult)

public:
	CResult();
	~CResult();

private:
	bool			m_bAdventureSuccess;
	bool			m_bPlayStart;
	bool			m_bShowResult;
	bool			m_bSlotCopy;
	int				m_iMonsterCount;
	int				m_iPlayTime;
	int				m_iExp;
	int				m_iAddMonster;
	int				m_iEatCoinCool;
	float			m_fLevelRatio;
	float			m_fItemEatSpeed;
	RESULT_PHASE	m_ePhase;
	DWORD			m_dwPlayTime;
	DWORD			m_dwPhaseCheck;
	DWORD			m_dwPrevTime;
	INFO			m_tTitleInfo;
	list<string>		m_ActivitiesList;
	list<CInvenSlot*>	m_ItemSlot;
	list<CGoldCoin*>	m_CoinList;
	CAnimatedBGObject*	m_pEatItem;
	CMyButton*			m_pExitButton;

private:
	bool			m_bLevelUp;
	bool			m_bLevelImgStop;
	float			m_fLevelUpImgRatio;
	float			m_fLevelUpRatioAcc;
	DWORD			m_dwLevelUpImgStop;
	INFO			m_tLevelUpImgInfo;

public:
	void Initialize();
	int Update();
	void Render(HDC hDC);

private:
	void Release();

public:
	void StartPlay()		{ m_bPlayStart = true; }
	void StopPlay()			{ m_bPlayStart = false; }
	void AdventureSuccess() { m_bAdventureSuccess = true; }
	void ShowResult();
	void QuitResult()		{ m_bShowResult = false; }
	void MonsterCount()		{ ++m_iMonsterCount; }
	void AddActivities(const string& strActivity);

private:
	void TimeUpdate();
	void EndOfAdventure(DWORD dwCurTime);
	void TitleMoveUp();
	void Result();
	void Leveling();
	void EatItems();
	int Exit();

public:
	const bool GetShowResult() const { return m_bShowResult; }

private:
	void TitleRender(HDC hDC);
	void ResultRender(HDC hDC);
	void EatItemRender(HDC hDC);
	void LevelBarRender(HDC hDC);

};