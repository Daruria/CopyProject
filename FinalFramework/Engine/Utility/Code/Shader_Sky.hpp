#include "Shader_Base.hpp"

texture		g_DissolveTexture;

sampler DissolveSampler = sampler_state
{
	texture = g_DissolveTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

vector	g_vDissolveColor = vector(1.f, 0.f, 0.f, 1.f);

float	g_fTime = 0.f;
vector	g_vColor = vector(1.f, 1.f, 1.f, 1.f);
vector	g_vAfterColor = vector(1.f, 0.75f, 0.5f, 1.f);

struct VS_IN
{
	vector				vPosition	: POSITION;
	float2				vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector				vPosition	: POSITION;
	float2				vTexUV		: TEXCOORD0;
};

VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	Out.vPosition = mul(Out.vPosition, g_matView);
	Out.vPosition = mul(Out.vPosition, g_matProj);
	
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct  PS_IN
{
	float2			vTexUV		: TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV) * g_vColor;
	
	Out.vColor.a = 1.f;

	return Out;
}

PS_OUT		PS_DISSOLVE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector vDiffuse = tex2D(BaseSampler, In.vTexUV);
	vector vDissolve = tex2D(DissolveSampler, In.vTexUV);

	float fMulti1 = ((vDissolve.r * sin(g_fTime)) * 2.8f);
	float fMulti2 = (vDissolve.r * sin(g_fTime));

	float fMulti3 = ((vDissolve.r * cos(g_fTime)) * 20.f);
	float fMulti4 = (vDissolve.r * cos(g_fTime));

	float b = saturate(pow(max(fMulti3 + fMulti4, 0.f), 20));

	float fAlpha = saturate(pow(max(fMulti3 + fMulti4, 0.f), 20));

	float3 Ke = float3(1.f, 1.f, 1.f);

	if (b >= 0.95f)
		Ke = float3(1.f, 1.f, 1.f);
	else if (b >= 0.1f)
		Ke = g_vDissolveColor.rgb * 100.f;
	else
		//Ke = float3(0.f, 0.f, 0.f);
		Ke = g_vAfterColor.rgb;

	Out.vColor = vector((Ke.rgb * vDiffuse.rgb), fAlpha);



	return Out;
}


technique Default_Device
{
	pass
	{
		ZWriteEnable = false;
		CullMode = None;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass Dissolve
	{
		ZWriteEnable = false;
		CullMode = None;
	
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_DISSOLVE();
	}

};