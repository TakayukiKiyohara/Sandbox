/*!
 * @brief	プリコンパイル済みヘッダー。
 */

#ifndef _STDAFX_H_
#define _STDAFX_H_


#include <windows.h>
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkSkinModelData.h"
#include "tkEngine/graphics/tkSkinModel.h"
#include "tkEngine/graphics/tkAnimation.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/graphics/tkCamera.h"
#include "tkEngine/graphics/tkLight.h"
#include "tkEngine/graphics/tkTexture.h"
#include "tkEngine/resource/tkSkinModelDataHandle.h"
#include "tkEngine/graphics/tkSkinModelMaterial.h"
#include "tkEngine/Physics/tkPhysics.h"
using namespace tkEngine;
#include "GameCamera.h"
#include "tkEngine/random/tkRandom.h"

class Player;

extern Player* g_player;
extern CRandom g_random;
#endif // _STDAFX_H_