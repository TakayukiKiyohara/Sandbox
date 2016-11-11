/*!
 *@brief	スプライト表示クラス。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/sprite/tkSprite.h"
#include "tkEngine/shape/tkShapeVertex.h"
#include "tkEngine/graphics/tkEffect.h"

namespace tkEngine{
	CSprite::CSprite()
	{
	}
	CSprite::~CSprite()
	{
	}
	void CSprite::Init(CTexture* texture)
	{
		m_effect = EffectManager().LoadEffect("Assets/presetShader/sprite.fx");
		static SShapeVertex_PT vertex[]{
			{
				-1.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 0.0f
			},
			{
				1.0f, 1.0f, 0.0f, 1.0f,
				1.0f, 0.0f
			},
			{
				-1.0f, -1.0f, 0.0f, 1.0f,
				0.0f, 1.0f
			},
			{
				1.0f, -1.0f, 0.0f, 1.0f,
				1.0f, 1.0f
			},
		};
		static unsigned short index[] = {
			0,1,2,3
		};
		m_primitive.Create(
			CPrimitive::eTriangleStrip,
			4,
			sizeof(SShapeVertex_PT),
			scShapeVertex_PT_Element,
			vertex,
			4,
			eIndexFormat16,
			index
			);
		m_texture = texture;
		m_size.x = s_cast<float>(m_texture->GetWidth());
		m_size.y = s_cast<float>(m_texture->GetHeight());
	}
	void CSprite::Draw(CRenderContext& renderContext)
	{
		CMatrix mWorld;
		mWorld = CMatrix::Identity;
		CVector3 scale;
		int frameBufferW = Engine().GetFrameBufferWidth();
		int frameBufferH = Engine().GetFrameBufferHeight();
		int frameBufferHalfW = frameBufferW / 2;
		int framebufferHalfH = frameBufferH / 2;
		scale.x = m_size.x / frameBufferW;
		scale.y = m_size.y / frameBufferH;
		scale.z = 1.0f;
		mWorld.MakeScaling(scale);
		CMatrix mTrans = CMatrix::Identity;
		CVector3 trans;
		trans.x = m_pos.x / frameBufferHalfW;
		trans.y = m_pos.y / framebufferHalfH;
		trans.z = 0.0f;
		//ピボットの分のオフセットを計算。
		CVector3 pivotOffset;
		pivotOffset.x = (m_size.x * (0.5f - m_pivot.x)) / frameBufferHalfW;
		pivotOffset.y = (m_size.y * (0.5f - m_pivot.y)) / framebufferHalfH;
		pivotOffset.z = 0.0f;
		trans.Add(pivotOffset);

		mTrans.MakeTranslation(trans);
		CMatrix mRot;
		mRot.MakeRotationFromQuaternion(m_rotation);
		mWorld.Mul(mWorld, mRot);
		mWorld.Mul(mWorld, mTrans);

		m_effect->Begin(renderContext);
		m_effect->BeginPass(renderContext, 0);
		m_effect->SetTechnique(renderContext, "SpriteTexture");
		m_effect->SetValue(renderContext, "mWorld", &mWorld, sizeof(mWorld));
		m_effect->SetTexture(renderContext, "g_tex", m_texture);
		m_effect->CommitChanges(renderContext);
		renderContext.SetVertexDeclaration(m_primitive.GetVertexDecl());
		renderContext.SetStreamSource(0, m_primitive.GetVertexBuffer());
		renderContext.SetIndices(m_primitive.GetIndexBuffer());
		renderContext.DrawIndexedPrimitive(&m_primitive);
		m_effect->EndPass(renderContext);
		m_effect->End(renderContext);
	}
}
