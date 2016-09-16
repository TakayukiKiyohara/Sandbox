/*!
 * @brief	IDMap
 */

#include "Common.h" 

struct VS_INPUT
{
	float4	Position	: POSITION;
};
struct VS_OUTPUT
{
	float4 Position	: POSITION;
};

float4x4 g_mWVP;    // World * View * Projection matrix


/*!
 * @brief	IDマップ描画の頂点シェーダー
 */
VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	
	Out.Position = mul(In.Position, g_mWVP);
	
	return Out;
}

texture g_idMap;		//!<IDMap。
float2 g_idMapSize;		//!<IDMapのサイズ。

sampler TextureSampler = 
sampler_state
{
    Texture = <g_idMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

struct VS_OUTPUT_RENDER_EDGE
{
	float4 Position	: POSITION;
	float2 texCoord : TEXCOORD;
};

/*!
 * @brief	エッジ描画用の頂点シェーダー
 */
VS_OUTPUT_RENDER_EDGE RenderEdgeVS( VS_INPUT In )
{
	VS_OUTPUT_RENDER_EDGE Out;
	Out.Position = In.Position;	//トランスフォーム済み頂点なのでそのまま。
	Out.texCoord = (In.Position * 0.5f) + 0.5f;
	Out.texCoord.y = 1.0f - Out.texCoord.y;
	return Out;
}
/*!
 * @brief	IDマップ描画のピクセルシェーダー
 */
float4 PSMain( VS_OUTPUT In ) : COLOR
{
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

/*!
 * @brief	エッジ描画用のピクセルシェーダー
 */
float4 RenderEdgePS( VS_OUTPUT_RENDER_EDGE In ) : COLOR
{
	//自身のピクセルのID
	float2 uv = In.texCoord.xy;
	float id = tex2D( TextureSampler, uv).a;
	//上下４ピクセルを調べる。
	float2 offset = 1.0f / g_idMapSize;

	float id0 = tex2D( TextureSampler, float2(uv.x + offset.x, uv.y)).a;	//右
	float id1 = tex2D( TextureSampler, float2(uv.x - offset.x, uv.y)).a;	//左
	float id2 = tex2D( TextureSampler, float2(uv.x, uv.y + offset.y)).a;	//上
	float id3 = tex2D( TextureSampler, float2(uv.x, uv.y - offset.y)).a;	//下
	//IDの差の絶対値を合算する。
	float t = abs(( id - id0 ))
			+ abs(( id - id1 ))
			+ abs(( id - id2 ))
			+ abs(( id - id3 ));
	//絶対値の差が0.1以下ならピクセルキル。
	clip(t - 0.1f);
	return float4(0.0, 0.0f, 0.0f, 1.0f);
}



technique RenderIDMap
{
	pass p0
	{
		VertexShader 	= compile vs_2_0 VSMain();
		PixelShader		= compile ps_2_0 PSMain();
	}
}
technique RenderEdge
{
	pass p0
	{
		VertexShader 	= compile vs_2_0 RenderEdgeVS();
		PixelShader 	= compile ps_2_0 RenderEdgePS();
	}
}