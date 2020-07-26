texture		g_ShadowTexture;

sampler ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;
};

texture			g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

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

	float4 vShadow = tex2D(ShadowSampler, In.vTexUV);
	float4 vDepth = tex2D(DepthSampler, In.vTexUV);
	Out.vColor.r = vShadow.g;
	Out.vColor.g = vShadow.b;
	Out.vColor.b = vDepth.y;
	Out.vColor.a = 1.f;

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