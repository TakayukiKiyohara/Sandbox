/*!
 * @brief	�A�j���[�V�����N���X�B
 */
#ifndef _TKANIMATION_H_
#define _TKANIMATION_H_

#include <deque>

namespace tkEngine{
	/*!
	 * @brief	�A�j���[�V�����N���X�B
	 */
	class CAnimation : Noncopyable{
	public:
		/*!
		 * @brief	�R���X�g���N�^
		 */
		CAnimation() :
			pAnimController(nullptr),
			numAnimSet(0),
			isBlending(false),
			isInterpolate(false),
			numMaxTracks(0),
			interpolateTime(0.0f),
			interpolateEndTime(0.0f)
		{
		}
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CAnimation();
		/*!
		 * @brief	�������B
		 *@param[in]	anim		�A�j���[�V�����R���g���[���[�B
		 */
		void Init(ID3DXAnimationController* anim);
		/*!
		* @brief	�A�j���[�V�����̏I���^�C����ݒ肷��B
		*@param[in]	animationSetIndex		�A�j���[�V�����C���f�b�N�X�B
		*@param[in] endTime					�A�j���[�V�����̏I���^�C���B-1.0��ݒ肷��ƃA�j���[�V�����t�@�C���ɐݒ肳��Ă���I���^�C���ɂȂ�B
		*/
		void SetAnimationEndTime(int animationSetIndex, double endTime)
		{
			if (animationSetIndex < numAnimSet) {
				animationEndTime[animationSetIndex] = endTime;
			}
			else {
				TK_LOG("warning animationSetIndex is invalid!!");
			}
		}
		void SetAnimationLoopFlag(int animationSetIndex, bool loopFlag)
		{
			if (animationSetIndex < numAnimSet) {
				animationLoopFlags[animationSetIndex] = loopFlag;
			}
			else {
				TK_LOG("warning animationSetIndex is invalid!!");
			}
		}
		/*!
		*@brief	�A�j���[�V�����̍Đ��B
		*@param[in]		animationIndex		�Đ��������A�j���[�V�����̃C���f�b�N�X�B
		*/
		void PlayAnimation(int animationSetIndex);
		/*!
		*@brief	�A�j���[�V�����̍Đ��B�A�j���[�V�����̕⊮���s���܂��B
		*@param[in]		animationSetIndex	�Đ��������A�j���[�V�����̃C���f�b�N�X�B
		*@param[in]		interpolateTime		��Ԏ��ԁB
		*/
		void PlayAnimation(int animationSetIndex, float interpolateTime);
		/*!
		*@brief	�A�j���[�V�����̍Đ����N�G�X�g���L���[�ɐςށB
		*@param[in]		animationSetIndex	�Đ��������A�j���[�V�����̃C���f�b�N�X�B
		*@param[in]		interpolateTime		��Ԏ��ԁB
		*/
		void PlayAnimationQueue(int animationSetIndex, float interpolateTime)
		{
			RequestPlayAnimation req;
			req.animationSetIndex = animationSetIndex;
			req.interpolateTime = interpolateTime;
			playAnimationRequest.push_back(req);
		}
		/*!
		*@brief	�Đ����̃A�j���[�V�����̔ԍ����擾�B
		*/
		int GetPlayAnimNo() const
		{
			return currentAnimationSetNo;
		}
		/*!
		*@brief	�A�j���[�V�����̍Đ�������B
		*/
		bool IsPlay() const
		{
			return !isAnimEnd;
		}
#if 0
		/*!
		*@brief	�A�j���[�V�����̃u�����f�B���O�Đ��B
		*@param		animationIndex		�Đ��������A�j���[�V�����̃C���f�b�N�X�B
		*/
		void BlendAnimation(int animationSetIndex);
#endif
		/*!
		*@brief	�A�j���[�V�����Z�b�g�̎擾�B
		*/
		int GetNumAnimationSet() const
		{
			return numAnimSet;
		}
		/*!
		*@brief	�A�j���[�V�����̍X�V�B
		*@param[in]		deltaTime	�X�V���ԁB�P�ʂ͕b�B
		*/
		void Update( float deltaTime );
		/*!
		*@brief	���[�J���A�j���[�V�����^�C���̎擾�B
		*@param[in]		���[�J���A�j���[�V�����^�C���B�P�ʂ͕b�B
		*/
		float GetLocalAnimationTime() const
		{
			return (float)localAnimationTime;
		}
		/*!
		*@brief	�A�j���[�V�����̍Đ����x�ɏ�Z�����l�B1.0�Œʏ�̃A�j���[�V�������x�B
		*/
		void SetAnimationSpeedRate(float rate)
		{
			animationSpeedRate = rate;
		}
	private:
		/*!
		*@brief	��Ԏ��Ԃ����Ƀg���b�N�̏d�݂��X�V�B
		*/
		void UpdateTrackWeights();
		/*!
		*@brief	�A�j���[�V�����̍Đ����N�G�X�g���|�b�v�B
		*/
		void PopRequestPlayAnimation();
	private:
		//�A�j���[�V�����Đ����N�G�X�g�B
		struct RequestPlayAnimation {
			int animationSetIndex;
			float interpolateTime;
		};
		ID3DXAnimationController*				pAnimController;		//!<�A�j���[�V�����R���g���[���B
		int										numAnimSet;				//!<�A�j���[�V�����Z�b�g�̐��B
		std::unique_ptr<ID3DXAnimationSet*[]>	animationSets;			//!<�A�j���[�V�����Z�b�g�̔z��B
		std::unique_ptr<float[]>				blendRateTable;			//!<�u�����f�B���O���[�g�̃e�[�u���B
		std::unique_ptr<double[]>				animationEndTime;		//!<�A�j���[�V�����̏I���^�C���B�f�t�H���g��-1.0�������Ă��āA-1.0�������Ă���ꍇ��ID3DXAnimationSet�̃A�j���[�V�����I���^�C�����D�悳���B
		std::unique_ptr<bool[]>					animationLoopFlags;		//!<�A�j���[�V�����̃��[�v�t���O�B																//!<DirectX9�̃A�j���[�V�����Z�b�g�ɂP�b�ȉ��̃A�j���[�V������������@��������Ȃ��B1�b�ȉ��̃A�j���[�V�����͂�����K���ݒ�B
		double									localAnimationTime;		//!<���[�J���A�j���[�V�����^�C���B
		int										currentAnimationSetNo;	//!<���ݍĐ����̃A�j���[�V�����g���b�N�̔ԍ��B
		int										currentTrackNo;			//!<���݂̃g���b�N�̔ԍ��B
		int										numMaxTracks;			//!<�A�j���[�V�����g���b�N�̍ő吔�B
		bool									isBlending;				//!<�A�j���[�V�����u�����f�B���O���H
		bool									isInterpolate;			//!<��Ԓ��H
		bool									isAnimEnd;				//!<�A�j���[�V�����̏I���t���O�B
		float									interpolateEndTime;		//!<��ԏI�����ԁB
		float									interpolateTime;		//!<��Ԏ��ԁB
		float									animationSpeedRate = 1.0f;		//!<�A�j���[�V�����̍Đ����x�ɏ�Z�����l�B1.0�Œʏ�̍Đ����x�B
		std::deque<RequestPlayAnimation>		playAnimationRequest;	//!<�A�j���[�V�����Đ��̃��N�G�X�g�B
	};
}

#endif // _TKMOTION_H_

