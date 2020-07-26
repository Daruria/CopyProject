#include "Shader_Base.hpp"

texture			g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

float			g_Alpha = 1.f;

float			g_fTime = 0.f;
float3			g_vScrollSpeeds;
float3			g_vScales;

texture			g_MaskTexture;

sampler MaskSampler = sampler_state
{
	texture = g_MaskTexture;
	
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_NoiseTexture;

sampler NoiseSampler = sampler_state
{
	texture = g_NoiseTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

float2	g_vDistortion1;
float2	g_vDistortion2;
float2	g_vDistortion3;

float	g_fDistortionScale;
float	g_fDistortionBias;


struct VS_IN
{
	vector				vPosition	: POSITION;
	float2				vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector				vPosition	: POSITION;
	float2				vTexUV		: TEXCOORD0;
	float2				vTexCoord1	: TEXCOORD1;
	float2				vTexCoord2	: TEXCOORD2;
	float2				vTexCoord3	: TEXCOORD3;
	vector				vProjPos	: TEXCOORD4;
};

VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV  = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;


	Out.vTexCoord1 = (In.vTexUV * g_vScales.x);
	Out.vTexCoord1.y = Out.vTexCoord1.y + (g_fTime * g_vScrollSpeeds.x);

	Out.vTexCoord2 = (In.vTexUV * g_vScales.y);
	Out.vTexCoord2.y = Out.vTexCoord2.y + (g_fTime * g_vScrollSpeeds.y);

	Out.vTexCoord3 = (In.vTexUV * g_vScales.z);
	Out.vTexCoord3.y = Out.vTexCoord3.y + (g_fTime * g_vScrollSpeeds.z);

	return Out;
}

struct  PS_IN
{
	float2				vTexUV : TEXCOORD0;
	float2				vTexCoord1 : TEXCOORD1;
	float2				vTexCoord2 : TEXCOORD2;
	float2				vTexCoord3 : TEXCOORD3;
	vector				vProjPos : TEXCOORD4;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	
	float2	vDepthUV;

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	vector vDepth = tex2D(DepthSampler, vDepthUV);

	if (vDepth.x <  In.vProjPos.z / In.vProjPos.w)
	{
		Out.vColor = 0.f;
		return Out;
	}

	float4 vNoise1, vNoise2, vNoise3, vFinalNoise, vFireColor, vAlphaColor;
	float	fPerturb;
	float2	vNoiseCoords;

	vNoise1 = tex2D(NoiseSampler, In.vTexCoord1);
	vNoise2 = tex2D(NoiseSampler, In.vTexCoord2);
	vNoise3 = tex2D(NoiseSampler, In.vTexCoord3);

	vNoise1 = (vNoise1 - 0.5f) * 2.f;
	vNoise2 = (vNoise2 - 0.5f) * 2.f;
	vNoise3 = (vNoise3 - 0.5f) * 2.f;

	vNoise1.xy = vNoise1.xy * g_vDistortion1.xy;
	vNoise2.xy = vNoise2.xy * g_vDistortion2.xy;
	vNoise3.xy = vNoise3.xy * g_vDistortion3.xy;

	vFinalNoise = vNoise1 + vNoise2 + vNoise3;

	//fPerturb = ((1.f - In.vTexUV.y) * g_fDistortionScale) + g_fDistortionBias;

	vNoiseCoords.xy = (vFinalNoise.xy * 0.05f) + In.vTexUV.xy;

	vFireColor = tex2D(BaseSampler, vNoiseCoords.xy);

	vAlphaColor = tex2D(MaskSampler, vNoiseCoords.xy);
	//vAlphaColor = tex2D(AlphaSampler, In.vTexUV.xy);

	vFireColor.a = vAlphaColor * g_Alpha;

	Out.vColor = vFireColor;

	return Out;
}

technique Default_Device
{
	pass
	{
		ZWriteEnable = false;
		alphablendenable = true;
		srcblend = srcalpha;	
		destblend = invsrcalpha;
		CullMode = None;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};