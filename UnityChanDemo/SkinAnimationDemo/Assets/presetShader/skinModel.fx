/*!
 * @brief	�X�L�����f���V�F�[�_�[�B(4�{�[���X�L�j���O)
 */
#include "Common.h" 
#include "LightingFunction.h"
#include "ShadowFunction.h"


//�X�L���s��B
#define MAX_MATRICES  26
float4x3    g_mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;
float4x4    g_mViewProj : VIEWPROJECTION;
float		g_numBone;			//���̐��B
float4x4	g_worldMatrix;			//!<���[���h�s��B
float4x4	g_rotationMatrix;		//!<��]�s��B
float4x4	g_viewMatrixRotInv;		//!<�J�����̉�]�s��̋t�s��B
float4x4	g_mViewProjLastFrame;	//!<1�t���[���O�̃r���[�v���W�F�N�V�����s��B
float4		g_fogParam;				//�t�H�O�̃p�����[�^�Bx�Ƀt�H�O���|����n�߂�[�x�By�Ƀt�H�O�����S�ɂ�����[�x�Bz�̓t�H�O���v�Z���邩�ǂ����̃t���O�B

float2		g_farNear;	//�����ʂƋߕ��ʁBx�ɉ����ʁAy�ɋߕ��ʁB

int4 g_flags;				//x�ɖ@���}�b�v�Ay�̓V���h�E���V�[�o�[�Az�̓������C�g�Aw�̓X�y�L�����}�b�v�B
int4 g_flags2;				//x�ɑ��x�}�b�v�ւ̏�������

texture g_diffuseTexture;		//�f�B�t���[�Y�e�N�X�`���B
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

//�@���}�b�v
texture g_normalTexture;		//�@���}�b�v�B
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
 * @brief	���͒��_
 */
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4  BlendWeights    : BLENDWEIGHT;
    float4  BlendIndices    : BLENDINDICES;
    float3  Normal          : NORMAL;
    float3	Tangent			: TANGENT;		//�ڃx�N�g��
    float3  Tex0            : TEXCOORD0;
};

/*!
 * @brief	�C���X�^���V���O�`��p�̓��͒��_�B
 */
struct VS_INPUT_INSTANCING
{
	VS_INPUT	base;
	float4 mWorld1	: TEXCOORD1;		//���[���h�s���1�s��
	float4 mWorld2	: TEXCOORD2;		//���[���h�s���2�s��
	float4 mWorld3	: TEXCOORD3;		//���[���h�s���3�s��
	float4 mWorld4	: TEXCOORD4;		//���[���h�s���4�s��
};

/*!
 * @brief	�o�͒��_�B
 */
struct VS_OUTPUT
{
	float4  Pos     		: POSITION;
    float3  Normal			: NORMAL;
    float2  Tex0   			: TEXCOORD0;
    float3	Tangent			: TEXCOORD1;	//�ڃx�N�g��
    float4  worldPos_depth	: TEXCOORD2;	//xyz�Ƀ��[���h���W�Bw�ɂ͎ˉe��Ԃł�depth���i�[�����B
    float4  lightViewPos_0	: TEXCOORD3;
    float4  lightViewPos_1	: TEXCOORD4;
    float4  lightViewPos_2	: TEXCOORD5;
    float4  velocity		: TEXCOORD6;	//���x�B
    float4  screenPos		: TEXCOORD7;
};

/*!
 * @brief	�V���h�E�}�b�v�`�����ݎ��̏o�͒��_�B
 */
struct VS_OUTPUT_RENDER_SHADOW_MAP
{
	float4  Pos     : POSITION;
	float4	depth	: TEXCOORD;
};

/*!
 * @brief	�s�N�Z���V�F�[�_�[����̏o�́B
 */
struct PSOutput{
	float4	color		: COLOR0;		//�����_�����O�^�[�Q�b�g0�ɏ������݁B
	float4	depth		: COLOR1;		//�����_�����O�^�[�Q�b�g1�ɏ������݁B
	float4  velocity 	: COLOR2;		//�����_�����O�^�[�Q�b�g2�ɏ������݁B
};

/*!
 *@brief	���[���h���W�ƃ��[���h�@�����X�L���s�񂩂�v�Z����B
 *@param[in]	In		���͒��_�B
 *@param[out]	Pos		���[���h���W�̊i�[��B
 *@param[out]	Normal	���[���h�@���̊i�[��B
 *@param[out]	Tangent	���[���h�ڃx�N�g���̊i�[��B
 */
