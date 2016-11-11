/*!
 * @brief	�p�[�e�B�N���̔����@�B
 */

#ifndef _TKPARTICLEEMITTER_H_
#define _TKPARTICLEEMITTER_H_


namespace tkEngine{
	class CCamera;
	class CRandom;
	class CParticle;
	/*!
	* @brief	�p�[�e�B�N�������p�����[�^
	*/
	struct SParicleEmitParameter {
		//�������B
		void Init()
		{
			memset(this, 0, sizeof(SParicleEmitParameter));
			initAlpha = 1.0f;
			uvTableSize = 1;
			brightness = 1.0f;
			isBillboard = true;
			mulColor = CVector3::Zero;
		}
		const char* texturePath;						//!<�e�N�X�`���̃t�@�C���p�X�B
		CVector3	initVelocity;						//!<�����x�B
		float		life;								//!<�����B�P�ʂ͕b�B
		float		intervalTime;						//!<�������ԁB�P�ʂ͕b�B
		float		w;									//!<�p�[�e�B�N���̕��B
		float		h;									//!<�p�[�e�B�N���̍����B
		CVector3	initPositionRandomMargin;			//!<�����ʒu�̃����_�����B
		CVector3	initVelocityVelocityRandomMargin;	//!<�����x�̃����_�����B
		CVector3	addVelocityRandomMargih;			//!<���x�̐ϕ��̂Ƃ��̃����_�����B
		CVector4	uvTable[4];							//!<UV�e�[�u���B�ő�4�܂ŕێ��ł���Bx�������u�Ay�������v�Az���E����u�Aw���E����v�ɂȂ�B
		int			uvTableSize;						//!<UV�e�[�u���̃T�C�Y�B
		CVector3	gravity;							//!<�d�́B
		bool		isFade;								//!<���ʂƂ��Ƀt�F�[�h�A�E�g����H
		float		fadeTime;							//!<�t�F�[�h���鎞�ԁB
		float		initAlpha;							//!<�����A���t�@�l�B
		bool		isBillboard;						//!<�r���{�[�h�H
		float		brightness;							//!<�P�x�B�u���[�����L���ɂȂ��Ă���Ƃ������������ƌ������܂��B
		int			alphaBlendMode;						//!<0�����������A1���Z�����B
		CVector3	mulColor;							//!<��Z�J���[�B
	};
	/*!
	 * @brief	�p�[�e�B�N���̔����@
	 */
	class CParticleEmitter : public IGameObject{
	public:
		
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CParticleEmitter();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CParticleEmitter();
		/*!
		 *@brief	�������B
		 *@param[in]	random		���������Ɏg�p���闐�������@�B
		 *@param[in]	camera		�p�[�e�B�N���̕`��Ŏg�p����J�����B
		 *@param[in]	param		�p�[�e�B�N�������p�̃p�����[�^�B
		 *@param[in]	emitPosition	�G�~�b�^�[�̍��W�B
		 */
		void Init(CRandom& random, const CCamera& camera, const SParicleEmitParameter& param, const CVector3& emitPosition);
		void Start() override ;
		void Update() override;
		void Render( CRenderContext& renderContext ) override;
		/*!
		*@brief	�p�[�e�B�N���ɗ͂�������B
		*@param[in]	applyForce		���������Ɏg�p���闐�������@�B
		*/
		void ApplyForce(const CVector3& applyForce);
	private:
		float					timer;			//!<�^�C�}�[
		CRandom*				random;			//!<���������@�B
		const CCamera*			camera;			//!<�J�����B
		SParicleEmitParameter	param;			//!<�p�����[�^�B
		CVector3				emitPosition;	//!<�G�~�b�^�[�̍��W�B
		std::list<CParticle*>	particleList;	//!<�p�[�e�B�N���̃��X�g�B
		std::list<CParticle*>	deleteParticleList;	//!<�폜���ꂽ�p�[�e�B�N���̃��X�g�B
	};
}

#endif // _TKPARTICLEEMITTER_H_