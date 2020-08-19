#include "stdafx.h"
/******************************************************************************

	file:	Bindable.cpp

	brief:	none

	date:	8/15/2020 9:40:29 AM

	author:	Evie Brown

	email:	evie@eviebrown.com

******************************************************************************/

#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(const Graphics& gfx)  noexcept
{
	return gfx.pContext.Get();
}

ID3D11Device* Bindable::GetDevice(const Graphics& gfx)  noexcept
{
	return gfx.pDevice.Get();
}

DebugLayerInfo& Bindable::GetInfoManager(Graphics& gfx)  noexcept(!_DEBUG)
{
	return gfx.infoManager;
}
