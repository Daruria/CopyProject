#pragma once
#include "Scene.h"
class CEnding :
	public CScene
{
private:
	enum STATE { REM_BG_INTRO, REM_BG, PLAYER_DIE, END };

public:
	CEnding();
	virtual ~CEnding();

public:// Inherited via CScene
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

	MACRO_CREATE(CEnding)

private:
	int StateUpdate();
	void Animation();

private:
	CEnding::STATE m_eCurState;
	FRAME		m_tFrame;
	wstring		m_wstrStateKey;
	bool		m_bOnceAnimFinish;
};

