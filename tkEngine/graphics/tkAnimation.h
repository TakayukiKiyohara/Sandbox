/*!
 * @brief	アニメーションクラス。
 */
#ifndef _TKANIMATION_H_
#define _TKANIMATION_H_

#include <deque>

namespace tkEngine{
	/*!
	 * @brief	アニメーションクラス。
	 */
	class CAnimation : Noncopyable{
	public:
		/*!
		 * @brief	コンストラクタ
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
		 * @brief	デストラクタ。
		 */
		~CAnimation();
		/*!
		 * @brief	初期化。
		 *@param[in]	anim		アニメーションコントローラー。
		 */
		void Init(ID3DXAnimationController* anim);
		/*!
		* @brief	アニメーションの終了タイムを設定する。
		*@param[in]	animationSetIndex		アニメーションインデックス。
		*@param[in] endTime					アニメーションの終了タイム。-1.0を設定するとアニメーションファイルに設定されている終了タイムになる。
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
		/*!
		*@brief	アニメーションの再生。
		*@param[in]		animationIndex		再生したいアニメーションのインデックス。
		*/
		void PlayAnimation(int animationSetIndex);
		/*!
		*@brief	アニメーションの再生。アニメーションの補完が行われます。
		*@param[in]		animationSetIndex	再生したいアニメーションのインデックス。
		*@param[in]		interpolateTime		補間時間。
		*/
		void PlayAnimation(int animationSetIndex, float interpolateTime);
#if 0
		/*!
		*@brief	アニメーションのブレンディング再生。
		*@param		animationIndex		再生したいアニメーションのインデックス。
		*/
		void BlendAnimation(int animationSetIndex);
#endif
		/*!
		*@brief	アニメーションセットの取得。
		*/
		int GetNumAnimationSet() const
		{
			return numAnimSet;
		}
		/*!
		*@brief	アニメーションの更新。
		*@param[in]		deltaTime	更新時間。単位は秒。
		*/
		void Update( float deltaTime );
	private:
		//アニメーション再生リクエスト。
		struct RequestPlayAnimation {
			int animationSetIndex;
			float interpolateTime;
		};
		ID3DXAnimationController*				pAnimController;		//!<アニメーションコントローラ。
		int										numAnimSet;				//!<アニメーションセットの数。
		std::unique_ptr<ID3DXAnimationSet*[]>	animationSets;			//!<アニメーションセットの配列。
		std::unique_ptr<float[]>				blendRateTable;			//!<ブレンディングレートのテーブル。
		std::unique_ptr<double[]>				animationEndTime;		//!<アニメーションの終了タイム。デフォルトは-1.0が入っていて、-1.0が入っている場合はID3DXAnimationSetのアニメーション終了タイムが優先される。
																		//!<DirectX9のアニメーションセットに１秒以下のアニメーションを入れる方法が見つからない。1秒以下のアニメーションはこいつを適時設定。
		double									localAnimationTime;		//!<ローカルアニメーションタイム。
		int										currentAnimationSetNo;	//!<現在再生中のアニメーショントラックの番号。
		int										currentTrackNo;			//!<現在のトラックの番号。
		int										numMaxTracks;			//!<アニメーショントラックの最大数。
		bool									isBlending;				//!<アニメーションブレンディング中？
		bool									isInterpolate;			//!<補間中？
		float									interpolateEndTime;		//!<補間終了時間。
		float									interpolateTime;		//!<補間時間。
		std::deque<RequestPlayAnimation>		playAnimationRequest;	//!<アニメーション再生のリクエスト。
	};
}

#endif // _TKMOTION_H_

