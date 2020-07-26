#include "Shader_Base.hpp"

float			g_WINCY;
float			g_Alpha = 1.f;
float			g_Glow = 0.f;

vector			g_vLightDir = {1.f, -1.f, -1.f, 0.f};

texture			g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

texture			g_MaskTexture;

sampler MaskSampler = sampler_state
{
	texture = g_MaskTexture;

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
	
	return Out;
}

struct  PS_IN
{
	float2			vTexUV		: TEXCOORD0;
	vector			vProjPos	: TEXCOORD1;
};

struct PS_OUT
{
	vector			vColor	: COLOR0;
	vector			vNormal : COLOR1;
	vector			vShadow : COLOR2;
	vector			vDepth	: COLOR3;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	vector vMask = tex2D(MaskSampler, In.vTexUV);

	float2	vDepthUV;
	
	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	vector vDepth = tex2D(DepthSampler, vDepthUV);

	if (vDepth.x >= In.vProjPos.z / In.vProjPos.w)
	{
		Out.vColor *= vMask;
		Out.vColor.a = vMask.r * g_Alpha;
	}
	else
		Out.vColor = 0.f;



	//float2		vDepthUV = (float2)0.f;

	//// -1 -> 0, 1 -> 1 (투영좌표인 x값이 텍스처 uv로 좌표 변환)

	//vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;

	//// 1 -> 0, -1 -> 1(투영좌표인 y값이 텍스처 uv로 좌표 변환)
	//vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	//float	vViewZ = tex2D(DepthSampler, vDepthUV).y * 1000.f;

	//Out.vColor.a = Out.vColor.a * saturate(vViewZ - In.vProjPos.w);

	//Out.vColor.a = 1.f;

	return Out;
}


struct VS_IN_PARTICLE
{
	float3		vPosition : POSITION;
	float		fPSize	: PSIZE;
	float2		vTexUV	: TEXCOORD0;

	float4		vRight	: TEXCOORD1;
	float4		vUp		: TEXCOORD2;
	float4		vLook	: TEXCOORD3;
	float4		vPos	: TEXCOORD4;

};

struct VS_OUT_PARTICLE
{
	float4		vPosition : POSITION;
	float		fPSize : PSIZE;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD5;
};

VS_OUT_PARTICLE	VS_PARTICLE(VS_IN_PARTICLE In)
{
	VS_OUT_PARTICLE	Out = (VS_OUT_PARTICLE)0;

	matrix matWorld = mul(float4x4(In.vRight, In.vUp, In.vLook, In.vPos), g_matWorld);
	matrix matWV, matWVP;

	matWV = mul(matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;

	// 화면 해상도 높이값(필요에 따라 변경 해야 함)
	Out.fPSize = matWorld._11 * (g_WINCY * In.fPSize) * sqrt(1.f / pow(Out.vPosition.w, 2.f));

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN_PARTICLE
{
	float4			vPosition : POSITION;
	float			fPSize	: PSIZE;
	float2			vTexUV : TEXCOORD0;
	vector			vProjPos : TEXCOORD5;
};

struct PS_OUT_PARTICLE
{
	vector			vColor : COLOR0;
};

PS_OUT		PS_PARTICLE(PS_IN_PARTICLE In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	vector vMask = tex2D(MaskSampler, In.vTexUV);

	float2	vDepthUV;

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	vector vDepth = tex2D(DepthSampler, vDepthUV);

	if (vDepth.x >= In.vProjPos.z / In.vProjPos.w)
	{
		Out.vColor *= vMask;
		Out.vColor.a = vMask.r * g_Alpha;
	}
	else
		Out.vColor = 0.f;

	return Out;

}


PS_OUT	PS_GLOW(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	vector vMask = tex2D(MaskSampler, In.vTexUV);

	float2	vDepthUV;

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	vector vDepth = tex2D(DepthSampler, vDepthUV);

	if (vDepth.x >= In.vProjPos.z / In.vProjPos.w)
	{
		Out.vColor *= vMask;
		Out.vColor.a *= vMask * g_Alpha;

		Out.vShadow = tex2D(BaseSampler, In.vTexUV);
		Out.vShadow.ra = 1.f;
		Out.vShadow.gb = 0.f;

		Out.vDepth.b = g_Glow;
	}
	else
	{
		Out.vColor = 0.f;
		Out.vShadow.b = 0.f;
	}

	return Out;
}

PS_OUT	PS_NO_DEPTH_GLOW(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	vector vMask = tex2D(MaskSampler, In.vTexUV);

	Out.vColor *= vMask;
	Out.vColor.a *= vMask * g_Alpha;

	Out.vShadow = tex2D(BaseSampler, In.vTexUV);
	Out.vShadow.ra = 1.f;
	Out.vShadow.g = 0.f;
	Out.vShadow.b = g_Glow;

	return Out;
}

PS_OUT	PS_DISTORTION(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2	vDepthUV;

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	vector vDepth = tex2D(DepthSampler, vDepthUV);

	if (vDepth.x >= In.vProjPos.z / In.vProjPos.w)
	{
		Out.vColor = tex2D(MaskSampler, In.vTexUV);
	}
	else
		Out.vColor = 0.f;

	return Out;
}

PS_OUT PS_NO_DEPTH(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	vector vMask = tex2D(MaskSampler, In.vTexUV);

	Out.vColor *= vMask;
	Out.vColor.a = vMask.r * g_Alpha;

	return Out;
}

technique Default_Device
{
	pass	//	0
	{
		alphablendenable = true;
		srcblend = srcalpha;	
		destblend = invsrcalpha;
		CullMode = None;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass PointSprite	//	1
	{
		pointspriteenable = true;
		zwriteenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		CullMode = None;

		vertexshader = compile vs_3_0	VS_PARTICLE();
		pixelshader = compile ps_3_0	PS_PARTICLE();

	}

	pass Glow	//	2
	{
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		CullMode = None;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_GLOW();
	}

	pass	//	3
	{
		ZWriteEnable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		CullMode = None;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	//	4
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		CullMode = None;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_NO_DEPTH_GLOW();
	}

	pass	//	5
	{
		ZWriteEnable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		CullMode = None;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	//	6
	{
		ZWriteEnable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		CullMode = None;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_NO_DEPTH();
	}
};