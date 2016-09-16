// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define USE_VSM							//定義でVSMが有効。

#define USE_ORTHO_PROJECTION
#define USE_BLOOM_FLOATING_BUFFER		//定義でブルームで浮動小数点バッファを使用する。

/*!
 * @brief	定義でDirectX9で動作
 */
#define TK_PLATFORM_DX9

#if defined( TK_PLATFORM_DX9 )
#include <windows.h>
#endif // #if defined( TK_PLATFORM_DX9 )
#include <memory>
#include <d3dx9.h>
#include <d3d9.h>
#include <XInput.h> // XInput API
#ifdef _DEBUG
#include <DxErr.h>
#pragma comment(lib, "dxerr.lib")
#endif
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include "tkEngine/typedef.h"
#include "tkEngine/graphics/tkFont.h"
#include "tkEngine/math/tkMath.h"
#include "tkEngine/debug/tkAssert.h"
#include "tkEngine/debug/tkLog.h"
#include "tkEngine/policy/tkNoncopyable.h"
#include "tkEngine/math/tkMatrix.h"
#include "tkEngine/math/tkVector.h"
#include "tkEngine/graphics/tkGraphicsType.h"
#include "tkEngine/Input/tkKeyInput.h"
#include "tkEngine/tkEngine.h"
#include "tkEngine/gameObject/tkGameObjectManager.h"
#include "tkEngine/gameObject/tkGameObject.h"
#include "tkEngine/graphics/tkRenderCommand.h"
#include "tkEngine/graphics/tkRenderContext.h"
#include "tkEngine/graphics/tkPIXPerfTag.h"


// TODO: reference additional headers your program requires here
