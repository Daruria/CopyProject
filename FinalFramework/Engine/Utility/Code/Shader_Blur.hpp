texture g_FilteredTexture;

sampler FilteredSampler = sampler_state
{
	texture = g_FilteredTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_ShadowTexture;

sampler ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

float	g_fScreeWidth;
float	g_fScreeHeight;

float	g_fWeight0 = 1.f;
float	g_fWeight1 = 0.9f;
float	g_fWeight2 = 0.55f;
float	g_fWeight3 = 0.18f;
float	g_fWeight4 = 0.1f;

struct  PS_IN
{
	float2			vTexUV		: TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT PS_HORIZONTAL_BLUR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float fTexelWidthSize = 1.f / g_fScreeWidth;

	float fWeight0, fWeight1, fWeight2, fWeight3, fWeight4;
	float fNormalization;

	fNormalization = (g_fWeight0 + 2.f * (g_fWeight1 + g_fWeight2 + g_fWeight3 + g_fWeight4));

	fWeight0 = g_fWeight0 / fNormalization;
	fWeight1 = g_fWeight1 / fNormalization;
	fWeight2 = g_fWeight2 / fNormalization;
	fWeight3 = g_fWeight3 / fNormalization;
	fWeight4 = g_fWeight4 / fNormalization;

	Out.vColor = 0.f;

	Out.vColor += tex2D(FilteredSampler, In.vTexUV + float2(fTexelWidthSize * -4.f, 0.f)) * fWeight4;
	Out.vColor += tex2D(FilteredSampler, In.vTexUV + float2(fTexelWidthSize * -3.f, 0.f)) * fWeight3;
	Out.vColor += tex2D(FilteredSampler, In.vTexUV + float2(fTexelWidthSize * -2.f, 0.f) * fWeight2;
	Out.vColor += tex2D(FilteredSampler, In.vTexUV + float2(fTexelWidthSize * -1.f, 0.f) * fWeight1;
	Out.vColor += tex2D(FilteredSampler, In.vTexUV + float2(fTexelWidthSize * 0.f, 0.f)) * fWeight0;
	Out.vColor += tex2D(FilteredSampler, In.vTexUV + float2(fTexelWidthSize * 1.f, 0.f)) * fWeight1;
	Out.vColor += tex2D(FilteredSampler, In.vTexUV + float2(fTexelWidthSize * 2.f, 0.f)) * fWeight2;
	Out.vColor += tex2D(FilteredSampler, In.vTexUV + float2(fTexelWidthSize * 3.f, 0.f)) * fWeight3;
	Out.vColor += tex2D(FilteredSampler, In.vTexUV + float2(fTexelWidthSize * 4.f, 0.f)) * fWeight4;

	Out.vColor.a = 1.f;

	return Out;
}


PS_OUT PS_VERTICAL_BLUR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float fTexelHeightSize = 1.f / g_fScreeHeight;

	float fWeight0, fWeight1, fWeight2, fWeight3, fWeight4;
	float fNormalization;

	fWeight0 = g_fWeight0;
	fWeight1 = g_fWeight1;
	fWeight2 = g_fWeight2;
	fWeight3 = g_fWeight3;
	fWeight4 = g_fWeight4;

	fNormalization = (fWeight0 + 2.f * (fWeight1 + fWeight2 + fWeight3 + fWeight4));

	fWeight0 = fWeight0 / fNormalization;
	fWeight1 = fWeight1 / fNormalization;
	fWeight2 = fWeight2 / fNormalization;
	fWeight3 = fWeight3 / fNormalization;
	fWeight4 = fWeight4 / fNormalization;

	Out.vColor = vector(0.f, 0.f, 0.f, 0.f);

	float2 f2TexUV1, f2TexUV2, f2TexUV3, f2TexUV4, f2TexUV5, f2TexUV6, f2TexUV7, f2TexUV8, f2TexUV9;

	f2TexUV1 = In.vTexUV + float2(0.f, fTexelHeightSize * -4.f);
	f2TexUV2 = In.vTexUV + float2(0.f, fTexelHeightSize * -3.f);
	f2TexUV3 = In.vTexUV + float2(0.f, fTexelHeightSize * -2.f);
	f2TexUV4 = In.vTexUV + float2(0.f, fTexelHeightSize * -1.f);
	f2TexUV5 = In.vTexUV + float2(0.f, fTexelHeightSize * 0.f);
	f2TexUV6 = In.vTexUV + float2(0.f, fTexelHeightSize * 1.f);
	f2TexUV7 = In.vTexUV + float2(0.f, fTexelHeightSize * 2.f);
	f2TexUV8 = In.vTexUV + float2(0.f, fTexelHeightSize * 3.f);
	f2TexUV9 = In.vTexUV + float2(0.f, fTexelHeightSize * 4.f);

	Out.vColor += tex2D(FilteredSampler, f2TexUV1) * fWeight4;
	Out.vColor += tex2D(FilteredSampler, f2TexUV2) * fWeight3;
	Out.vColor += tex2D(FilteredSampler, f2TexUV3) * fWeight2;
	Out.vColor += tex2D(FilteredSampler, f2TexUV4) * fWeight1;
	Out.vColor += tex2D(FilteredSampler, f2TexUV5) * fWeight0;
	Out.vColor += tex2D(FilteredSampler, f2TexUV6) * fWeight1;
	Out.vColor += tex2D(FilteredSampler, f2TexUV7) * fWeight2;
	Out.vColor += tex2D(FilteredSampler, f2TexUV8) * fWeight3;
	Out.vColor += tex2D(FilteredSampler, f2TexUV9) * fWeight4;

	Out.vColor.a = 1.f;

	return Out;
}

PS_OUT		PS_BLUR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;


	float fTexelWidthSize	= 1.f / g_fScreeWidth;
	float fTexelHeightSize	= 1.f / g_fScreeHeight;

	float fWeight0, fWeight1, fWeight2, fWeight3, fWeight4;
	float fNormalization;

	fWeight0 = g_fWeight0;
	fWeight1 = g_fWeight1;
	fWeight2 = g_fWeight2;
	fWeight3 = g_fWeight3;
	fWeight4 = g_fWeight4;

	fNormalization = (fWeight0 + 2.f * (fWeight1 + fWeight2 + fWeight3 + fWeight4));

	fWeight0 = fWeight0 / fNormalization;
	fWeight1 = fWeight1 / fNormalization;
	fWeight2 = fWeight2 / fNormalization;
	fWeight3 = fWeight3 / fNormalization;
	fWeight4 = fWeight4 / fNormalization;

	Out.vColor = vector(0.f, 0.f, 0.f, 0.f);

	float2 f2TexUV1, f2TexUV2, f2TexUV3, f2TexUV4, f2TexUV5, f2TexUV6, f2TexUV7, f2TexUV8, f2TexUV9;

	f2TexUV1 = In.vTexUV + float2(fTexelWidthSize * -4.f, fTexelHeightSize * -4.f);
	f2TexUV2 = In.vTexUV + float2(fTexelWidthSize * -3.f, fTexelHeightSize * -3.f);
	f2TexUV3 = In.vTexUV + float2(fTexelWidthSize * -2.f, fTexelHeightSize * -2.f);
	f2TexUV4 = In.vTexUV + float2(fTexelWidthSize * -1.f, fTexelHeightSize * -1.f);
	f2TexUV5 = In.vTexUV + float2(fTexelWidthSize * 0.f,  fTexelHeightSize * 0.f);
	f2TexUV6 = In.vTexUV + float2(fTexelWidthSize * 1.f,  fTexelHeightSize * 1.f);
	f2TexUV7 = In.vTexUV + float2(fTexelWidthSize * 2.f,  fTexelHeightSize * 2.f);
	f2TexUV8 = In.vTexUV + float2(fTexelWidthSize * 3.f,  fTexelHeightSize * 3.f);
	f2TexUV9 = In.vTexUV + float2(fTexelWidthSize * 4.f,  fTexelHeightSize * 4.f);

	Out.vColor += tex2D(FilteredSampler, f2TexUV1) * fWeight4;
	Out.vColor += tex2D(FilteredSampler, f2TexUV2) * fWeight3;
	Out.vColor += tex2D(FilteredSampler, f2TexUV3) * fWeight2;
	Out.vColor += tex2D(FilteredSampler, f2TexUV4) * fWeight1;
	Out.vColor += tex2D(FilteredSampler, f2TexUV5) * fWeight0;
	Out.vColor += tex2D(FilteredSampler, f2TexUV6) * fWeight1;
	Out.vColor += tex2D(FilteredSampler, f2TexUV7) * fWeight2;
	Out.vColor += tex2D(FilteredSampler, f2TexUV8) * fWeight3;
	Out.vColor += tex2D(FilteredSampler, f2TexUV9) * fWeight4;

	//vector vShadow = tex2D(ShadowSampler, In.vTexUV);
	//if (vShadow.b )

	Out.vColor.a = 1.f;

	return Out;
}


technique Default_Device
{
	pass Horizontal_Blur
	{
		ZWriteEnable = false;
		pixelshader = compile ps_3_0 PS_HORIZONTAL_BLUR();
	}

	pass Vertical_Blur
	{
		ZWriteEnable = false;
		pixelshader = compile ps_3_0 PS_VERTICAL_BLUR();
	}

	pass Blur
	{
		ZWriteEnable = false;
		pixelshader = compile ps_3_0 PS_BLUR();
	}

};