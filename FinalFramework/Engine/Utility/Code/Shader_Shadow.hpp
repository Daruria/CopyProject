matrix		g_matWorld;			// 상수 테이블
matrix		g_matLightView;
matrix		g_matLightProj;

struct VS_IN
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition		: POSITION;
	float2		vTexUV			: TEXCOORD0;
	vector		vPosAtLight		: TEXCOORD1;
};


VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	vector vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);

	Out.vPosition = mul(vWorldPos, g_matLightView);
	Out.vPosition = mul(Out.vPosition, g_matLightProj);

	Out.vTexUV = In.vTexUV;

	Out.vPosAtLight = Out.vPosition;


	return Out;
}

struct PS_IN // 픽셀 쉐이더 구조체에서 POSITION이란 Semantic은 사용할 수 없다.
{
	float2		vTexUV			: TEXCOORD0;
	vector		vPosAtLight		: TEXCOORD1;
};

struct PS_OUT
{
	vector			vShadowDepth	: COLOR0;
	//vector			vLightPos		: COLOR1;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float fLightDepth = In.vPosAtLight.z / In.vPosAtLight.w;

	Out.vShadowDepth = vector(fLightDepth, 0.f, 0.f, 1.f);

	//Out.vShadowDepth = tex2D(BaseSampler, In.vTexUV);





	//float fLightDepth = In.vPosAtLight.z / In.vPosAtLight.w;

	//float fDepth = tex2D(DepthSampler, vUV).r;

	//vector vLightToWorld = In.vLightToWorld;
	//float fLength = length(normalize(vLightToWorld.xyz));

	//if (fLightDepth > fDepth + 0.001f)
	//	Color.rgb *= 0.5f;

	//Out.vShadowDepth = Color;



	return Out;
}


technique Default_Device
{
	pass
	{
	//alphablendenable = true;
	//srcblend = srcalpha;
	//destblend = invsrcalpha;
		//ZWriteEnable = false;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};