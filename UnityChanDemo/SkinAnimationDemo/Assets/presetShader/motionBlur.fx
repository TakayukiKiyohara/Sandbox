/*!
 * @brief	モーションブラー。
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

texture g_velocityTexture;	//速度テクスチャ
float2 g_sceneTexSize;	//シーンテクスチャのサイズ。
sampler g_velocitySampler = 
sampler_state
{
	Texture = <g_velocityTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
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
	//速度を取得。
	float4 velocity = tex2D(g_velocitySampler, In.tex0 );
	velocity.xy *= 2.0f;
	velocity.xy -= 1.0f;
	velocity.y *= -1.0f;
	float4 color = 0.0f;
	//4ピクセルを取得する。
	int numSample = 16;
	float t = 0.03f;
	for(int i = 0; i < numSample; i++){
		color += tex2D( g_SceneSampler, In.tex0 + velocity * t*i );
	}
	color.xyz /= numSample;
	return color;
}
technique MotionBlur
{
	pass p0{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
};