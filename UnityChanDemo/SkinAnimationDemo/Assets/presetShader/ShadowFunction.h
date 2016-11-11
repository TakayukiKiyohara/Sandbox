/*!
 * @brief	影関係の関数集。
 */



float4x4	g_mLVP;					//ライトビュープロジェクション行列。

//シャドウマップ
texture g_shadowMap_0 : register(t0);
sampler g_shadowMapSampler_0 = 
sampler_state
{
	Texture = <g_shadowMap_0>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = CLAMP;
	AddressV = CLAMP;
};

texture g_shadowMap_1 : register(t1);
sampler g_shadowMapSampler_1 = 
sampler_state
{
	Texture = <g_shadowMap_1>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = CLAMP;
	AddressV = CLAMP;
};

texture g_shadowMap_2 : register(t2);
sampler g_shadowMapSampler_2 = 
sampler_state
{
	Texture = <g_shadowMap_2>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = CLAMP;
	AddressV = CLAMP;
};

#define MAX_SHADOW_MAP 	3		//シャドウマップの最大数。
/*!
 * @brief	シャドウレシーバー用のパラメータ。
 */
struct ShadowReceiverParam{
	float4x4		mLVP[MAX_SHADOW_MAP];		//ライトビュープロジェクション。
	int4			vsmFlag_numShadowMap;		//xにvsmのフラグ。yにシャドウマップの枚数。
};

ShadowReceiverParam gShadowReceiverParam : register(c0);		//シャドウレシーバーのパラメータ。

/*!
 * @brief	影計算。
 */
float CalcShadow(
	float4 lightViewPos_0, 
	float4 lightViewPos_1, 
	float4 lightViewPos_2
)
{
	float4x4 mLightViewPos;
	mLightViewPos[0] = lightViewPos_0;
	mLightViewPos[1] = lightViewPos_1;
	mLightViewPos[2] = lightViewPos_2;
	
	sampler texSampler[MAX_SHADOW_MAP];
	texSampler[0] = g_shadowMapSampler_0;
	texSampler[1] = g_shadowMapSampler_1;
	texSampler[2] = g_shadowMapSampler_2;
	float result = 1.0f;
	for(int i = 0; i < gShadowReceiverParam.vsmFlag_numShadowMap.y; i++ ){
		float4 posInLVP = mLightViewPos[i];
		posInLVP.xyz /= posInLVP.w;
		//uv座標に変換。
		float2 shadowMapUV = float2(0.5f, -0.5f) * posInLVP.xy  + float2(0.5f, 0.5f);
		float2 shadow_val = 1.0f;
		
		if(shadowMapUV.x < 0.99f && shadowMapUV.y < 0.99f && shadowMapUV.x > 0.01f && shadowMapUV.y > 0.01f){
			shadow_val = tex2D( texSampler[i], shadowMapUV ).rg;
			float depth = min(posInLVP.z, 1.0f);
			if(gShadowReceiverParam.vsmFlag_numShadowMap.x == 1){
				if( depth > shadow_val.r ){
					 // σ^2
					float depth_sq = shadow_val.r * shadow_val.r;
			        float variance = max(shadow_val.g - depth_sq, 0.0006f);
					float md = depth - shadow_val.r;
			        float P = variance / ( variance + md * md );
					result =  pow( P, 5.0f );
				}
			}else{
				if( depth > shadow_val.r + 0.006f ){
					result =  0.0f;
				}
			}
			break;
		}
	}
	//影になっていない。
	return result;
}