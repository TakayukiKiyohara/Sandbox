/*!
 *@brief	���_�J���[�A�@������̃v���~�e�B�u�B
 */
#include "Common.h" 
#include "LightingFunction.h"

float4x4	g_mWVP;					//���[���h�r���[�v���W�F�N�V�����s��B
float4x4	g_worldRotationMatrix;	//���[���h�s��̉�]�����B
float4x4	g_mWorld;				//���[���h�s��B
float4x4	g_mLVP;					//���C�g�r���[�v���W�F�N�V�����s��B

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
texture g_shadowMap;		//�V���h�E�}�b�v�B
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

float2		g_farNear;	//�����ʂƋߕ��ʁBx�ɉ����ʁAy�ɋߕ��ʁB


/*!
 * @brief
 */
VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out;
	Out.pos = mul(In.pos, g_mWVP);
	Out.color = In.color;
	//�@������]�B
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
		//���ɋP�x�𖄂ߍ��ށB
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
	//�@������]�B
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
	//uv���W�ɕϊ��B
	float2 shadowMapUV = float2(0.5f, -0.5f) * posInLVP.xy/posInLVP.w   + float2(0.5f, 0.5f);
	float shadow_val = 1.0f;
	if(shadowMapUV.x <= 1.0f && shadowMapUV.y <= 1.0f && shadowMapUV.x >= 0.0f && shadowMapUV.y >= 0.0f){
		shadow_val = tex2D( g_shadowMapSampler, shadowMapUV ).r;
	}
	float depth = ( posInLVP.z - g_farNear.y ) / (g_farNear.x - g_farNear.y);	

	if( depth > shadow_val){
		//�e�ɂȂ��Ă���B
		diffuse = 0.0f;
	}
	float4 color = diffuse;
	color.xyz += g_light.ambient;
	color.xyz *= In.color.xyz;

	if(isIuminance){
		//���ɋP�x�𖄂ߍ��ށB
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
 * @brief	COLOR+���C�g+�V���h�E
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
 * @brief	���Ȕ������s���I�u�W�F�N�g�Ŏg���ĂˁB
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
 * @brief	COLOR+���C�g+�V���h�E+���Ȕ���
 */
technique ColorNormalShadowPrimIuminance
{
	pass P0
	{
		VertexShader = compile vs_2_0 VSMainShadow();
		PixelShader = compile ps_2_0 PSMainShadow(true);
	}
}
