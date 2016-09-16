/*!
 * @brief	パーティクル。
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/particle/tkParticleEmitter.h"
#include "tkEngine/particle/tkParticle.h"
#include "tkEngine/shape/tkShapeVertex.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/graphics/tkCamera.h"
#include "tkEngine/random/tkRandom.h"

namespace tkEngine{
	CParticle::CParticle()
	{
		applyForce = CVector3::Zero;
		texture = nullptr;
	}
	CParticle::~CParticle()
	{
		primitive.Release();
	}
	void CParticle::Init(CRandom& random, CCamera& camera, const SParicleEmitParameter& param, const CVector3& emitPosition )
	{
		float halfW = param.w * 0.5f;
		float halfH = param.h * 0.5f;
		
		TK_ASSERT(param.uvTableSize <= ARRAYSIZE(param.uvTable), "uvTable size over!!!");
		CVector4 uv;
		if (param.uvTableSize > 0) {
			uv = param.uvTable[random.GetRandInt() % param.uvTableSize];
		}
		else {
			uv = param.uvTable[0];
		}
		
		SShapeVertex_PT vb[] = {
			{
				-halfW, halfH, 0.0f, 1.0f,
				uv.x, uv.y
			},
			{
				halfW, halfH, 0.0f, 1.0f,
				uv.z, uv.y
			},
			{
				-halfW, -halfH, 0.0f, 1.0f,
				uv.x, uv.w
			},
			{
				halfW, -halfH, 0.0f, 1.0f,
				uv.z, uv.w
			},
			
		};
		short index[]{
			0,1,2,3
		};
		primitive.Create(
			CPrimitive::eTriangleStrip,
			4,
			sizeof(SShapeVertex_PT),
			scShapeVertex_PT_Element,
			vb,
			4,
			eIndexFormat16,
			index
			);
		
		texture = ParticleResources().LoadTexture(param.texturePath);
		shaderEffect = EffectManager().LoadEffect("Assets/presetShader/ColorTexPrim.fx");
		this->camera = &camera;
		this->random = &random;
		life = param.life;
		velocity = param.initVelocity;
		//初速度に乱数を加える。
		velocity.x += (((float)random.GetRandDouble() - 0.5f) * 2.0f) * param.initVelocityVelocityRandomMargin.x;
		velocity.y += (((float)random.GetRandDouble() - 0.5f) * 2.0f) * param.initVelocityVelocityRandomMargin.y;
		velocity.z += (((float)random.GetRandDouble() - 0.5f) * 2.0f) * param.initVelocityVelocityRandomMargin.z;
		position = emitPosition;
		position.x += (((float)random.GetRandDouble() - 0.5f) * 2.0f) * param.initPositionRandomMargin.x;
		position.y += (((float)random.GetRandDouble() - 0.5f) * 2.0f) * param.initPositionRandomMargin.y;
		position.z += (((float)random.GetRandDouble() - 0.5f) * 2.0f) * param.initPositionRandomMargin.z;
		addVelocityRandomMargih = param.addVelocityRandomMargih;
		gravity = param.gravity;
		isFade = param.isFade;
		state = eStateRun;
		initAlpha = param.initAlpha;
		alpha = initAlpha;
		fadeTIme = param.fadeTime;
		isBillboard = param.isBillboard;
		brightness = param.brightness;
		alphaBlendMode = param.alphaBlendMode;
		rotateZ = CMath::PI * 2.0f * (float)random.GetRandDouble();
	}
	void CParticle::Start()
	{
	}
	void CParticle::Update()
	{
		float deltaTime = 1.0f / 60.0f;
		CVector3 addGrafity = gravity;
		addGrafity.Scale(deltaTime);
		velocity.Add(addGrafity);
		CVector3 force = applyForce;
		force.x += (((float)random->GetRandDouble() - 0.5f) * 2.0f) * addVelocityRandomMargih.x;
		force.y += (((float)random->GetRandDouble() - 0.5f) * 2.0f) * addVelocityRandomMargih.y;
		force.z += (((float)random->GetRandDouble() - 0.5f) * 2.0f) * addVelocityRandomMargih.z;
		force.Scale(deltaTime);
		velocity.Add(force);
		CVector3 addPos = velocity;
		addPos.Scale(deltaTime);
		applyForce = CVector3::Zero;

		position.Add(addPos);
		CMatrix mTrans;
		mTrans.MakeTranslation(position);
		if (isBillboard) {
			//ビルボード処理を行う。
			const CMatrix& mCameraRot = camera->GetCameraRotation();
			CQuaternion qRot;
			qRot.SetRotation(CVector3(mCameraRot.m[2][0], mCameraRot.m[2][1], mCameraRot.m[2][2]), rotateZ);
			CMatrix rot;
			rot.MakeRotationFromQuaternion(qRot);
			mWorld.Mul(mCameraRot, rot);
			mWorld.Mul(mWorld, mTrans);
		}
		else {
			mWorld = mTrans;
		}
		timer += deltaTime;
		switch (state) {
		case eStateRun:
			if (timer >= life) {
				if (isFade) {
					state = eStateFadeOut;
					timer = 0.0f;
				}
				else {
					state = eStateDead;
				}
			}
			break;
		case eStateFadeOut: {
			float t = timer / fadeTIme;
			timer += deltaTime;
			alpha = initAlpha + (-initAlpha)*t;
			if (alpha <= 0.0f) {
				alpha = 0.0f;
				state = eStateDead;	//死亡。
			}
		}break;
		case eStateDead:
			GameObjectManager().DeleteGameObject(this);
			break;
		}
		
	}
	void CParticle::Render( CRenderContext& renderContext )
	{
		CMatrix m;
		m.Mul(mWorld, camera->GetViewMatrix());
		m.Mul(m, camera->GetProjectionMatrix());
		renderContext.SetRenderState(RS_ALPHABLENDENABLE, TRUE);
		switch (alphaBlendMode) {
		case 0:
			renderContext.SetRenderState(RS_SRCBLEND, BLEND_SRCALPHA);
			renderContext.SetRenderState(RS_DESTBLEND, BLEND_INVSRCALPHA);
			shaderEffect->SetTechnique(renderContext, "ColorTexPrimTrans");
			break;
		case 1:
			renderContext.SetRenderState(RS_SRCBLEND, BLEND_ONE);
			renderContext.SetRenderState(RS_DESTBLEND, BLEND_ONE);
			renderContext.SetRenderState(RS_SEPARATEALPHABLENDENABL, TRUE);	//αはソースを書きこむ。
			renderContext.SetRenderState(RS_SRCBLENDALPHA, BLEND_ONE);
			renderContext.SetRenderState(RS_DESTBLENDALPHA, BLEND_ZERO);
			shaderEffect->SetTechnique(renderContext, "ColorTexPrimAdd");
			break;
		}
		
		shaderEffect->Begin(renderContext);
		shaderEffect->BeginPass(renderContext, 0);
		renderContext.SetRenderState(RS_ZENABLE, FALSE);

		shaderEffect->SetValue(renderContext, "g_mWVP", &m, sizeof(CMatrix));
		shaderEffect->SetValue(renderContext, "g_alpha", &alpha, sizeof(alpha));
		shaderEffect->SetValue(renderContext, "g_brightness", &brightness, sizeof(brightness));
		shaderEffect->SetTexture(renderContext, "g_texture", texture);
		shaderEffect->CommitChanges(renderContext);
		renderContext.SetStreamSource(0, primitive.GetVertexBuffer());
		renderContext.SetIndices(primitive.GetIndexBuffer());
		renderContext.SetVertexDeclaration(primitive.GetVertexDecl());
		renderContext.DrawIndexedPrimitive(&primitive);
		shaderEffect->EndPass(renderContext);
		shaderEffect->End(renderContext);
		renderContext.SetRenderState(RS_ALPHABLENDENABLE, FALSE);
		renderContext.SetRenderState(RS_SRCBLEND, BLEND_ONE);
		renderContext.SetRenderState(RS_DESTBLEND, BLEND_ZERO);
		renderContext.SetRenderState(RS_ZENABLE, TRUE);
		renderContext.SetRenderState(RS_SEPARATEALPHABLENDENABL, FALSE);	//αは乗算
		
	}
}