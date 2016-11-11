#pragma once

#include "tkEngine/Util/tkUtil.h"

namespace tkEngine {
	
	class NameKey : Noncopyable{
		std::string name;		//!<���O
		unsigned int hashCode;	//!<�n�b�V���l�B
	public:
		NameKey(){}
		NameKey( const char* name )
		{
			Init(name);
		}
		//�������B
		void Init(const char* name)
		{
			this->name = name;
			//���O���n�b�V���l�ɕϊ��B
			hashCode = CUtil::MakeHash(name);
		}
		//�n�b�V���R�[�h���擾�B
		unsigned int GetHashCode() const
		{
			return hashCode;
		}
		//���O���擾�B
		const char* GetName() const
		{
			return name.c_str();
		}
	};
}
