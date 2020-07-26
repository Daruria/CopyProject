texture			g_NormalTexture;

sampler NormalSampler = sampler_state		// ���͸��� ���� ���ƾ� �Ѵ�. �븻 ���͸� ���͸��ϰ� �Ǹ� ��� ���� ��߳� ���ɼ��� �ִ�.
{
	texture = g_NormalTexture;
};

texture			g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

texture			g_ShadowTexture;

sampler ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;
};

vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse = (vector)1.f;
vector		g_vMtrlAmbient = (vector)1.f;

vector		g_vCamPos;
float		g_fPower;

matrix		g_matViewInv;
matrix		g_matProjInv;

vector		g_vLightPos;
matrix		g_matLightView;
matrix		g_matLightProj;

float BRDF(float3 vNormal, float3 vView, float3 vLight, float fRoughness)
{
	float fAlpha = fRoughness * fRoughness;
	float fAlpha2 = fAlpha * fAlpha;

	float fNoV = dot(vNormal, vView);
	float fNoL = dot(vNormal, vLight);
	
	float fG_V = fNoV + sqrt((fNoV - fNoV * fAlpha2) * fNoV + fAlpha2);
	float fG_L = fNoL + sqrt((fNoL - fNoL * fAlpha2) * fNoL + fAlpha2);
	
	return rcp(fG_V * fG_L);

	//return G_ggx(vNormal, vLight, fRoughness) * G_ggx(vNormal, vView, fRoughness) / (4 * dot(vNormal, vLight) * dot(vNormal, vView));
}


struct PS_IN					// screen ������ xyzrhwf �����ǰ� �ؽ��� uv�� �����Ǿ� �־���. �ȼ����̴����� ��ġ ���� ����� �� ���� ������ �ؽ��� uv���� �������� �ִ�.
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vShade		: COLOR0;
	vector			vSpecular	: COLOR1;
	vector			vRimLight	: COLOR2;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	vector	vNormal = tex2D(NormalSampler, In.vTexUV);

	//vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	vector vDir = normalize(g_vLightDir * -1.f);

	Out.vShade = saturate(dot(vDir, normalize(vNormal)))	* (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	vector		vReflect = reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal);

	vector		vDepth = tex2D(DepthSampler, In.vTexUV);

	float		vViewZ = vDepth.y * 1000.f;	

	vector		vPosition;

	vPosition.x = (In.vTexUV * 2.f - 1.f) * vViewZ;
	vPosition.y = (In.vTexUV * -2.f + 1.f) * vViewZ;
	vPosition.z = vDepth.x * vViewZ;
	vPosition.w = vViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vector		vLook = normalize(g_vCamPos - vPosition);

	float3 vEye = -normalize(vPosition.xyz - g_vCamPos.xyz);

	Out.vRimLight = pow((1.f - saturate(dot(vEye, normalize(vNormal.xyz)))), 1.f)* vShadow.g;

	//vector		vHalf = normalize(vLook + vDir);

	float		fMetalic = vDepth.b;
	Out.vSpecular = pow(saturate(dot(vReflect, vLook)), g_fPower) * fMetalic;
	Out.vSpecular.a = 0.f;

	vector	vShadow = tex2D(ShadowSampler, In.vTexUV);

	//float	fRoughness = vShadow.g;
	//Out.vSpecular.rgb = BRDF(vNormal.xyz, vLook.xyz, vDir.xyz, fRoughness).xxx * fMetalic;

	Out.vShade.rgb *= vShadow.rrr;
	//Out.vShade.rgb *= vDepth.a;
	
	//Out.vShade.rgb *= (1.f - fMetalic);

	//Out.vRimLight = 1.f;
	return Out;
}

technique Default_Device
{
	pass	Directional
	{
		zwriteEnable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass Point
	{
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}

};