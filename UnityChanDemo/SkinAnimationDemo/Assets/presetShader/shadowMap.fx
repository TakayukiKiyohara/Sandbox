/*!
 * @brief	シャドウマップ。
 */

#include "Common.h" 


struct VS_INPUT
{
	float4	Position	: POSITION;
};
struct VS_OUTPUT
{
	float4 Position	: POSITION;
	float4 depth	: TEXCOORD;
};

float4x4	g_mWVP;		//ワールドビュープロジェクション行列。
float2		g_farNear;	//遠平面と近平面。xに遠平面、yに近平面。
VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	
	Out.Position = mul(In.Position, g_mWVP);
	Out.depth = Out.Position;
	return Out;
}
float4 PSMain( VS_OUTPUT In ) : COLOR
{
	//深度を書き込む。
//	float z = In.depth.z / In.depth.w;
	float z = ( In.depth.z - g_farNear.y ) / (g_farNear.x - g_farNear.y);
	return float4(z,z*z,0.0f,1.0f);
}

technique RenderShadowMap
{
	pass p0
	{
		VertexShader 	= compile vs_2_0 VSMain();
		PixelShader 	= compile ps_2_0 PSMain();
	}
}
