/*!
 *@brief	FXAA
 *@details
 * FXAAを使用したアンチエイリアス。
 */

struct VS_INPUT{
	float4		pos		: POSITION;
	float2		uv		: TEXCOORD0;
};

struct VS_OUTPUT{
	float4		pos		: POSITION;
	float2		uv		: TEXCOORD0;
};

float2 g_sceneTexSize;	//シーンテクスチャのサイズ。
texture g_scene;
sampler SceneSampler = sampler_state
{
	Texture = <g_scene>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


/*!
 *@brief	頂点シェーダー。
 */
VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out;
	Out.pos = In.pos;
	Out.uv = In.uv + float2( 0.5/g_sceneTexSize.x, 0.5/g_sceneTexSize.y);
	return Out;
}
float4 FxaaPixelShader( 
	float2 pos, 
    float4 fxaaConsoleRcpFrameOpt,
    float4 fxaaConsoleRcpFrameOpt2,
    float fxaaQualityEdgeThreshold,
    float fxaaQualityEdgeThresholdMin,
    float fxaaConsoleEdgeSharpness,
    float fxaaConsoleEdgeThreshold,
    float fxaaConsoleEdgeThresholdMin
)
{
	//近傍4テクセルをフェッチ。
	float4 nTex = float4( 
		pos.x + 0.5f/g_sceneTexSize.x, 
		pos.y + 0.5f/g_sceneTexSize.y,
		pos.x - 0.5f/g_sceneTexSize.x, 
		pos.y - 0.5f/g_sceneTexSize.y
	);
	float lumaNw = tex2D(SceneSampler, nTex.xy).y;
	float lumaSw = tex2D(SceneSampler, nTex.xw).y;
	float lumaNe = tex2D(SceneSampler, nTex.zy).y;
	float lumaSe = tex2D(SceneSampler, nTex.zw).y;
	
	float4 rgbyM = tex2D(SceneSampler, pos.xy);
	float lumaM = rgbyM.y;
	
	
	//近傍テクセルの輝度の差を調べる。
	float lumaMaxNwSw = max(lumaNw, lumaSw);
    lumaNe += 1.0/384.0;
    float lumaMinNwSw = min(lumaNw, lumaSw);
/*--------------------------------------------------------------------------*/
    float lumaMaxNeSe = max(lumaNe, lumaSe);
    float lumaMinNeSe = min(lumaNe, lumaSe);
/*--------------------------------------------------------------------------*/
    float lumaMax = max(lumaMaxNeSe, lumaMaxNwSw);
    float lumaMin = min(lumaMinNeSe, lumaMinNwSw);
/*--------------------------------------------------------------------------*/
    float lumaMaxScaled = lumaMax * fxaaConsoleEdgeThreshold;
/*--------------------------------------------------------------------------*/
    float lumaMinM = min(lumaMin, lumaM);
    float lumaMaxScaledClamped = max(fxaaConsoleEdgeThresholdMin, lumaMaxScaled);
    float lumaMaxM = max(lumaMax, lumaM);
    float dirSwMinusNe = lumaSw - lumaNe;
    float lumaMaxSubMinM = lumaMaxM - lumaMinM;
    float dirSeMinusNw = lumaSe - lumaNw;
    if(lumaMaxSubMinM < lumaMaxScaledClamped){
		//輝度の差がしきい値以下だったので、このピクセルはアンチをかけない。
		 return rgbyM;
	}
    
/*--------------------------------------------------------------------------*/
	//輝度の差を利用して、ギザギザが発生している可能性の高いテクセルをフェッチする。
    float2 dir;
    dir.x = dirSwMinusNe + dirSeMinusNw;
    dir.y = dirSwMinusNe - dirSeMinusNw;
   
/*--------------------------------------------------------------------------*/
    float2 dir1 = normalize(dir.xy);
   
    float4 rgbyN1 = tex2D(SceneSampler, pos.xy - dir1 * fxaaConsoleRcpFrameOpt.zw);
    float4 rgbyP1 = tex2D(SceneSampler, pos.xy + dir1 * fxaaConsoleRcpFrameOpt.zw);
/*--------------------------------------------------------------------------*/
    float dirAbsMinTimesC = min(abs(dir1.x), abs(dir1.y)) * fxaaConsoleEdgeSharpness;
    float2 dir2 = clamp(dir1.xy / dirAbsMinTimesC, -2.0, 2.0);
/*--------------------------------------------------------------------------*/
    float4 rgbyN2 = tex2D(SceneSampler, pos.xy - dir2 * fxaaConsoleRcpFrameOpt2.zw);
    float4 rgbyP2 = tex2D(SceneSampler, pos.xy + dir2 * fxaaConsoleRcpFrameOpt2.zw);
    
/*--------------------------------------------------------------------------*/
	//ブレンドブレンド。
    float4 rgbyA = rgbyN1 + rgbyP1;
    float4 rgbyB = ((rgbyN2 + rgbyP2) * 0.25) + (rgbyA * 0.25);
/*--------------------------------------------------------------------------*/
    int twoTap = (rgbyB.y < lumaMin) || (rgbyB.y > lumaMax);
    
    if(twoTap){
		//まだ輝度の差が大きいので、再度ブレンド。
		rgbyB.xyz = rgbyA.xyz * 0.5;
	}
    return rgbyB;
    
}
/*!
 *@brief	ピクセルシェーダー。
 */
float4 PSMain( VS_OUTPUT In ) : COLOR0
{
	float4 rcpFrame = float4(0.0f, 0.0f, 1.0f/g_sceneTexSize.x, 1.0f/g_sceneTexSize.y);
	return FxaaPixelShader( 
		In.uv,
        rcpFrame,							// float4 fxaaConsoleRcpFrameOpt,
        rcpFrame,							// float4 fxaaConsoleRcpFrameOpt2,
        0.166f,								// FxaaFloat fxaaQualityEdgeThreshold,
        0.0833f,							// FxaaFloat fxaaQualityEdgeThresholdMin,
        1.0f,								// FxaaFloat fxaaConsoleEdgeSharpness,
        0.4f,								// FxaaFloat fxaaConsoleEdgeThreshold,
        0.0833f								// FxaaFloat fxaaConsoleEdgeThresholdMin,
	);
}

technique FXAA
{
	pass p0
	{
		VertexShader	= compile vs_3_0 VSMain();
		PixelShader		= compile ps_3_0 PSMain();
	}
}
