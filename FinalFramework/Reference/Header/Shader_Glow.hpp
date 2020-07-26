texture g_BlurredTexture;

sampler BlurredSampler = sampler_state
{
	texture = g_BlurredTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_FilteredTexture;

sampler FilteredSampler = sampler_state
{
	texture = g_FilteredTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct  PS_IN
{
	float2			vTexUV		: TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT		PS_GLOW(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = tex2D(BlurredSampler, In.vTexUV);

	vector vBlurFilter = tex2D(FilteredSampler, In.vTexUV);

	//Out.vColor.rgb = Out.vColor.rgb * Out.vColor.a + (1.f - Out.vColor.a) * vBlurFilter;
	Out.vColor.rgb = Out.vColor.rgb + vBlurFilter.rgb;

	if (Out.vColor.a < vBlurFilter.a)
		Out.vColor.a = vBlurFilter.a;

	if (Out.vColor.r == 0 && Out.vColor.g == 0 && Out.vColor.b == 0)
		Out.vColor.a = 0.f;

	return Out;
}


technique Default_Device
{
	pass Blur
	{
		pixelshader = compile ps_3_0 PS_GLOW();
	}

};