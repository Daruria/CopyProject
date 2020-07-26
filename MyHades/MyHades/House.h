#pragma once
#include "Stage.h"
class CHouse :
	public CStage
{
private:
	CHouse();
public:
	virtual ~CHouse();

public:// Inherited via CScene
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

	MACRO_CREATE(CHouse)

private:
	void TitleCheck();

private:
	bool	m_bShowTitle;

};

