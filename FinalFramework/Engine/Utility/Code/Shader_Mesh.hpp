#include "Shader_Base.hpp"

texture		g_NormalTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_MreTexture;

sampler MreSampler = sampler_state
{
	texture = g_MreTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_LightDepthTexture;

sampler LightDepthSampler = sampler_state
{
	texture = g_LightDepthTexture;
};

vector	g_vLightPos;
matrix	g_matLightView;
matrix	g_matLightProj;

texture g_DissolveTexture;

sampler DissolveSampler = sampler_state
{
	texture = g_DissolveTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

float	g_fTime;
vector	g_vDissolveColor = vector(1.f, 0.f, 0.f, 1.f);
float	g_fRim = 0.f;
float	g_fAlpha = 1.f;
float	g_fCapture = 0.f;

struct VS_IN
{
	vector		vPosition	: POSITION;	
	vector		vNormal		: NORMAL;
	vector		vTangent	: TANGENT;
	vector		vBinormal	: BINORMAL;
	float2		vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition		: POSITION;
	vector		vNormal			: NORMAL;
	vector		vTangent		: TANGENT;
	vector		vBinormal		: BINORMAL;
	float2		vTexUV			: TEXCOORD0;
	vector		vProjPos		: TEXCOORD1;
	vector		vLightViewPos	: TEXCOORD2;
};

// 버텍스 쉐이더 함수
VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	vector vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vTangent = normalize(mul(vector(In.vTangent.xyz, 0.f), g_matWorld));
	Out.vBinormal = normalize(mul(vector(In.vBinormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	
	//	Calculate about Light
	Out.vLightViewPos = mul(vWorldPos, g_matLightView);
	Out.vLightViewPos = mul(Out.vLightViewPos, g_matLightProj);

	return Out;
}

struct PS_IN // 픽셀 쉐이더 구조체에서 POSITION이란 Semantic은 사용할 수 없다.
{
	vector			vNormal		: NORMAL;
	vector			vTangent	: TANGENT;
	vector			vBinormal	: BINORMAL;
	float2			vTexUV		: TEXCOORD0;
	vector			vProjPos	: TEXCOORD1;
	vector			vLightViewPos : TEXCOORD2;
};

struct PS_OUT
{
	vector			vColor		: COLOR0;
	vector			vNormal		: COLOR1;
	vector			vShadow		: COLOR2;
	vector			vDepth		: COLOR3;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.a = g_fAlpha;
	
	Out.vNormal = tex2D(NormalSampler, In.vTexUV);

	Out.vNormal = normalize((Out.vNormal * 2.f) - 1.f);
	
	Out.vNormal = normalize(In.vNormal + Out.vNormal.x * In.vTangent + Out.vNormal.y * In.vBinormal);
	Out.vNormal.a = 1.f;


	float  fShadow = 1.f;


	Out.vShadow = 1.f;

	float2 vUV;

	float fLightDist = In.vLightViewPos.z / In.vLightViewPos.w;

	vUV.x = (In.vLightViewPos.x / In.vLightViewPos.w) * 0.5f + 0.5f;
	vUV.y = -(In.vLightViewPos.y / In.vLightViewPos.w) * 0.5f + 0.5f;

	float fLightDepth = tex2D(LightDepthSampler, vUV).r;

	if (fLightDist > fLightDepth + 0.0000125f)
	{
		Out.vShadow.r *= 0.5f;
	}

	//Out.vShadow.gb = vMre.gb;
	//Out.vShadow.gb = 0.f;
	Out.vShadow.g = g_fRim;
	Out.vShadow.b = g_fCapture;

	vector vMre = tex2D(MreSampler, In.vTexUV);

	//Out.vColor.rgb *= (1.f - vMre.r);

	float fDepth = In.vProjPos.z / In.vProjPos.w;
	Out.vDepth	= vector(fDepth,	// z나누기를 수행하여 0~1사이의 투영 z값을 만들고, 이를 텍스쳐 uv좌표로 판단
						In.vProjPos.w * 0.001f,			// 뷰스페이스 상태의 z값을 텍스쳐의 uv로 변환
						vMre.r,
						1.f);

	//Out.vShadow.g = min(In.vVelocity.x, 10.f);
	//Out.vShadow.b = min(In.vVelocity.y, 10.f);

	//Out.vShadow.g = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	//Out.vShadow.b = (In.vProjPos.y / In.vProjPos.w) * 0.5f + 0.5f;

	//Out.vShadow.gb = (In.vProjPos.xy / In.vProjPos.w) * 0.5f + 0.5f;

	//Out.vShadow.g = In.vVelocity.x;
	//Out.vShadow.b = In.vVelocity.y;

	//Out.vShadow.g = saturate((In.vVelocity.x + In.vVelocity.y + In.vVelocity.z) / 3);

	//Out.vShadow = In.vVelocity;


	return Out;
}


PS_OUT		PS_ALPHA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);		

	return Out;
}

PS_OUT	PS_DISSOLVE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vDiffuse		= tex2D(BaseSampler,		In.vTexUV);
	vector vDissolve	= tex2D(DissolveSampler,	In.vTexUV);

	//float fDif		= 20.f * vDissolve.r * (sin(g_fTime) + 0.5f);
	//float fAlpha	= 5.1f * vDissolve.r *(sin(g_fTime) + 0.5f);
	//
	//fDif = saturate(pow(max(fDif, 0.f), 20));
	//fAlpha = saturate(pow(max(fAlpha, 0.f), 20));

	//if (fDif > 0.2f)
	//	Out.vColor = vector(vDiffuse.rgb, 1.f);
	//else if (fDif > 0.1f)
	//	Out.vColor = vector(g_vDissolveColor.rgb, 1.f);
	//else
	//	Out.vColor = 0.f;

	float fMulti1 = ((vDissolve.r	* sin(g_fTime)) * 2.8f);
	float fMulti2 = (vDissolve.r	* sin(g_fTime));

	float fMulti3 = ((vDissolve.r	* cos(g_fTime)) * 20.f);
	float fMulti4 = (vDissolve.r	* cos(g_fTime));

	float b = saturate(pow(max(fMulti3 + fMulti4, 0.f), 20));

	float fAlpha = saturate(pow(max(fMulti3 + fMulti4, 0.f), 20));

	float3 Ke = float3(1.f, 1.f, 1.f);

	if (b >= 0.95f)
		Ke = float3(1.f, 1.f, 1.f);
	else if (b >= 0.1f)
		Ke = g_vDissolveColor.rgb * 100.f;
	else
		Ke = float3(0.f, 0.f, 0.f);
		//Ke = g_vAfterColor.rgb;

	Out.vColor = vector((Ke.rgb * vDiffuse.rgb), fAlpha);



	Out.vNormal = normalize((tex2D(NormalSampler, In.vTexUV) * 2.f) - 1.f);

	Out.vNormal = normalize(In.vNormal + Out.vNormal.x * In.vTangent + Out.vNormal.y * In.vBinormal);
	Out.vNormal.a = fAlpha;

	vector vMre = tex2D(MreSampler, In.vTexUV);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,	In.vProjPos.w * 0.001f, vMre.r, 1.f);

	vector vColor = 1.f;

	float2 vUV;

	float fLightDist = In.vLightViewPos.z / In.vLightViewPos.w;

	vUV.x = (In.vLightViewPos.x / In.vLightViewPos.w) * 0.5f + 0.5f;
	vUV.y = -(In.vLightViewPos.y / In.vLightViewPos.w) * 0.5f + 0.5f;

	float fLightDepth = tex2D(LightDepthSampler, vUV).r;

	if (fLightDist > fLightDepth + 0.0000125f)
	{
		vColor.r *= 0.5f;
	}

	Out.vShadow = vColor;


	Out.vShadow.gb = 0.f;
	//Out.vShadow.gb = (In.vProjPos.xy / In.vProjPos.w) * 0.5f + 0.5f;



	return Out;
}

