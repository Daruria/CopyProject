#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl : public CBase
{
private:
	explicit CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAniCtrl(const CAniCtrl& rhs);
	virtual ~CAniCtrl();

public: 
	LPD3DXANIMATIONCONTROLLER		Get_AnimationCtrl() { return m_pAniCtrl; }
	_uint							Get_AnimationMax();
	_double							Get_AnimationProgress();
	_double							Get_AnimationPosition();

	_bool							Is_AnimationSetEnd();

public:
	HRESULT			Ready_AnimationCtrl();

	void			Set_AnimationSet(const _uint& iIndex, const _double& dInterpolationTime);
	void			Set_AnimationPosition(const _double& dPosition);

	void			Play_Animation(const _float& fTimeDelta);
	_bool			Is_InItpProgress() const { return m_fAccTime < m_dInterpolationTime; }

	

private:
	LPD3DXANIMATIONCONTROLLER			m_pAniCtrl;

	_uint								m_iCurrentTrack;
	_uint								m_iNewTrack;

	_float								m_fAccTime;
	_uint								m_iOldAniIdx;

	_double								m_dInterpolationTime = 0.05;
	_double								m_dPeriod = 0.0;

public:
	static CAniCtrl*			Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAniCtrl*			Create(const CAniCtrl& rhs);
	virtual void				Free(void);
};

END

#endif // AniCtrl_h__
