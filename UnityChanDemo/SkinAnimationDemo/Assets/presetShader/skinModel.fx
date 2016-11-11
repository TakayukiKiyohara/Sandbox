/*!
 * @brief	スキンモデルシェーダー。(4ボーンスキニング)
 */
#include "Common.h" 
#include "LightingFunction.h"
#include "ShadowFunction.h"


//スキン行列。
#define MAX_MATRICES  26
float4x3    g_mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;
float4x4    g_mViewProj : VIEWPROJECTION;
float		g_numBone;			//骨の数。
float4x4	g_worldMatrix;			//!<ワールド行列。
float4x4	g_rotationMatrix;		//!<回転行列。
float4x4	g_viewMatrixRotInv;		//!<カメラの回転行列の逆行列。
float4x4	g_mViewProjLastFrame;	//!<1フレーム前のビュープロジェクション行列。
float4		g_fogParam;				//フォグのパラメータ。xにフォグが掛かり始める深度。yにフォグが完全にかかる深度。zはフォグを計算するかどうかのフラグ。

float2		g_farNear;	//遠平面と近平面。xに遠平面、yに近平面。

int4 g_flags;				//xに法線マップ、yはシャドウレシーバー、zはリムライト、wはスペキュラマップ。
int4 g_flags2;				//xに速度マップへの書き込み

texture g_diffuseTexture;		//ディフューズテクスチャ。
sampler g_diffuseTextureSampler = 
sampler_state
{
	Texture = <g_diffuseTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Wrap;
	AddressV = Wrap;
};

//法線マップ
texture g_normalTexture;		//法線マップ。
sampler g_normalMapSampler = 
sampler_state
{
	Texture = <g_normalTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Wrap;
	AddressV = Wrap;
};




/*!
 * @brief	入力頂点
 */
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4  BlendWeights    : BLENDWEIGHT;
    float4  BlendIndices    : BLENDINDICES;
    float3  Normal          : NORMAL;
    float3	Tangent			: TANGENT;		//接ベクトル
    float3  Tex0            : TEXCOORD0;
};

/*!
 * @brief	インスタンシング描画用の入力頂点。
 */
struct VS_INPUT_INSTANCING
{
	VS_INPUT	base;
	float4 mWorld1	: TEXCOORD1;		//ワールド行列の1行目
	float4 mWorld2	: TEXCOORD2;		//ワールド行列の2行目
	float4 mWorld3	: TEXCOORD3;		//ワールド行列の3行目
	float4 mWorld4	: TEXCOORD4;		//ワールド行列の4行目
};

/*!
 * @brief	出力頂点。
 */
struct VS_OUTPUT
{
	float4  Pos     		: POSITION;
    float3  Normal			: NORMAL;
    float2  Tex0   			: TEXCOORD0;
    float3	Tangent			: TEXCOORD1;	//接ベクトル
    float4  worldPos_depth	: TEXCOORD2;	//xyzにワールド座標。wには射影空間でのdepthが格納される。
    float4  lightViewPos_0	: TEXCOORD3;
    float4  lightViewPos_1	: TEXCOORD4;
    float4  lightViewPos_2	: TEXCOORD5;
    float4  velocity		: TEXCOORD6;	//速度。
    float4  screenPos		: TEXCOORD7;
};

/*!
 * @brief	シャドウマップ描き込み時の出力頂点。
 */
struct VS_OUTPUT_RENDER_SHADOW_MAP
{
	float4  Pos     : POSITION;
	float4	depth	: TEXCOORD;
};

/*!
 * @brief	ピクセルシェーダーからの出力。
 */
struct PSOutput{
	float4	color		: COLOR0;		//レンダリングターゲット0に書き込み。
	float4	depth		: COLOR1;		//レンダリングターゲット1に書き込み。
	float4  velocity 	: COLOR2;		//レンダリングターゲット2に書き込み。
};

/*!
 *@brief	ワールド座標とワールド法線をスキン行列から計算する。
 *@param[in]	In		入力頂点。
 *@param[out]	Pos		ワールド座標の格納先。
 *@param[out]	Normal	ワールド法線の格納先。
 *@param[out]	Tangent	ワールド接ベクトルの格納先。
 */
