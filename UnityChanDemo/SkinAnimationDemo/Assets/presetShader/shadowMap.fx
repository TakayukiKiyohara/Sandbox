/*!
 * @brief	�V���h�E�}�b�v�B
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

float4x4	g_mWVP;		//���[���h�r���[�v���W�F�N�V�����s��B
float2		g_farNear;	//�����ʂƋߕ��ʁBx�ɉ����ʁAy�ɋߕ��ʁB
VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	
	Out.Position = mul(In.Position, g_mWVP);
	Out.depth = Out.Position;
	return Out;
}
float4 PSMain( VS_OUTPUT In ) : COLOR
{
	//�[�x���������ށB
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
