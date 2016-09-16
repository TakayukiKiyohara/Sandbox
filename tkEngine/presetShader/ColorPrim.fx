/*!
 *@brief	カラープリミティブ。
 */
#include "Common.h" 
float4x4	g_mWVP;		//ワールドビュープロジェクション行列。

struct VS_INPUT{
	float4	pos		: POSITION;
	float4	color 	: COLOR0;
};

struct VS_OUTPUT{
	float4 pos		: POSITION;
	float4 color	: COLOR0;
};

VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out;
	Out.pos = mul(In.pos, g_mWVP);
	Out.color = In.color;
	return Out;
}
float4 PSMain( VS_OUTPUT In ) : COLOR0
{
	return In.color;
}

technique ColorPrim
{
    pass P0
    {          
        VertexShader = compile vs_2_0 VSMain();
        PixelShader  = compile ps_2_0 PSMain();
    }
}