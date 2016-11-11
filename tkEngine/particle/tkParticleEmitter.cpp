/*!
 * @brief	�p�[�e�B�N�������@
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/particle/tkParticleEmitter.h"
#include "tkEngine/particle/tkParticle.h"
#include "tkEngine/shape/tkShapeVertex.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/graphics/tkCamera.h"

using namespace std;
namespace tkEngine{
	CParticleEmitter::CParticleEmitter()
	{

	}
	CParticleEmitter::~CParticleEmitter()
	{
	}
	void CParticleEmitter::Init(CRandom& random, const CCamera& camera, const SParicleEmitParameter& param, const CVector3& emitPosition )
	{
		this->random = &random;
		this->camera = &camera;
		this->param = param;
		this->emitPosition = emitPosition;
		timer = param.intervalTime;
	}
	void CParticleEmitter::Start()
	{
	}
	void CParticleEmitter::Update()
	{
		if (timer >= param.intervalTime) {
			//�p�[�e�B�N���𐶐��B
			CParticle* p = GameObjectManager().NewGameObject<CParticle>(0);
			p->Init(*random, *camera, param, emitPosition);
			timer = 0.0f;
			particleList.push_back(p);
		}
		timer += 1.0f / 60.0f;
		list<CParticle*>::iterator p = particleList.begin();
		while(p != particleList.end()){
			if ((*p)->IsDead()) {
				p = particleList.erase(p);
			}
			else {
				p++;
			}
		}
	}
	/*!
	*@brief	�p�[�e�B�N���ɗ͂�������B
	*@param[in]	applyForce		���������Ɏg�p���闐�������@�B
	*/
	void CParticleEmitter::ApplyForce(const CVector3& applyForce)
	{
		for (auto p : particleList) {
			p->ApplyForce(applyForce);
		}
	}
	void CParticleEmitter::Render( CRenderContext& renderContext )
	{
		
	}
}