void CalcWorldPosAndNormalFromSkinMatrix( VS_INPUT In, out float3 Pos, out float3 Normal, out float3 Tangent, uniform bool calcNormal )
{
	Pos = 0.0f;
	Normal = 0.0f;
	Tangent = 0.0f;
	//�u�����h����{�[���̃C���f�b�N�X�B
	int4 IndexVector = D3DCOLORtoUBYTE4(In.BlendIndices);
	
	//�u�����h���[�g�B
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
 *@brief	���[���h���W�ƃ��[���h�@�����v�Z�B
 *@param[in]	In		���͒��_�B
 *@param[out]	Pos		���[���h���W�̊i�[��B
 *@param[out]	Normal	���[���h�@���̊i�[��B
 *@param[out]	Tangent	���[���h�ڃx�N�g���̊i�[��B
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
 *@brief	���_�V�F�[�_�[�B
 *@param[in]	In			���͒��_�B
 *@param[in]	hasSkin		�X�L������H
 */
VS_OUTPUT VSMain( VS_INPUT In, uniform bool hasSkin )
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//�X�L������B
	    CalcWorldPosAndNormalFromSkinMatrix( In, Pos, Normal, Tangent, true );
	}else{
		//�X�L���Ȃ��B
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
		//�V���h�E���V�[�o�[�B
		o.lightViewPos_0 = mul(float4(Pos.xyz, 1.0f), gShadowReceiverParam.mLVP[0] );
		o.lightViewPos_1 = mul(float4(Pos.xyz, 1.0f), gShadowReceiverParam.mLVP[1] );
		o.lightViewPos_2 = mul(float4(Pos.xyz, 1.0f), gShadowReceiverParam.mLVP[2] );
	}
	return o;
}
/*!
 *@brief	���_�V�F�[�_�[�B
 *@param[in]	In			���͒��_�B
 *@param[in]	hasSkin		�X�L������H
 */
VS_OUTPUT VSMainInstancing( VS_INPUT_INSTANCING In, uniform bool hasSkin )
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//�X�L������B
	    CalcWorldPosAndNormalFromSkinMatrix( In.base, Pos, Normal, Tangent, true );
	}else{
		//�X�L���Ȃ��B
		CalcWorldPosAndNormal( In.base, Pos, Normal, Tangent, true );
	}
	float4x4 worldMat;
	worldMat[0] = In.mWorld1;
	worldMat[1] = In.mWorld2;
	worldMat[2] = In.mWorld3;
	worldMat[3] = In.mWorld4;
	Pos = mul(float4(Pos.xyz, 1.0f), worldMat );	//���[���h�s���������B

	o.worldPos_depth.xyz = Pos.xyz;
    o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
    o.worldPos_depth.w = o.Pos.w;
    o.Normal = mul(normalize(Normal), worldMat);
    o.Tex0 = In.base.Tex0;
   	o.velocity = mul(float4(Pos.xyz, 1.0f), g_mViewProjLastFrame);
   	o.screenPos = o.Pos;
    if(g_flags.y){
		//�V���h�E���V�[�o�[�B
		o.lightViewPos_0 = mul(float4(Pos.xyz, 1.0f), gShadowReceiverParam.mLVP[0] );
		o.lightViewPos_1 = mul(float4(Pos.xyz, 1.0f), gShadowReceiverParam.mLVP[1] );
		o.lightViewPos_2 = mul(float4(Pos.xyz, 1.0f), gShadowReceiverParam.mLVP[2] );
	}
	return o;
}

/*!
 * @brief	�s�N�Z���V�F�[�_�[�B
 */
PSOutput PSMain( VS_OUTPUT In )
{
	float4 color = tex2D(g_diffuseTextureSampler, In.Tex0);
	float3 normal = normalize(In.Normal);
	if(g_flags.x){
		//�@���}�b�v����B
		float3 tangent = normalize(In.Tangent);
		float3 binSpaceNormal = tex2D( g_normalMapSampler, In.Tex0);
		float4x4 tangentSpaceMatrix;
		float3 biNormal = normalize( cross( tangent, normal) );
		tangentSpaceMatrix[0] = float4( tangent, 0.0f);
		tangentSpaceMatrix[1] = float4( biNormal, 0.0f);
		tangentSpaceMatrix[2] = float4( normal, 0.0f);
		tangentSpaceMatrix[3] = float4( 0.0f, 0.0f, 0.0f, 1.0f );
		//-1.0�`1.0�͈̔͂Ƀ}�b�s���O����B
		binSpaceNormal = (binSpaceNormal * 2.0f)- 1.0f;
		normal = tangentSpaceMatrix[0] * binSpaceNormal.x + tangentSpaceMatrix[1] * binSpaceNormal.y + tangentSpaceMatrix[2] * binSpaceNormal.z; 
		
	}
	
	
	float4 lig = DiffuseLight(normal);
	if(g_flags.z){
		//�������C�g�B
		lig.xyz += CalcLimLight(normal, g_light.limLightDir, g_light.limLightColor.xyz);
	}
	if(g_flags.w){
		//�X�y�L�������C�g�B
		lig.xyz += SpecLight(normal, In.worldPos_depth.xyz, In.Tex0);
	}
	
	if(g_flags.y){
		//�e
		lig *= CalcShadow(In.lightViewPos_0, In.lightViewPos_1, In.lightViewPos_2);
	
	}
	//�|�C���g���C�g�B
	lig.xyz += PointLight(normal, In.worldPos_depth.xyz, g_flags.z);
	
	//�A���r�G���g���C�g�����Z�B
	lig.xyz += g_light.ambient.xyz;
	color.xyz *= lig;
	
	
	if(g_fogParam.z > 1.9f){
		//�����t�H�O
		float h = max(In.worldPos_depth.y - g_fogParam.y, 0.0f);
		float t = min(h / g_fogParam.x, 1.0f);
		color.xyz = lerp(float3(0.75f, 0.75f, 0.95f), color.xyz, t);
	}else if(g_fogParam.z > 0.0f){
		//�����t�H�O
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
		//���x�Ȃ��B
		psOut.velocity = 0.5f;
	}
	return psOut;
}


/*!
 * @brief	�V���h�E�}�b�v�`�����ݎ��̒��_�V�F�[�_�[�B
 */
VS_OUTPUT_RENDER_SHADOW_MAP VSMainRenderShadowMap( VS_INPUT In, uniform bool hasSkin )
{
	VS_OUTPUT_RENDER_SHADOW_MAP o = (VS_OUTPUT_RENDER_SHADOW_MAP)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//�X�L������B
	    CalcWorldPosAndNormalFromSkinMatrix( In, Pos, Normal, Tangent, false );
	}else{
		//�X�L���Ȃ��B
		CalcWorldPosAndNormal( In, Pos, Normal, Tangent, false );
	}
	o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
	o.depth = o.Pos;
	return o;
}
/*!
 * @brief	�V���h�E�}�b�v�������ݎ��̒��_�V�F�[�_�[(�C���X�^���V���O��)
 */
VS_OUTPUT_RENDER_SHADOW_MAP VSMainInstancingRenderShadowMap(VS_INPUT_INSTANCING In, uniform bool hasSkin)
{
	VS_OUTPUT_RENDER_SHADOW_MAP o = (VS_OUTPUT_RENDER_SHADOW_MAP)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//�X�L������B
	    CalcWorldPosAndNormalFromSkinMatrix( In.base, Pos, Normal, Tangent, false );
	}else{
		//�X�L���Ȃ��B
		CalcWorldPosAndNormal( In.base, Pos, Normal, Tangent, false );
	}
	float4x4 worldMat;
	worldMat[0] = In.mWorld1;
	worldMat[1] = In.mWorld2;
	worldMat[2] = In.mWorld3;
	worldMat[3] = In.mWorld4;
	Pos = mul(float4(Pos.xyz, 1.0f), worldMat );	//���[���h�s���������B
	o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
	o.depth = o.Pos;
	return o;
}
/*!
 * @brief	�V���h�E�}�b�v�`�����ݎ��̃s�N�Z���V�F�[�_�[�B
 */
float4 PSMainRenderShadowMap( VS_OUTPUT_RENDER_SHADOW_MAP In ) : COLOR
{

	float z = In.depth.z/In.depth.w;
	float dx = ddx(z);
	float dy = ddy(z);
	return float4(z, z*z+0.25f*(dx*dx+dy*dy), 0.0f, 1.0f);
}

/*!
 *@brief	�X�L�����胂�f���p�̃e�N�j�b�N�B
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
 *@brief	�X�L���Ȃ����f���p�̃e�N�j�b�N�B
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
 *@brief	�X�L�����胂�f���̃C���X�^���V���O�`��p�̃e�N�j�b�N�B
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
 *@brief	�X�L���Ȃ����f���̃C���X�^���V���O�`��p�̃e�N�j�b�N�B
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
 * @brief	�X�L�����胂�f���p�̃V���h�E�}�b�v�`�����݃e�N�j�b�N�B
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
 * @brief	�X�L���Ȃ����f���p�̃V���h�E�}�b�v�`�����݃e�N�j�b�N�B
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
