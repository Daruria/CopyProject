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
};

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
	vector			vPreCapture : COLOR1;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(DepthSampler, In.vTexUV);
	Out.vPreCapture = tex2D(CaptureSampler, In.vTexUV);



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