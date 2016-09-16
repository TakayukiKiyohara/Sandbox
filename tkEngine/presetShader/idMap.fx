/*!
 * @brief	IDMap
 */

#include "Common.h" 

struct VS_INPUT
{
	float4	Position	: POSITION;
};
struct VS_OUTPUT
{
	float4 Position	: POSITION;
};

float4x4 g_mWVP;    // World * View * Projection matrix


/*!
 * @brief	ID�}�b�v�`��̒��_�V�F�[�_�[
 */
VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	
	Out.Position = mul(In.Position, g_mWVP);
	
	return Out;
}

texture g_idMap;		//!<IDMap�B
float2 g_idMapSize;		//!<IDMap�̃T�C�Y�B

sampler TextureSampler = 
sampler_state
{
    Texture = <g_idMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

struct VS_OUTPUT_RENDER_EDGE
{
	float4 Position	: POSITION;
	float2 texCoord : TEXCOORD;
};

/*!
 * @brief	�G�b�W�`��p�̒��_�V�F�[�_�[
 */
VS_OUTPUT_RENDER_EDGE RenderEdgeVS( VS_INPUT In )
{
	VS_OUTPUT_RENDER_EDGE Out;
	Out.Position = In.Position;	//�g�����X�t�H�[���ςݒ��_�Ȃ̂ł��̂܂܁B
	Out.texCoord = (In.Position * 0.5f) + 0.5f;
	Out.texCoord.y = 1.0f - Out.texCoord.y;
	return Out;
}
/*!
 * @brief	ID�}�b�v�`��̃s�N�Z���V�F�[�_�[
 */
float4 PSMain( VS_OUTPUT In ) : COLOR
{
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

/*!
 * @brief	�G�b�W�`��p�̃s�N�Z���V�F�[�_�[
 */
float4 RenderEdgePS( VS_OUTPUT_RENDER_EDGE In ) : COLOR
{
	//���g�̃s�N�Z����ID
	float2 uv = In.texCoord.xy;
	float id = tex2D( TextureSampler, uv).a;
	//�㉺�S�s�N�Z���𒲂ׂ�B
	float2 offset = 1.0f / g_idMapSize;

	float id0 = tex2D( TextureSampler, float2(uv.x + offset.x, uv.y)).a;	//�E
	float id1 = tex2D( TextureSampler, float2(uv.x - offset.x, uv.y)).a;	//��
	float id2 = tex2D( TextureSampler, float2(uv.x, uv.y + offset.y)).a;	//��
	float id3 = tex2D( TextureSampler, float2(uv.x, uv.y - offset.y)).a;	//��
	//ID�̍��̐�Βl�����Z����B
	float t = abs(( id - id0 ))
			+ abs(( id - id1 ))
			+ abs(( id - id2 ))
			+ abs(( id - id3 ));
	//��Βl�̍���0.1�ȉ��Ȃ�s�N�Z���L���B
	clip(t - 0.1f);
	return float4(0.0, 0.0f, 0.0f, 1.0f);
}



technique RenderIDMap
{
	pass p0
	{
		VertexShader 	= compile vs_2_0 VSMain();
		PixelShader		= compile ps_2_0 PSMain();
	}
}
technique RenderEdge
{
	pass p0
	{
		VertexShader 	= compile vs_2_0 RenderEdgeVS();
		PixelShader 	= compile ps_2_0 RenderEdgePS();
	}
}