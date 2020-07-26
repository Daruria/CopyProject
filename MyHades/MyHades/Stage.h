#pragma once
#include "Scene.h"
class CScreenEffect;
class CStage :
	public CScene
{
public:
	enum STATE { INTRO, PLAY, OUTRO, END };

protected:
	CStage();
public:
	virtual ~CStage();

public:// Inherited via CScene
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	
protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

protected:
	void AddIntro();
	void AddOutro();
	void AddBoonOrb();
	void EButtonRender();

protected:
	CStage::STATE m_eCurState;
	SCENE_TYPE	m_eNextStage;
	bool	m_bIsAddBoonOrb;
	bool	m_bEButtonRender;
};