void CalcWorldPosAndNormalFromSkinMatrix( VS_INPUT In, out float3 Pos, out float3 Normal, out float3 Tangent, uniform bool calcNormal )
{
	Pos = 0.0f;
	Normal = 0.0f;
	Tangent = 0.0f;
	//ブレンドするボーンのインデックス。
	int4 IndexVector = D3DCOLORtoUBYTE4(In.BlendIndices);
	
	//ブレンドレート。
	float BlendWeightsArray[4] = (float[4])In.BlendWeights;
    int   IndexArray[4]        = (int[4])IndexVector;
    float LastWeight = 0.0f;
    for (int iBone = 0; iBone < g_numBone-1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        Pos += mul(In.Pos, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        if(calcNormal){
	        Normal += mul(In.Normal, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    	    Tangent += mul(In.Tangent, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    	}
    }
    LastWeight = 1.0f - LastWeight; 
    
	Pos += (mul(In.Pos, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
	if(calcNormal){
	    Normal += (mul(In.Normal, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
    	Tangent += (mul(In.Tangent, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
    }
}
/*!
 *@brief	ワールド座標とワールド法線を計算。
 *@param[in]	In		入力頂点。
 *@param[out]	Pos		ワールド座標の格納先。
 *@param[out]	Normal	ワールド法線の格納先。
 *@param[out]	Tangent	ワールド接ベクトルの格納先。
 */
void CalcWorldPosAndNormal( VS_INPUT In, out float3 Pos, out float3 Normal, out float3 Tangent, uniform bool calcNormal )
{
	Normal = 0.0f;
	Tangent = 0.0f;
	Pos = mul(In.Pos, g_worldMatrix );
	if(calcNormal){
		Normal = mul(In.Normal, g_worldMatrix );
		Tangent = mul(In.Tangent, g_worldMatrix );
	}
}
/*!
 *@brief	頂点シェーダー。
 *@param[in]	In			入力頂点。
 *@param[in]	hasSkin		スキンあり？
 */
VS_OUTPUT VSMain( VS_INPUT In, uniform bool hasSkin )
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//スキンあり。
	    CalcWorldPosAndNormalFromSkinMatrix( In, Pos, Normal, Tangent, true );
	}else{
		//スキンなし。
		CalcWorldPosAndNormal( In, Pos, Normal, Tangent, true );
	}
	o.worldPos_depth.xyz = Pos.xyz;


    o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
    o.worldPos_depth.w = o.Pos.w;
    o.Normal = normalize(Normal);
    o.Tangent = normalize(Tangent);
    o.Tex0 = In.Tex0;
    o.velocity = mul(float4(Pos.xyz, 1.0f), g_mViewProjLastFrame);
    o.screenPos = o.Pos;
    if(g_flags.y){
		//シャドウレシーバー。
		o.lightViewPos_0 = mul(float4(Pos.xyz, 1.0f), gShadowReceiverParam.mLVP[0] );
		o.lightViewPos_1 = mul(float4(Pos.xyz, 1.0f), gShadowReceiverParam.mLVP[1] );
		o.lightViewPos_2 = mul(float4(Pos.xyz, 1.0f), gShadowReceiverParam.mLVP[2] );
	}
	return o;
}
/*!
 *@brief	頂点シェーダー。
 *@param[in]	In			入力頂点。
 *@param[in]	hasSkin		スキンあり？
 */
VS_OUTPUT VSMainInstancing( VS_INPUT_INSTANCING In, uniform bool hasSkin )
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//スキンあり。
	    CalcWorldPosAndNormalFromSkinMatrix( In.base, Pos, Normal, Tangent, true );
	}else{
		//スキンなし。
		CalcWorldPosAndNormal( In.base, Pos, Normal, Tangent, true );
	}
	float4x4 worldMat;
	worldMat[0] = In.mWorld1;
	worldMat[1] = In.mWorld2;
	worldMat[2] = In.mWorld3;
	worldMat[3] = In.mWorld4;
	Pos = mul(float4(Pos.xyz, 1.0f), worldMat );	//ワールド行列をかける。

	o.worldPos_depth.xyz = Pos.xyz;
    o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
    o.worldPos_depth.w = o.Pos.w;
    o.Normal = mul(normalize(Normal), worldMat);
    o.Tex0 = In.base.Tex0;
   	o.velocity = mul(float4(Pos.xyz, 1.0f), g_mViewProjLastFrame);
   	o.screenPos = o.Pos;
    if(g_flags.y){
		//シャドウレシーバー。
		o.lightViewPos_0 = mul(float4(Pos.xyz, 1.0f), gShadowReceiverParam.mLVP[0] );
		o.lightViewPos_1 = mul(float4(Pos.xyz, 1.0f), gShadowReceiverParam.mLVP[1] );
		o.lightViewPos_2 = mul(float4(Pos.xyz, 1.0f), gShadowReceiverParam.mLVP[2] );
	}
	return o;
}

/*!
 * @brief	ピクセルシェーダー。
 */
PSOutput PSMain( VS_OUTPUT In )
{
	float4 color = tex2D(g_diffuseTextureSampler, In.Tex0);
	float3 normal = normalize(In.Normal);
	if(g_flags.x){
		//法線マップあり。
		float3 tangent = normalize(In.Tangent);
		float3 binSpaceNormal = tex2D( g_normalMapSampler, In.Tex0);
		float4x4 tangentSpaceMatrix;
		float3 biNormal = normalize( cross( tangent, normal) );
		tangentSpaceMatrix[0] = float4( tangent, 0.0f);
		tangentSpaceMatrix[1] = float4( biNormal, 0.0f);
		tangentSpaceMatrix[2] = float4( normal, 0.0f);
		tangentSpaceMatrix[3] = float4( 0.0f, 0.0f, 0.0f, 1.0f );
		//-1.0～1.0の範囲にマッピングする。
		binSpaceNormal = (binSpaceNormal * 2.0f)- 1.0f;
		normal = tangentSpaceMatrix[0] * binSpaceNormal.x + tangentSpaceMatrix[1] * binSpaceNormal.y + tangentSpaceMatrix[2] * binSpaceNormal.z; 
		
	}
	
	
	float4 lig = DiffuseLight(normal);
	if(g_flags.z){
		//リムライト。
		lig.xyz += CalcLimLight(normal, g_light.limLightDir, g_light.limLightColor.xyz);
	}
	if(g_flags.w){
		//スペキュラライト。
		lig.xyz += SpecLight(normal, In.worldPos_depth.xyz, In.Tex0);
	}
	
	if(g_flags.y){
		//影
		lig *= CalcShadow(In.lightViewPos_0, In.lightViewPos_1, In.lightViewPos_2);
	
	}
	//ポイントライト。
	lig.xyz += PointLight(normal, In.worldPos_depth.xyz, g_flags.z);
	
	//アンビエントライトを加算。
	lig.xyz += g_light.ambient.xyz;
	color.xyz *= lig;
	
	
	if(g_fogParam.z > 1.9f){
		//高さフォグ
		float h = max(In.worldPos_depth.y - g_fogParam.y, 0.0f);
		float t = min(h / g_fogParam.x, 1.0f);
		color.xyz = lerp(float3(0.75f, 0.75f, 0.95f), color.xyz, t);
	}else if(g_fogParam.z > 0.0f){
		//距離フォグ
		float z = length(In.worldPos_depth.xyz - g_cameraPos);
		z = max(z - g_fogParam.x, 0.0f);
		float t = min( z / g_fogParam.y, 1.0f);
		color.xyz = lerp(color.xyz, float3(0.75f, 0.75f, 0.95f), t);
	}
	PSOutput psOut = (PSOutput)0;
	psOut.color = color;
	psOut.depth = In.worldPos_depth.w;
	if(g_flags2.x){
		psOut.velocity.xy = In.velocity.xy / In.velocity.w-In.screenPos.xy / In.screenPos.w;
		psOut.velocity.xy *= 0.5f;
		psOut.velocity.xy += 0.5f;
		psOut.velocity.zw = 0.0f;
	}else{
		//速度なし。
		psOut.velocity = 0.5f;
	}
	return psOut;
}


/*!
 * @brief	シャドウマップ描き込み時の頂点シェーダー。
 */
VS_OUTPUT_RENDER_SHADOW_MAP VSMainRenderShadowMap( VS_INPUT In, uniform bool hasSkin )
{
	VS_OUTPUT_RENDER_SHADOW_MAP o = (VS_OUTPUT_RENDER_SHADOW_MAP)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//スキンあり。
	    CalcWorldPosAndNormalFromSkinMatrix( In, Pos, Normal, Tangent, false );
	}else{
		//スキンなし。
		CalcWorldPosAndNormal( In, Pos, Normal, Tangent, false );
	}
	o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
	o.depth = o.Pos;
	return o;
}
/*!
 * @brief	シャドウマップ書き込み時の頂点シェーダー(インスタンシング版)
 */
VS_OUTPUT_RENDER_SHADOW_MAP VSMainInstancingRenderShadowMap(VS_INPUT_INSTANCING In, uniform bool hasSkin)
{
	VS_OUTPUT_RENDER_SHADOW_MAP o = (VS_OUTPUT_RENDER_SHADOW_MAP)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//スキンあり。
	    CalcWorldPosAndNormalFromSkinMatrix( In.base, Pos, Normal, Tangent, false );
	}else{
		//スキンなし。
		CalcWorldPosAndNormal( In.base, Pos, Normal, Tangent, false );
	}
	float4x4 worldMat;
	worldMat[0] = In.mWorld1;
	worldMat[1] = In.mWorld2;
	worldMat[2] = In.mWorld3;
	worldMat[3] = In.mWorld4;
	Pos = mul(float4(Pos.xyz, 1.0f), worldMat );	//ワールド行列をかける。
	o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
	o.depth = o.Pos;
	return o;
}
/*!
 * @brief	シャドウマップ描き込み時のピクセルシェーダー。
 */
float4 PSMainRenderShadowMap( VS_OUTPUT_RENDER_SHADOW_MAP In ) : COLOR
{

	float z = In.depth.z/In.depth.w;
	float dx = ddx(z);
	float dy = ddy(z);
	return float4(z, z*z+0.25f*(dx*dx+dy*dy), 0.0f, 1.0f);
}

/*!
 *@brief	スキンありモデル用のテクニック。
 */
technique SkinModel
{
    pass p0
    {
        VertexShader = compile vs_3_0 VSMain(true);
        PixelShader = compile ps_3_0 PSMain();
    }
}
/*!
 *@brief	スキンなしモデル用のテクニック。
 */
technique NoSkinModel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain(false);
		PixelShader = compile ps_3_0 PSMain();
	}
}
/*!
 *@brief	スキンありモデルのインスタンシング描画用のテクニック。
 */
technique SkinModelInstancing
{
    pass p0
    {
        VertexShader = compile vs_3_0 VSMainInstancing(true);
        PixelShader = compile ps_3_0 PSMain();
    }
}
/*!
 *@brief	スキンなしモデルのインスタンシング描画用のテクニック。
 */
technique NoSkinModelInstancing
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancing(false);
		PixelShader = compile ps_3_0 PSMain();
	}
}

/*!
 * @brief	スキンありモデル用のシャドウマップ描き込みテクニック。
 */
technique SkinModelRenderShadowMap
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainRenderShadowMap(true);
		PixelShader = compile ps_3_0 PSMainRenderShadowMap();
	}
}

/*!
 * @brief	スキンなしモデル用のシャドウマップ描き込みテクニック。
 */
technique NoSkinModelRenderShadowMap
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainRenderShadowMap(false);
		PixelShader = compile ps_3_0 PSMainRenderShadowMap();
	}
}
technique SkinModelInstancingRenderToShadowMap
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancingRenderShadowMap(true);
        PixelShader = compile ps_3_0 PSMainRenderShadowMap();
	}
}
technique NoSkinModelInstancingRenderToShadowMap
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancingRenderShadowMap(false);
        PixelShader = compile ps_3_0 PSMainRenderShadowMap();
	}
}
