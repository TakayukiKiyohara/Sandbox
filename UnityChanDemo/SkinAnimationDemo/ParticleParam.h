/*!
 * @brief	�p�[�e�B�N���p�����[�^
 */

#pragma once

#include "tkEngine/particle/tkParticleEmitter.h"

static SParicleEmitParameter bloodEmitterParam[] = {
	{
		"Assets/effect/blood2.png",	//const char* texturePath;						//!<�e�N�X�`���̃t�@�C���p�X�B
		CVector3(0.0f, 1.0f, 0.0f),		//CVector3	initVelocity;						//!<�����x�B
		0.2f,						//float		life;									//!<�����B�P�ʂ͕b�B
		10.0f,							//float		intervalTime;						//!<�������ԁB�P�ʂ͕b�B
		0.5f,							//float		w;									//!<�p�[�e�B�N���̕��B
		0.5f,							//float		h;									//!<�p�[�e�B�N���̍����B
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	initPositionRandomMargin;			//!<�����ʒu�̃����_�����B
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	initVelocityVelocityRandomMargin;	//!<�����x�̃����_�����B
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	addVelocityRandomMargih;			//!<���x�̐ϕ��̂Ƃ��̃����_�����B
		{											//!<CVector4	uvTable[4];			UV�e�[�u���B�ő�4�܂ŕێ��ł���Bx�������u�Ay�������v�Az���E����u�Aw���E����v�ɂȂ�B
			CVector4(0.0f,  0.0f, 1.0f, 1.0f),
			CVector4(0.25f, 0.0f, 0.5f,  0.5f),
			CVector4(0.5f,  0.0f, 0.75f, 0.5f),
			CVector4(0.75f, 0.0f, 1.0f,  0.5f),
		},
		0,								//int			uvTableSize;						//!<UV�e�[�u���̃T�C�Y�B
		CVector3(0.0f, -9.8f, 0.0f),	//CVector3	gravity;							//!<�d�́B
		true,							//bool		isFade;								//!<���ʂƂ��Ƀt�F�[�h�A�E�g����H
		0.5f,							//float		fadeTime;							//!<�t�F�[�h���鎞�ԁB
		1.0f,							//float		initAlpha;							//!<�����A���t�@�l�B
		true,							//bool		isBillboard;						//!<�r���{�[�h�H
		2.0f,							//float		brightness;							//!<�P�x�B�u���[�����L���ɂȂ��Ă���Ƃ������������ƌ������܂��B
		0,								//int		alphaBlendMode;						//!<0�����������A1���Z�����B
		CVector3(0.5f, 0.5f, 0.5f),		//CVector3	mulColor;							//!<��Z�J���[�B
	},
	{
		"Assets/effect/Blood1.png",	//const char* texturePath;						//!<�e�N�X�`���̃t�@�C���p�X�B
			CVector3(0.0f, 1.0f, 0.0f),		//CVector3	initVelocity;						//!<�����x�B
		0.2f,						//float		life;									//!<�����B�P�ʂ͕b�B
		10.0f,							//float		intervalTime;						//!<�������ԁB�P�ʂ͕b�B
		0.5f,							//float		w;									//!<�p�[�e�B�N���̕��B
		0.5f,							//float		h;									//!<�p�[�e�B�N���̍����B
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	initPositionRandomMargin;			//!<�����ʒu�̃����_�����B
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	initVelocityVelocityRandomMargin;	//!<�����x�̃����_�����B
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	addVelocityRandomMargih;			//!<���x�̐ϕ��̂Ƃ��̃����_�����B
		{											//!<CVector4	uvTable[4];			UV�e�[�u���B�ő�4�܂ŕێ��ł���Bx�������u�Ay�������v�Az���E����u�Aw���E����v�ɂȂ�B
			CVector4(0.0f,  0.0f, 1.0f, 1.0f),
			CVector4(0.25f, 0.0f, 0.5f,  0.5f),
		CVector4(0.5f,  0.0f, 0.75f, 0.5f),
		CVector4(0.75f, 0.0f, 1.0f,  0.5f),
		},
		0,								//int			uvTableSize;						//!<UV�e�[�u���̃T�C�Y�B
		CVector3(0.0f, -9.8f, 0.0f),	//CVector3	gravity;							//!<�d�́B
		true,							//bool		isFade;								//!<���ʂƂ��Ƀt�F�[�h�A�E�g����H
		0.5f,							//float		fadeTime;							//!<�t�F�[�h���鎞�ԁB
		1.0f,							//float		initAlpha;							//!<�����A���t�@�l�B
		true,							//bool		isBillboard;						//!<�r���{�[�h�H
		2.0f,							//float		brightness;							//!<�P�x�B�u���[�����L���ɂȂ��Ă���Ƃ������������ƌ������܂��B
		0,								//int		alphaBlendMode;						//!<0�����������A1���Z�����B
		CVector3(0.5f, 0.5f, 0.5f),		//CVector3	mulColor;							//!<��Z�J���[�B
	},
	{
		"Assets/effect/blood6.png",	//const char* texturePath;						//!<�e�N�X�`���̃t�@�C���p�X�B
			CVector3(0.0f, 1.0f, 0.0f),		//CVector3	initVelocity;						//!<�����x�B
		0.2f,						//float		life;									//!<�����B�P�ʂ͕b�B
		10.0f,							//float		intervalTime;						//!<�������ԁB�P�ʂ͕b�B
		0.5f,							//float		w;									//!<�p�[�e�B�N���̕��B
		0.5f,							//float		h;									//!<�p�[�e�B�N���̍����B
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	initPositionRandomMargin;			//!<�����ʒu�̃����_�����B
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	initVelocityVelocityRandomMargin;	//!<�����x�̃����_�����B
		CVector3(0.0f, 0.0f, 0.0f),		//CVector3	addVelocityRandomMargih;			//!<���x�̐ϕ��̂Ƃ��̃����_�����B
		{											//!<CVector4	uvTable[4];			UV�e�[�u���B�ő�4�܂ŕێ��ł���Bx�������u�Ay�������v�Az���E����u�Aw���E����v�ɂȂ�B
			CVector4(0.0f,  0.0f, 1.0f, 1.0f),
			CVector4(0.25f, 0.0f, 0.5f,  0.5f),
		CVector4(0.5f,  0.0f, 0.75f, 0.5f),
		CVector4(0.75f, 0.0f, 1.0f,  0.5f),
		},
		0,								//int			uvTableSize;						//!<UV�e�[�u���̃T�C�Y�B
		CVector3(0.0f, -9.8f, 0.0f),	//CVector3	gravity;							//!<�d�́B
		true,							//bool		isFade;								//!<���ʂƂ��Ƀt�F�[�h�A�E�g����H
		0.5f,							//float		fadeTime;							//!<�t�F�[�h���鎞�ԁB
		1.0f,							//float		initAlpha;							//!<�����A���t�@�l�B
		true,							//bool		isBillboard;						//!<�r���{�[�h�H
		2.0f,							//float		brightness;							//!<�P�x�B�u���[�����L���ɂȂ��Ă���Ƃ������������ƌ������܂��B
		0,								//int		alphaBlendMode;						//!<0�����������A1���Z�����B
		CVector3(0.5f, 0.5f, 0.5f),		//CVector3	mulColor;							//!<��Z�J���[�B
	},

};
