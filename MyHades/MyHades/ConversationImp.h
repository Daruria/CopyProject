#pragma once
#include "EffectImp.h"
class CConversationImp :
	public CEffectImp
{
private:
	enum STATE { INTRO, STOP, OUTRO, END };

public:
	CConversationImp();
	virtual ~CConversationImp();

public:// Inherited via CEffectImp
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const D3DXMATRIX * pMatrix) override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CConversationImp* Create(const wstring& wstrNameKey, const wstring& wstrPortraitKey, const wstring& wstrSpeaker, const wstring& wstrTalk, int iTalkNum);

private:
	int StateUpdate();
	int IntroUpdate();
	int StopUpdate();
	int OutroUpdate();

private:
	wstring m_wstrNameKey;
	wstring m_wstrPortraitKey;
	wstring m_wstrSpeaker;
	wstring m_wstrTalk;
	int		m_iTalkNum;

	FRAME	m_tFrame;

	CConversationImp::STATE m_eCurState;
};

