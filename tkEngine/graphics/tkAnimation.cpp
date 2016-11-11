#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkAnimation.h"

namespace tkEngine{
	CAnimation::~CAnimation()
	{
		if(pAnimController != nullptr){
			pAnimController->Release();
		}
	}
	void CAnimation::Init(ID3DXAnimationController* anim)
	{
		pAnimController = anim;
		ID3DXAnimationSet* animSet;
		pAnimController->GetAnimationSet(0, &animSet);
		numAnimSet = pAnimController->GetMaxNumAnimationSets();
		numMaxTracks = pAnimController->GetMaxNumTracks();
		blendRateTable.reset( new float[numMaxTracks] );
		animationEndTime.reset(new double[numAnimSet]);
		animationSets.reset(new ID3DXAnimationSet*[numAnimSet]);
		animationLoopFlags.reset(new bool[numAnimSet]);
		for( int i = 0; i < numMaxTracks; i++ ){
			blendRateTable[i] = 1.0f;
		}
		//アニメーションセットを初期化。
		for (int i = 0; i < numAnimSet; i++) {
			pAnimController->GetAnimationSet(i, &animationSets[i]);
			animationEndTime[i] = -1.0;
			animationLoopFlags[i] = true;
		}
		localAnimationTime = 0.0;
	}

	void CAnimation::PlayAnimation(int animationSetIndex)
	{
		if (animationSetIndex < numAnimSet) {
			if (pAnimController) {
				isAnimEnd = false;
				isInterpolate = false;
				playAnimationRequest.clear();
				currentAnimationSetNo = animationSetIndex;
				currentTrackNo = 0;
				//0番目以外のトラックは無効にする。
				for (int i = 1; i < numMaxTracks; i++) {
					pAnimController->SetTrackEnable(i,  FALSE);
				}
				pAnimController->SetTrackWeight(0, 1.0f);
				pAnimController->SetTrackAnimationSet(currentTrackNo, animationSets[currentAnimationSetNo]);
				pAnimController->SetTrackEnable(0, TRUE);
				pAnimController->SetTrackPosition(0, 0.0f);
				localAnimationTime = 0.0;
			}
		}
		else {
			TK_LOG("warning!!! animationSetIndex is over range!!!!!");
		}
	}
	void CAnimation::PlayAnimation(int animationSetIndex, float interpolateTime)
	{
		if (animationSetIndex < numAnimSet) {
			if (pAnimController) {
				isAnimEnd = false;
				//補間開始の印。
				isInterpolate = true;
				this->interpolateTime = 0.0f;
				interpolateEndTime = interpolateTime;
				int prevTrackNo = currentTrackNo;
				currentTrackNo = (currentTrackNo + 1) % numMaxTracks;
				pAnimController->SetTrackAnimationSet(currentTrackNo, animationSets[animationSetIndex]);
				pAnimController->SetTrackSpeed(prevTrackNo, 0.0f);
				pAnimController->SetTrackEnable(currentTrackNo, TRUE);
				pAnimController->SetTrackSpeed(currentTrackNo, 1.0f);
				pAnimController->SetTrackPosition(currentTrackNo, 0.0f);
				localAnimationTime = 0.0;
				currentAnimationSetNo = animationSetIndex;
				UpdateTrackWeights();
			}
		}
		else {
			TK_LOG("warning!!! animationSetIndex is over range!!!!!");
		}
	}
	/*!
	*@brief	補間時間を元にトラックの重みを更新。
	*/
	void CAnimation::UpdateTrackWeights()
	{
		float weight = 0.0f;
		if (interpolateTime < interpolateEndTime) {
			weight = interpolateTime / interpolateEndTime;
			float invWeight = 1.0f - weight;
			//ウェイトを設定していく。
			for (int i = 0; i < numMaxTracks; i++) {
				if (i != currentTrackNo) {
					pAnimController->SetTrackWeight(i, blendRateTable[i] * invWeight);
				}
				else {
					pAnimController->SetTrackWeight(i, weight);
				}
			}
		}
		else {
			for (int i = 0; i < numMaxTracks; i++) {
				if (i != currentTrackNo) {
					pAnimController->SetTrackWeight(i, 0.0f);
				}
				else {
					pAnimController->SetTrackWeight(i, 1.0f);
				}
			}
		}
	}
	/*!
	*@brief	アニメーションの再生リクエストをポップ。
	*/
	void CAnimation::PopRequestPlayAnimation()
	{
		if (!playAnimationRequest.empty()) {
			RequestPlayAnimation& req = playAnimationRequest.front();
			PlayAnimation(req.animationSetIndex, req.interpolateTime);
			playAnimationRequest.pop_front();
		}
	}
	void CAnimation::Update(float deltaTime)
	{
		deltaTime *= animationSpeedRate;
		if (pAnimController && !isAnimEnd) {
			localAnimationTime += deltaTime;
			if (isInterpolate) {
				//補間中。
				interpolateTime += deltaTime;
				float weight = 0.0f;
				if (interpolateTime > interpolateEndTime) {
					//補間終了。
					isInterpolate = false;
					weight = 1.0f;
					//現在のトラック以外を無効にする。
					for (int i = 0; i < numMaxTracks; i++) {
						if (i != currentTrackNo) {
							pAnimController->SetTrackEnable(i, FALSE);
						}
					}
				}
				else {
					//各トラックの重みを更新。
					UpdateTrackWeights();
				}
			}
			if (animationEndTime[currentAnimationSetNo] > 0.0 //アニメーションの終了時間が設定されている。
				&& localAnimationTime > animationEndTime[currentAnimationSetNo] //アニメーションの終了時間を超えた。
			) {
				if (animationLoopFlags[currentAnimationSetNo]) {
					localAnimationTime -= animationEndTime[currentAnimationSetNo];
					pAnimController->SetTrackPosition(currentTrackNo, localAnimationTime);
					pAnimController->AdvanceTime(0, NULL);
				}
				else {
					isAnimEnd = true;
				}
			}
			else {
				//普通に再生。
				if (animationSets[currentAnimationSetNo]->GetPeriod() < localAnimationTime
					&& !animationLoopFlags[currentAnimationSetNo]) {
					localAnimationTime = animationSets[currentAnimationSetNo]->GetPeriod();
					isAnimEnd = true;
				}
				else {
					pAnimController->AdvanceTime(deltaTime, NULL);
				}
			}

			//ローカルタイムをトラックから取得して同期。
			D3DXTRACK_DESC trackDesk;
			pAnimController->GetTrackDesc(currentTrackNo, &trackDesk);
			localAnimationTime = fmod(trackDesk.Position, animationSets[currentAnimationSetNo]->GetPeriod());
		}

		if (isAnimEnd) {
			//アニメーション終わった。
			//アニメーションの連続再生のリクエストをポップする。
			PopRequestPlayAnimation();
		}
	}
}
