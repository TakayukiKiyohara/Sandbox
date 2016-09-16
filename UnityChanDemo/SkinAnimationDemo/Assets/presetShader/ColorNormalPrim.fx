/*!
 *@brief	頂点カラー、法線ありのプリミティブ。
 */
#include "Common.h" 
#include "LightingFunction.h"

float4x4	g_mWVP;					//ワールドビュープロジェクション行列。
float4x4	g_worldRotationMatrix;	//ワールド行列の回転成分。
float4x4	g_mWorld;				//ワールド行列。
float4x4	g_mLVP;					//ライトビュープロジェクション行列。

struct VS_INPUT{
	float4	pos		: POSITION;
	float3  normal	: NORMAL;
	float4	color 	: COLOR0;
};

struct VS_OUTPUT{
	float4 pos		: POSITION;
	float4 color	: COLOR0;
	float3 normal	: TEXCOORD0;
};


struct VS_INPUTShadow{
	float4	pos			: POSITION;
	float3  normal		: NORMAL;
	float4	color 		: COLOR0;
};

struct VS_OUTPUTShadow{
	float4 pos			: POSITION;
	float4 color		: COLOR0;
	float3 normal		: TEXCOORD0;
	float4 worldPos		: TEXCOORD1;
};
texture g_shadowMap;		//シャドウマップ。
sampler g_shadowMapSampler = 
sampler_state
{
    Texture = <g_shadowMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Clamp;
	AddressV = Clamp;
};

float2		g_farNear;	//遠平面と近平面。xに遠平面、yに近平面。


/*!
 * @brief
 */
VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out;
	Out.pos = mul(In.pos, g_mWVP);
	Out.color = In.color;
	//法線を回転。
	Out.normal = mul(In.normal, g_worldRotationMatrix);
	return Out;
}
/*!
 * @brief
 */
float4 PSMain( VS_OUTPUT In, uniform bool isIuminance  ) : COLOR0
{
	float4 color = DiffuseLight(In.normal);
	color.xyz += g_light.ambient;
	color.xyz *= In.color.xyz;
	
	if(isIuminance){
		//αに輝度を埋め込む。
		color.a = CalcLuminance(color.xyz);
	}else{
		color.a = 0.0f;
	}
	return color;
}
/*!
 * @brief	
 */
VS_OUTPUTShadow VSMainShadow( VS_INPUTShadow In )
{
	VS_OUTPUTShadow Out;
	Out.pos = mul(In.pos, g_mWVP);
	Out.color = In.color;
	//法線を回転。
	Out.normal = mul(In.normal, g_worldRotationMatrix);
	Out.worldPos = mul(In.pos, g_mWorld );
	Out.worldPos = mul(Out.worldPos, g_mLVP );
	return Out;
}
/*!
 * @brief
 */
float4 PSMainShadow( VS_OUTPUTShadow In, uniform bool isIuminance ) : COLOR0
{
	float4 diffuse = DiffuseLight(In.normal);
	
	float4 posInLVP = In.worldPos;
	//uv座標に変換。
	float2 shadowMapUV = float2(0.5f, -0.5f) * posInLVP.xy/posInLVP.w   + float2(0.5f, 0.5f);
	float shadow_val = 1.0f;
	if(shadowMapUV.x <= 1.0f && shadowMapUV.y <= 1.0f && shadowMapUV.x >= 0.0f && shadowMapUV.y >= 0.0f){
		shadow_val = tex2D( g_shadowMapSampler, shadowMapUV ).r;
	}
	float depth = ( posInLVP.z - g_farNear.y ) / (g_farNear.x - g_farNear.y);	

	if( depth > shadow_val){
		//影になっている。
		diffuse = 0.0f;
	}
	float4 color = diffuse;
	color.xyz += g_light.ambient;
	color.xyz *= In.color.xyz;

	if(isIuminance){
		//αに輝度を埋め込む。
		color.a = CalcLuminance(color.xyz);
	}else{
		color.a = 0.0f;
	}
	return color;
}
technique ColorNormalPrim
{
    pass P0
    {          
        VertexShader = compile vs_2_0 VSMain();
        PixelShader  = compile ps_2_0 PSMain(false);
    }
}
/*!
 * @brief	COLOR+ライト+シャドウ
 */
technique ColorNormalShadowPrim
{
	pass P0
	{
		VertexShader = compile vs_2_0 VSMainShadow();
		PixelShader = compile ps_2_0 PSMainShadow(false);
	}
}
/*!
 * @brief	自己発光を行うオブジェクトで使ってね。
 */
technique ColorNormalPrimIuminance
{
	pass P0
    {          
        VertexShader = compile vs_2_0 VSMain();
        PixelShader  = compile ps_2_0 PSMain(true);
	}
}
/*!
 * @brief	COLOR+ライト+シャドウ+自己発光
 */
technique ColorNormalShadowPrimIuminance
{
	pass P0
	{
		VertexShader = compile vs_2_0 VSMainShadow();
		PixelShader = compile ps_2_0 PSMainShadow(true);
	}
}
