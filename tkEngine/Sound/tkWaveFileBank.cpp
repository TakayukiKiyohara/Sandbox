/*!
 *@brief	波形データバンク。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/Sound/tkWaveFile.h"
#include "tkEngine/Sound/tkWaveFileBank.h"

namespace tkEngine{
	CWaveFileBank::CWaveFileBank()
	{
	}
	CWaveFileBank::~CWaveFileBank()
	{
		ReleaseAll();
	}
	void CWaveFileBank::Release(int groupID)
	{
		for (auto waveFile : m_waveFileMap[groupID]) {
			waveFile.second->Release();
		}
		m_waveFileMap[groupID].clear();
	}
	void CWaveFileBank::RegistWaveFile(int groupID, CWaveFilePtr waveFile)
	{
		TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
		m_waveFileMap[groupID].insert({ waveFile->GetFilePathHash(), waveFile });
	}
	CWaveFilePtr CWaveFileBank::FindWaveFile(int groupID, const char* filePath)
	{
		TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
		auto value = m_waveFileMap[groupID].find(CUtil::MakeHash(filePath));
		if (value != m_waveFileMap[groupID].end()) {
			return value->second;
		}
		return CWaveFilePtr();
	}
	CWaveFilePtr CWaveFileBank::FindWaveFile(int groupID, const NameKey& nameKey)
	{
		TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
		auto value = m_waveFileMap[groupID].find(nameKey.GetHashCode());
		if (value != m_waveFileMap[groupID].end()) {
			return value->second;
		}
		return CWaveFilePtr();
	}
}
