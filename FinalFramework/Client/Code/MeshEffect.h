#ifndef MeshEffect_h__
#define MeshEffect_h__

#include "Defines.h"
#include "StaticObject.h"

class CMeshEffect : public Engine::CStaticObject
{
protected:
	explicit CMeshEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMeshEffect();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;
	

public:
	void Set_Pos(const _vec3* pPos);
	void Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void Set_PosX(const _float& fX);
	void Set_PosY(const _float& fY);
	void Set_PosZ(const _float& fZ);

	void Set_Scale(const _vec3* pScale);
	void Set_Scale(const _float& fScale);
	void Set_Scale(const _float& fX, const _float& fY, const _float& fZ);
	void Set_ScaleX(const _float& fX);
	void Set_ScaleY(const _float& fY);
	void Set_ScaleZ(const _float& fZ);

	void Set_LifeTime(const _float& fLifeTime) { m_fLifeTime = fLifeTime; }
	void Set_DeltaScale(const _vec3* pDeltaScale) { memcpy(&m_vDeltaScale, pDeltaScale, sizeof(_vec3)); }
	void Set_MaxScale(const _vec3* pMaxScale) { memcpy(&m_vMaxScale, pMaxScale, sizeof(_vec3)); }
	void SetUp_ScaleChange(const _vec3* pStartScale, const _vec3* pMaxScale, const _float& fScalingSpeed);
	void Set_DissolveStartTime(const _float& fStartTime) { m_fDissolveStartTime = fStartTime; }

	void Set_Rotation(const _vec3* pRot);
	void Set_Rotation(const _float& fX, const _float&fY, const _float& fZ);
	void Set_RotationX(const _float& fX);
	void Set_RotationY(const _float& fY);
	void Set_RotationZ(const _float& fZ);

	void Start_Dissolve() { m_bDissolve = true; }

	void Get_Pos(_vec3* pOut);

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;


private:
	_float		m_fLifeTime = 1.f;
	_float		m_fLifeCnt = 0.f;
	_vec3		m_vDeltaScale = { 0.f, 0.f, 0.f };
	_vec3		m_vMaxScale = { 0.f, 0.f, 0.f };

	class Engine::CTexture* m_pDissolveTextureCom = nullptr;
	_bool					m_bDissolve = false;
	_float					m_fDissolveTime = 0.f;
	_float					m_fDissolveStartTime = 1.f;

public:
	static CMeshEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag);

protected:
	virtual void Free() override;
};

#endif // MeshEffect_h__
