/*!
 * @brief	ライティング関係の関数集
 */

#define NUM_DIFFUSE_LIGHT	4					//ディフューズライトの数。

//スペキュラマップ
texture g_speculerMap;
sampler g_speculerMapSampler = 
sampler_state
{
	Texture = <g_speculerMap>;
    MipFilter = NONE;
    MinFilter = NONE;
    MagFilter = NONE;
    AddressU = Wrap;
	AddressV = Wrap;
};

struct SLight{
	float3	diffuseLightDir[NUM_DIFFUSE_LIGHT];		//ディフューズライトの向き。
	float4  diffuseLightColor[NUM_DIFFUSE_LIGHT];	//ディフューズライトのカラー。
	float3	limLightDir;							//リムライトの方向。
	float4	limLightColor;							//リムライトの色。
	float3  ambient;								//アンビエントライト。
};
SLight	g_light;		//!<ライト
float4	g_cameraPos;	//!<カメラの座標。
float3	g_cameraDir;	//!<カメラ方向。
/*!
 *@brief	ディフューズライトを計算。
 */	
float4 DiffuseLight( float3 normal )
{
	float4 color = 0.0f;
	color += max( 0, -dot(normal, g_light.diffuseLightDir[0])) * g_light.diffuseLightColor[0];
	color += max( 0, -dot(normal, g_light.diffuseLightDir[1])) * g_light.diffuseLightColor[1];
	color += max( 0, -dot(normal, g_light.diffuseLightDir[2])) * g_light.diffuseLightColor[2];
	color += max( 0, -dot(normal, g_light.diffuseLightDir[3])) * g_light.diffuseLightColor[3];
	
	color.a = 1.0f;
	return color;
}
/*!
 *@brief	リムライトを計算。
 */
float3 CalcLimLight( float3 normal)
{
	float lim = 0.0f;
	float baselim = 1.0f - abs( dot(normal, g_cameraDir ) );
	lim += baselim * max( 0.0f, -dot(g_cameraDir, g_light.limLightDir));
	lim = pow(lim, 1.5f);
	return g_light.limLightColor.xyz * lim;
}
/*!
 *@brief	スペキュラライトを計算。
 */
float3 SpecLight(float3 normal, float3 worldPos, float2 uv)
{
	float specPow = tex2D(g_speculerMapSampler, uv);
	float3 spec = 0.0f;
	float3 toEyeDir = normalize( g_cameraPos - worldPos );
	float3 R = -toEyeDir + 2.0f * dot(normal, toEyeDir) * normal;
	for( int i = 0; i < NUM_DIFFUSE_LIGHT; i++ ){
		//スペキュラ成分を計算する。
		//反射ベクトルを計算。
		float3 L = -g_light.diffuseLightDir[i].xyz;
		spec += g_light.diffuseLightColor[i] * pow(max(0.0f, dot(L,R)), 2 ) * g_light.diffuseLightColor[i].w;	//スペキュラ強度。
	}
	return spec * specPow;
}
/*!
 * @brief	アルファに埋め込む輝度を計算。
 */
float CalcLuminance( float3 color )
{
	float luminance = dot( color.xyz, float3(0.2125f, 0.7154f, 0.0721f) );
	if(luminance > 1.0f ){
		luminance = 1.0f / luminance;
	}else{
		luminance = 0.0f;
	}
	return luminance;
}