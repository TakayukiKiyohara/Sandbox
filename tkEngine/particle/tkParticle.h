/*!
 * @brief	�p�[�e�B�N���B
 */
#ifndef _TKPARTICLE_H_
#define _TKPARTICLE_H_

namespace tkEngine{
	class CRandom;
	class CCamera;
	struct SParicleEmitParameter;
	/*!
	 * @brief	�p�[�e�B�N���B
	 */
	class CParticle : public IGameObject{
		enum eState {
			eStateRun,
			eStateFadeOut,
			eStateDead,
		};
	private:
		CPrimitive		primitive;					//!<�v���~�e�B�u�B
		CTexture*		texture;					//!<�e�N�X�`���B
		CEffect*		shaderEffect;				//!<�V�F�[�_�[�G�t�F�N�g�B
		const CCamera*	camera;						//!<�J�����B
		CRandom*		random;						//!<�����B
		float			life;						//!<���C�t�B
		float			timer;						//!<�^�C�}�[�B
		CVector3		velocity;					//!<���x�B
		CVector3		position;					//!<���W�B
		CVector3		gravity;					//!<�d�́B
		float			rotateZ;					//!<Z������̉�]�B
		CMatrix			mWorld;						//!<���[���h�s��B
		CVector3		addVelocityRandomMargih;	//!<���x�̐ϕ��̂Ƃ��̃����_�����B
		bool			isDead;						//!<���S�t���O�B
		bool			isFade;						//!<���ʂƂ��Ƀt�F�[�h�A�E�g����H
		float			fadeTIme;					//!<�t�F�[�h�̎��ԁB
		eState			state;						//!<��ԁB
		float			initAlpha;					//!<�����A���t�@�B
		float			alpha;						//!<�A���t�@�B
		bool			isBillboard;				//!<�r���{�[�h�H
		CVector3		applyForce;					//!<�O����������́B
		float			brightness;					//!<�P�x�B�u���[�����L���ɂȂ��Ă���Ƃ������������ƌ������܂��B
		int				alphaBlendMode;				//!<0�����������A1���Z�����B
		CVector3		mulColor;					//!<��Z�J���[�B
	public:
		CParticle();
		~CParticle();
		void Init(CRandom& random, const CCamera& camera, const SParicleEmitParameter& param, const CVector3& emitPosition);
		void Start() override ;
		void Update() override;
		void Render( CRenderContext& renderContext ) override;
		/*!
		*@brief	�p�[�e�B�N���ɗ͂�������B
		*@param[in]	applyForce		���������Ɏg�p���闐�������@�B
		*/
		void ApplyForce(const CVector3& applyForce)
		{
			this->applyForce = applyForce;
		}
	};
}

#endif //_TKPARTICLE_H_