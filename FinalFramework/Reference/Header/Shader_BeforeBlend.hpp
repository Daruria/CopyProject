texture				g_PreDepthTexture;

sampler PreDepthSampler = sampler_state
{
	texture = g_PreDepthTexture;
};

texture			g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

texture g_CaptureTexture;

sampler CaptureSampler = sampler_state
{
	texture = g_CaptureTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	AddressU = clamp;
	AddressV = clamp;
};


float	g_fScreeWidth;
float	g_fScreeHeight;

float	g_fWeight0 = 1.f;
float	g_fWeight1 = 0.9f;
float	g_fWeight2 = 0.55f;
float	g_fWeight3 = 0.18f;
float	g_fWeight4 = 0.1f;

matrix	g_matView;
matrix	g_matProj;

matrix	g_matViewInv;
matrix	g_matProjInv;
matrix	g_matPreViewInv;

bool	g_bBlur = false;

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//vector vBlend	= tex2D(BlendSampler, In.vTexUV);
	vector vPreDepth = tex2D(PreDepthSampler, In.vTexUV);
	vector vDepth	= tex2D(DepthSampler, In.vTexUV);

	float fTexelWidthSize = 1.f / g_fScreeWidth;
	float fTexelHeightSize = 1.f / g_fScreeHeight;

	vector vPrePos, vCurPos, vDir;

	float fCurW = vDepth.y * 1000.f, fPreW = vPreDepth.y * 1000.f;

	vCurPos.x = (In.vTexUV * 2.f - 1.f) * fCurW;
	vCurPos.y = (In.vTexUV * -2.f + 1.f) * fCurW;
	vCurPos.z = vDepth.x * fCurW;
	vCurPos.w = fCurW;

	vCurPos = mul(vCurPos, g_matProjInv);
	vCurPos = mul(vCurPos, g_matViewInv);

	vPrePos.x = (In.vTexUV * 2.f - 1.f) * fPreW;
	vPrePos.y = (In.vTexUV * -2.f + 1.f) * fPreW;
	vPrePos.z = vPreDepth.x * fPreW;
	vPrePos.w = fPreW;

	vPrePos = mul(vPrePos, g_matProjInv);
	vPrePos = mul(vPrePos, g_matPreViewInv);

	vDir = vPrePos - vCurPos;
	vDir.w = 0.f;
	vDir = mul(vDir, g_matView);
	vDir = mul(vDir, g_matProj);
	vDir.x = -vDir.x * 0.1f;
	vDir.y = vDir.y * 0.1f;

	Out.vColor = tex2D(CaptureSampler, In.vTexUV + vDir);
	//Out.vColor = vDir;


	//Out.vColor = vBlend;

	//float fWeight0, fWeight1, fWeight2, fWeight3, fWeight4;
	//float fNormalization;

	//fWeight0 = g_fWeight0;
	//fWeight1 = g_fWeight1;
	//fWeight2 = g_fWeight2;
	//fWeight3 = g_fWeight3;
	//fWeight4 = g_fWeight4;

	////fNormalization = (fWeight0 + 2.f * (fWeight1 + fWeight2 + fWeight3 + fWeight4));
	//fNormalization = (fWeight0 + fWeight1 + fWeight2 + fWeight3 + fWeight4);

	//fWeight0 = fWeight0 / fNormalization;
	//fWeight1 = fWeight1 / fNormalization;
	//fWeight2 = fWeight2 / fNormalization;
	//fWeight3 = fWeight3 / fNormalization;
	//fWeight4 = fWeight4 / fNormalization;

	//float2 f2TexUV1, f2TexUV2, f2TexUV3, f2TexUV4, f2TexUV5, f2TexUV6, f2TexUV7, f2TexUV8, f2TexUV9;

	//f2TexUV1 = In.vTexUV + float2(fTexelWidthSize * vShadow.g * 4.f, fTexelHeightSize * vShadow.b * 4.f);
	//f2TexUV2 = In.vTexUV + float2(fTexelWidthSize * vShadow.g * 3.f, fTexelHeightSize * vShadow.b * 3.f);
	//f2TexUV3 = In.vTexUV + float2(fTexelWidthSize * vShadow.g * 2.f, fTexelHeightSize * vShadow.b * 2.f);
	//f2TexUV4 = In.vTexUV + float2(fTexelWidthSize * vShadow.g * 1.f, fTexelHeightSize * vShadow.b * 1.f);
	//f2TexUV5 = In.vTexUV + float2(fTexelWidthSize * vShadow.g * 0.f, fTexelHeightSize * vShadow.b * 0.f);

	////f2TexUV1 = In.vTexUV + float2(fTexelWidthSize * -4.f, fTexelHeightSize * -4.f);
	////f2TexUV2 = In.vTexUV + float2(fTexelWidthSize * -3.f, fTexelHeightSize * -3.f);
	////f2TexUV3 = In.vTexUV + float2(fTexelWidthSize * -2.f, fTexelHeightSize * -2.f);
	////f2TexUV4 = In.vTexUV + float2(fTexelWidthSize * -1.f, fTexelHeightSize * -1.f);
	////f2TexUV5 = In.vTexUV + float2(fTexelWidthSize * 0.f, fTexelHeightSize * 0.f);
	////f2TexUV6 = In.vTexUV + float2(fTexelWidthSize * 1.f, fTexelHeightSize * 1.f);
	////f2TexUV7 = In.vTexUV + float2(fTexelWidthSize * 2.f, fTexelHeightSize * 2.f);
	////f2TexUV8 = In.vTexUV + float2(fTexelWidthSize * 3.f, fTexelHeightSize * 3.f);
	////f2TexUV9 = In.vTexUV + float2(fTexelWidthSize * 4.f, fTexelHeightSize * 4.f);

	//vBlurred += tex2D(BlendSampler, f2TexUV1) * fWeight4;
	//vBlurred += tex2D(BlendSampler, f2TexUV2) * fWeight3;
	//vBlurred += tex2D(BlendSampler, f2TexUV3) * fWeight2;
	//vBlurred += tex2D(BlendSampler, f2TexUV4) * fWeight1;
	//vBlurred += tex2D(BlendSampler, f2TexUV5) * fWeight0;

	//Out.vColor = vBlend * (1.f - vShadow.g) + vBlurred * vShadow.g;
	//Out.vColor = vBlurred;
	//Out.vColor = vBlend;
	return Out;
}

technique Default_Device
{
	pass	Final
	{
		zwriteEnable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}

};