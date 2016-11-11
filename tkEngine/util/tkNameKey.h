#pragma once

#include "tkEngine/Util/tkUtil.h"

namespace tkEngine {
	
	class NameKey : Noncopyable{
		std::string name;		//!<名前
		unsigned int hashCode;	//!<ハッシュ値。
	public:
		NameKey(){}
		NameKey( const char* name )
		{
			Init(name);
		}
		//初期化。
		void Init(const char* name)
		{
			this->name = name;
			//名前をハッシュ値に変換。
			hashCode = CUtil::MakeHash(name);
		}
		//ハッシュコードを取得。
		unsigned int GetHashCode() const
		{
			return hashCode;
		}
		//名前を取得。
		const char* GetName() const
		{
			return name.c_str();
		}
	};
}
