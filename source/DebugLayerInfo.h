#pragma once
/******************************************************************************

	file:	DebugLayerInfo.h

	brief:	none

	date:	6/29/2020 2:43:51 PM

	author:	Evie Brown

	email:	evie@eviebrown.com

******************************************************************************/

#include <vector>
#include <string>

class DebugLayerInfo {

public:

	DebugLayerInfo();
	~DebugLayerInfo() noexcept;
	DebugLayerInfo(const DebugLayerInfo&) = delete;
	DebugLayerInfo& operator=(const DebugLayerInfo&) = delete;
	DebugLayerInfo(DebugLayerInfo&&) = delete;
	DebugLayerInfo& operator=(DebugLayerInfo&&) = delete;
	void Set() noexcept;
	std::vector<std::string> GetMessages() const;
private:
	unsigned long long next = 0u;
	struct IDXGIInfoQueue* pInfoQueue = nullptr;

};
