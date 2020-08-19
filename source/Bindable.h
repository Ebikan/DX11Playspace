#pragma once
/******************************************************************************

	file:	Bindable.h

	brief:	none

	date:	8/15/2020 9:40:29 AM

	author:	Evie Brown

	email:	evie@eviebrown.com

******************************************************************************/
#include "Graphics.h"


class Bindable {
public:
	virtual void Bind(Graphics& gfx) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	static ID3D11DeviceContext* GetContext(const Graphics& gfx)  noexcept;
	static ID3D11Device* GetDevice(const Graphics& gfx)  noexcept;
	static DebugLayerInfo& GetInfoManager(Graphics& gfx)  noexcept(!_DEBUG);


};