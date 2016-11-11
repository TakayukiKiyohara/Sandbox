/*!
 *@brief	カラーテクスチャプリミティブ。
 */

float4x4	g_mWVP;		//ワールドビュープロジェクション行列。

struct VS_INPUT{
	float4	pos		: POSITION;
	float2	uv		: TEXCOORD0;
};

struct VS_OUTPUT{
	float4 pos		: POSITION;
	float2	uv		: TEXCOORD0;
};
float g_brightness;
float g_alpha;			//!<アルファ
float3 g_mulColor;		//!<乗算カラー。
texture g_texture;		//ディフューズテクスチャ。
sampler g_textureSampler = 
sampler_state
{
	Texture = <g_texture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Wrap;
	AddressV = Wrap;
};

VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out;
	Out.pos = mul(In.pos, g_mWVP);
	Out.uv = In.uv;
	return Out;
}
/*!
 * @brief	半透明合成用のピクセルシェーダー。
 */
float4 PSMainTrans( VS_OUTPUT In ) : COLOR0
{
	float4 tex = tex2D(g_textureSampler, In.uv);
	return float4(tex.xyz * g_mulColor, tex.a * g_alpha );
}
/*!
 * @brief	加算合成用のピクセルシェーダー。
 */
float4 PSMainAdd( VS_OUTPUT In ) : COLOR0
{
	float4 tex = tex2D(g_textureSampler, In.uv);
	return float4(tex.xyz * g_alpha, 1.0f );
}
/*!
 * @brief	半透明合成用のテクニック。
 */
technique ColorTexPrimTrans
{
	pass P0
    {          
        VertexShader = compile vs_2_0 VSMain();
        PixelShader  = compile ps_2_0 PSMainTrans();
    }
}
/*!
 * @brief	加算用テクニック。
 */
technique ColorTexPrimAdd
{
    pass P0
    {          
        VertexShader = compile vs_2_0 VSMain();
        PixelShader  = compile ps_2_0 PSMainAdd();
    }
}