/*!
 *@brief	スプライト用シェーダー。
 */
#include "Common.h" 

struct VS_INPUT{
	float4		pos		: POSITION;
	float2		uv		: TEXCOORD0;
};

struct VS_OUTPUT{
	float4		pos		: POSITION;
	float2		uv		: TEXCOORD0;
};

float4x4 mWorld;		//ワールド行列。

texture g_tex;
sampler TextureSampler = 
sampler_state
{
    Texture = <g_tex>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out;
	Out.pos = mul(In.pos, mWorld);
	Out.uv 	= In.uv;
	return Out;
}
float4 PSMain( VS_OUTPUT In ) : COLOR0
{
	return tex2D( TextureSampler, In.uv );
}

technique SpriteTexture
{
	pass p0
	{
		VertexShader 	= compile vs_3_0 VSMain();
		PixelShader		= compile ps_3_0 PSMain();
	}
}