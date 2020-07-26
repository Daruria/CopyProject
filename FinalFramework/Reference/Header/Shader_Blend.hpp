texture				g_AlbedoTexture;

sampler AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_LightTexture;

sampler LightSampler = sampler_state
{
	texture = g_LightTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_SpecularTexture;

sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_PriorityTexture;

sampler PrioritySampler = sampler_state
{
	texture = g_PriorityTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_GlowTexture;

sampler GlowSampler = sampler_state
{
	texture = g_GlowTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_RimTexture;

sampler RimSampler = sampler_state
{
	texture = g_RimTexture;

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

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
	//vector			vCapture : COLOR1;
};


PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vAlbedo		= tex2D(AlbedoSampler, In.vTexUV);
	vector		vLight		= tex2D(LightSampler, In.vTexUV);		//	Shade 작업한 텍스쳐.
	vector		vSpecular	= tex2D(SpecularSampler, In.vTexUV);
	vector		vPriority	= tex2D(PrioritySampler, In.vTexUV);	//	SkySphere
	vector		vGlow		= tex2D(GlowSampler, In.vTexUV);
	vector		vRim		= tex2D(RimSampler, In.vTexUV);
	//vector		vShadow		= tex2D(ShadowSampler, In.vTexUV);

	//vector vColor = pow(abs(vAlbedo), 2.2 ) * vLight;
	vector vColor = (vAlbedo) * vLight;

	Out.vColor = vector(vColor.rgb * vAlbedo.a + (1.f - vAlbedo.a) * vPriority.rgb + vSpecular.rgb + vGlow.rgb * 0.4f + vRim.rgb, 1.f);

	//Out.vColor = pow(abs(Out.vColor), 1 / 2.2);
	//Out.vColor = Out.vColor / (Out.vColor + 1);


	//Out.vCapture = vColor * vShadow.b;

	//Out.vColor = vColor * (1.f - vDepth.b) + vSpecular * vDepth.b;

	//if (vAlbedo.a == 0.f)
	//	Out.vColor = vPriority;

	//Out.vColor = vAlbedo * vLight + vSpecular;

	return Out;
}

technique Default_Device
{
	pass	Blend
	{
		zwriteEnable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}

};