texture g_AlbedoTexture;

sampler AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;

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

texture g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

texture g_GlowEffectTexture;

sampler GlowEffectSampler = sampler_state
{
	texture = g_GlowEffectTexture;

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

PS_OUT		PS_BLUR_FILTER(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor		= tex2D(AlbedoSampler, In.vTexUV);

	//vector vShadow	= tex2D(ShadowSampler, In.vTexUV);
	//Out.vColor.rgb	*= vShadow.b;

	vector vDepth = tex2D(DepthSampler, In.vTexUV);
	Out.vColor.rgb *= vDepth.b;

	Out.vColor += tex2D(GlowEffectSampler, In.vTexUV);

	return Out;
}


technique Default_Device
{
	pass Blur
	{
		pixelshader = compile ps_3_0 PS_BLUR_FILTER();
	}

};