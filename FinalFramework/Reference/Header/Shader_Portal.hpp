#include "Shader_Base.hpp"

float		g_Alpha = 1.f;
float		g_Glow = 0.f;

float		g_fTime = 0.f;
float3		g_vScrollSpeeds;
float3		g_vScales;

float2		g_vDistortion1;
float2		g_vDistortion2;
float2		g_vDistortion3;

float		g_fDistortionScale;
float		g_fDistortionBias;

vector		g_vColor = vector(0.f, 0.f, 0.f, 1.f);

texture		g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;

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

texture			g_MaskTexture;

sampler MaskSampler = sampler_state
{
	texture = g_MaskTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_Mask2Texture;

sampler Mask2Sampler = sampler_state
{
	texture = g_Mask2Texture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct VS_IN
{
	vector				vPosition	: POSITION;
	float2				vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector				vPosition	: POSITION;
	float2				vTexUV		: TEXCOORD0;
	vector				vProjPos	: TEXCOORD1;
	float2				vTexCoord1	: TEXCOORD2;
	float2				vTexCoord2	: TEXCOORD3;
	float2				vTexCoord3	: TEXCOORD4;

};

VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV  = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

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
	float2			vTexUV		: TEXCOORD0;
	vector			vProjPos	: TEXCOORD1;
	float2			vTexCoord1	: TEXCOORD2;
	float2			vTexCoord2	: TEXCOORD3;
	float2			vTexCoord3	: TEXCOORD4;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};


struct PS_GLOW
{
	vector			vColor : COLOR0;
	vector			vNormal : COLOR1;
	vector			vShadow : COLOR2;
	vector			vDepth : COLOR3;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float2		vDepthUV = (float2)0.f;

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	vector vDepth = tex2D(DepthSampler, vDepthUV);

	if (vDepth.x >= In.vProjPos.z / In.vProjPos.w)
	{
		Out.vColor = tex2D(BaseSampler, In.vTexUV);
		Out.vColor.a = 1.f;
	}
	else
		Out.vColor = 0.f;


	//float2		vDepthUV = (float2)0.f;

	//// -1 -> 0, 1 -> 1 (투영좌표인 x값이 텍스처 uv로 좌표 변환)
	//vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;

	//// 1 -> 0, -1 -> 1(투영좌표인 y값이 텍스처 uv로 좌표 변환)
	//vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	//float	fViewZ = tex2D(DepthSampler, vDepthUV).y * 1000.f;



	//Out.vColor.a = Out.vColor.r;


	//Out.vColor.a = Out.vColor.a * saturate(vViewZ - In.vProjPos.w);

	return Out;
}

PS_GLOW		PS_NON_ALPHA_GLOW(PS_IN In)
{
	PS_GLOW			Out = (PS_GLOW)0;

	float2		vDepthUV = (float2)0.f;

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	vector vDepth = tex2D(DepthSampler, vDepthUV);

	if (vDepth.x < In.vProjPos.z / In.vProjPos.w)
	{
		Out.vColor = 0.f;

		return Out;
	}

	vector vColor	= tex2D(BaseSampler, In.vTexUV);
	vector vMask	= tex2D(MaskSampler, In.vTexUV);
	//vector vMask = tex2D(MaskSampler, In.vTexUV);
	//Out.vColor *= vMask;

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

	Out.vColor.rgb = vFireColor.rgb * vFireColor.a + g_vColor.rgb * (1.f - vFireColor.a);
	//Out.vColor.rgb = vFireColor.rgb;
	//Out.vColor.a = vMask * g_Alpha;
	if (In.vTexUV.y < 0.5f)
		Out.vColor.a = vMask * g_Alpha;
	else
		Out.vColor.a = g_Alpha;

	Out.vShadow = tex2D(BaseSampler, In.vTexUV);
	Out.vShadow.ra = 1.f;
	Out.vShadow.g = 0.f;
	Out.vShadow.b = g_Glow;

	return Out;
}

technique Default_Device
{
	pass
	{
		alphablendenable = true;
		srcblend = srcalpha;	
		destblend = invsrcalpha;
		CullMode = None;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
	pass Glow
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		CullMode = None;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_NON_ALPHA_GLOW();
	}
};