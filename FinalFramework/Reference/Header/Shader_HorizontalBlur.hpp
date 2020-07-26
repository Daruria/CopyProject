#include "Shader_Base.hpp"

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

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	//Out.vColor.rgb *= float3(1.f, 0.9f, 0.8f);
	
	Out.vColor.a = 1.f;

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

};