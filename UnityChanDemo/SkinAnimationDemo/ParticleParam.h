/*!
 * @brief	パーティクルパラメータ
 */

#pragma once

#include "tkEngine/particle/tkParticleEmitter.h"

static SParicleEmitParameter bloodEmitterParam[] = {
	{
		"Assets/effect/blood2.png",	//const char* texturePath;						//!<テクスチャのファイルパス。
		CVector3(0.0f, 1.0f, 0.0f),		//CVector3	initVelocity;						//!<初速度。
		0.2f,						//float		life;									//!<寿命。単位は秒。
		10.0f,							//float		intervalTime;						//!<発生時間。単位は秒。
		0.5f,							//float		w;									//!<パーティクルの幅。
		0.5f,							//float		h;									//!<パーティクルの高さ。
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	initPositionRandomMargin;			//!<初期位置のランダム幅。
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	initVelocityVelocityRandomMargin;	//!<初速度のランダム幅。
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	addVelocityRandomMargih;			//!<速度の積分のときのランダム幅。
		{											//!<CVector4	uvTable[4];			UVテーブル。最大4まで保持できる。xが左上のu、yが左上のv、zが右下のu、wが右下のvになる。
			CVector4(0.0f,  0.0f, 1.0f, 1.0f),
			CVector4(0.25f, 0.0f, 0.5f,  0.5f),
			CVector4(0.5f,  0.0f, 0.75f, 0.5f),
			CVector4(0.75f, 0.0f, 1.0f,  0.5f),
		},
		0,								//int			uvTableSize;						//!<UVテーブルのサイズ。
		CVector3(0.0f, -9.8f, 0.0f),	//CVector3	gravity;							//!<重力。
		true,							//bool		isFade;								//!<死ぬときにフェードアウトする？
		0.5f,							//float		fadeTime;							//!<フェードする時間。
		1.0f,							//float		initAlpha;							//!<初期アルファ値。
		true,							//bool		isBillboard;						//!<ビルボード？
		2.0f,							//float		brightness;							//!<輝度。ブルームが有効になっているとこれを強くすると光が溢れます。
		0,								//int		alphaBlendMode;						//!<0半透明合成、1加算合成。
		CVector3(0.5f, 0.5f, 0.5f),		//CVector3	mulColor;							//!<乗算カラー。
	},
	{
		"Assets/effect/Blood1.png",	//const char* texturePath;						//!<テクスチャのファイルパス。
			CVector3(0.0f, 1.0f, 0.0f),		//CVector3	initVelocity;						//!<初速度。
		0.2f,						//float		life;									//!<寿命。単位は秒。
		10.0f,							//float		intervalTime;						//!<発生時間。単位は秒。
		0.5f,							//float		w;									//!<パーティクルの幅。
		0.5f,							//float		h;									//!<パーティクルの高さ。
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	initPositionRandomMargin;			//!<初期位置のランダム幅。
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	initVelocityVelocityRandomMargin;	//!<初速度のランダム幅。
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	addVelocityRandomMargih;			//!<速度の積分のときのランダム幅。
		{											//!<CVector4	uvTable[4];			UVテーブル。最大4まで保持できる。xが左上のu、yが左上のv、zが右下のu、wが右下のvになる。
			CVector4(0.0f,  0.0f, 1.0f, 1.0f),
			CVector4(0.25f, 0.0f, 0.5f,  0.5f),
		CVector4(0.5f,  0.0f, 0.75f, 0.5f),
		CVector4(0.75f, 0.0f, 1.0f,  0.5f),
		},
		0,								//int			uvTableSize;						//!<UVテーブルのサイズ。
		CVector3(0.0f, -9.8f, 0.0f),	//CVector3	gravity;							//!<重力。
		true,							//bool		isFade;								//!<死ぬときにフェードアウトする？
		0.5f,							//float		fadeTime;							//!<フェードする時間。
		1.0f,							//float		initAlpha;							//!<初期アルファ値。
		true,							//bool		isBillboard;						//!<ビルボード？
		2.0f,							//float		brightness;							//!<輝度。ブルームが有効になっているとこれを強くすると光が溢れます。
		0,								//int		alphaBlendMode;						//!<0半透明合成、1加算合成。
		CVector3(0.5f, 0.5f, 0.5f),		//CVector3	mulColor;							//!<乗算カラー。
	},
	{
		"Assets/effect/blood6.png",	//const char* texturePath;						//!<テクスチャのファイルパス。
			CVector3(0.0f, 1.0f, 0.0f),		//CVector3	initVelocity;						//!<初速度。
		0.2f,						//float		life;									//!<寿命。単位は秒。
		10.0f,							//float		intervalTime;						//!<発生時間。単位は秒。
		0.5f,							//float		w;									//!<パーティクルの幅。
		0.5f,							//float		h;									//!<パーティクルの高さ。
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	initPositionRandomMargin;			//!<初期位置のランダム幅。
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	initVelocityVelocityRandomMargin;	//!<初速度のランダム幅。
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	addVelocityRandomMargih;			//!<速度の積分のときのランダム幅。
		{											//!<CVector4	uvTable[4];			UVテーブル。最大4まで保持できる。xが左上のu、yが左上のv、zが右下のu、wが右下のvになる。
			CVector4(0.0f,  0.0f, 1.0f, 1.0f),
			CVector4(0.25f, 0.0f, 0.5f,  0.5f),
		CVector4(0.5f,  0.0f, 0.75f, 0.5f),
		CVector4(0.75f, 0.0f, 1.0f,  0.5f),
		},
		0,								//int			uvTableSize;						//!<UVテーブルのサイズ。
		CVector3(0.0f, -9.8f, 0.0f),	//CVector3	gravity;							//!<重力。
		true,							//bool		isFade;								//!<死ぬときにフェードアウトする？
		0.5f,							//float		fadeTime;							//!<フェードする時間。
		1.0f,							//float		initAlpha;							//!<初期アルファ値。
		true,							//bool		isBillboard;						//!<ビルボード？
		2.0f,							//float		brightness;							//!<輝度。ブルームが有効になっているとこれを強くすると光が溢れます。
		0,								//int		alphaBlendMode;						//!<0半透明合成、1加算合成。
		CVector3(0.5f, 0.5f, 0.5f),		//CVector3	mulColor;							//!<乗算カラー。
	},

};