PS_OUT	PS_GLOW(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.a = 1.f;

	Out.vNormal = tex2D(NormalSampler, In.vTexUV);

	Out.vNormal = normalize((Out.vNormal * 2.f) - 1.f);

	Out.vNormal = normalize(In.vNormal + Out.vNormal.x * In.vTangent + Out.vNormal.y * In.vBinormal);
	Out.vNormal.a = 1.f;

	vector vMre = tex2D(MreSampler, In.vTexUV);

	float fDepth = In.vProjPos.z / In.vProjPos.w;
	Out.vDepth = vector(fDepth,	In.vProjPos.w * 0.001f, vMre.r, 1.f);

	Out.vShadow = 1.f;

	float2 vUV;

	float fLightDist = In.vLightViewPos.z / In.vLightViewPos.w;

	vUV.x = (In.vLightViewPos.x / In.vLightViewPos.w) * 0.5f + 0.5f;
	vUV.y = -(In.vLightViewPos.y / In.vLightViewPos.w) * 0.5f + 0.5f;

	float fLightDepth = tex2D(LightDepthSampler, vUV).r;

	if (fLightDist > fLightDepth + 0.0000125f)
	{
		Out.vShadow.r *= 0.5f;
	}

	Out.vShadow.gb = 0.f;
	//Out.vShadow.gb = (In.vProjPos.xy / In.vProjPos.w) * 0.5f + 0.5f;

	return Out;
}

technique Default_Device
{
	pass	//	0
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = compile vs_3_0 VS_MAIN();		
		pixelshader  = compile ps_3_0 PS_MAIN();	
	}


	pass AlphaTesting // 1
	{
		alphatestenable = true;
		alphafunc = greater;
		alpharef = 0xc0;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_ALPHA();
	}

	pass Dissolve	//	2
	{
		CullMode = None;
		AlphaBlendEnable = true;
		DestBlend = InvSrcAlpha;
		SrcBlend = SrcAlpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_DISSOLVE();
	}

	pass Glow		//	3
	{
		CullMode = None;
		AlphaBlendEnable = true;
		DestBlend = InvSrcAlpha;
		SrcBlend = SrcAlpha;
		

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_GLOW();
	}
};