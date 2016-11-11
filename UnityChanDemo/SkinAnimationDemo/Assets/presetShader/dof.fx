/*!
 * @brief	被写界深度。
 */


texture g_scene;		//シーンテクスチャ。

sampler g_SceneSampler = 
sampler_state
{
    Texture = <g_scene>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

texture g_depthTexture;	//深度テクスチャ。
float2 g_sceneTexSize;	//シーンテクスチャのサイズ。
float2 g_nearFar;
sampler g_depthSampler = 
sampler_state
{
	Texture = <g_depthTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

float3 g_dofParam;		//
texture g_blurBack;	//奥ボケ
sampler g_blurBackSampler = 
sampler_state
{
	Texture = <g_blurBack>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

texture g_blurForward;	//手前ボケ
sampler g_blurForwardSampler = 
sampler_state
{
	Texture = <g_blurForward>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


struct VS_INPUT{
	float4	pos : POSITION;
};

struct VS_OUTPUT{
	float4 pos 	: POSITION;
	float2 tex0 : TEXCOORD0;
};
/*!
 * @brief	頂点シェーダ。
 */
VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	Out.pos = In.pos;
	float2 tex = (In.pos * 0.5f) + 0.5f;
	
	tex.y = 1.0f - tex.y;
	Out.tex0 = tex + float2( 0.5/g_sceneTexSize.x, 0.5/g_sceneTexSize.y);
	return Out;
}

/*!
 * @brief	ピクセルシェーダー。
 */
float4 PSMain(VS_OUTPUT In ) : COLOR
{
	float4 sceneColor = tex2D(g_SceneSampler, In.tex0);
	float4 depth = tex2D(g_depthSampler, In.tex0);
	//手前ボケ
	float t = depth - g_dofParam.z;
	float forwardRate = max( 0.0f, -g_dofParam.x - t );
	float backRate = max(0.0f, t - g_dofParam.y);
	t = max(forwardRate, backRate);
	t = t / ((g_dofParam.z- g_dofParam.x));
	float4 color = 0.0f;
	if(forwardRate < backRate){
		//奥ボケ
		t *= g_dofParam.x / (g_dofParam.y);
		t = min(1.0f, t);
		float4 blur = tex2D(g_blurBackSampler, In.tex0);
		color = lerp(sceneColor, blur, t);

	}else{
		//手前ボケ
		t = min(1.0f, t * 2.0f);
		float4 blur = tex2D(g_blurForwardSampler, In.tex0);
		color = lerp(sceneColor, blur, t);
	}
	
	return color;
}
technique Dof
{
	pass p0{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
};