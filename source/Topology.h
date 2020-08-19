#pragma once
/******************************************************************************

	file:	Topology.h

	brief:	none

	date:	8/18/2020 7:23:57 PM

	author:	Evie Brown

	email:	evie@eviebrown.com

******************************************************************************/
#include "Bindable.h"

class Topology : public Bindable {

public:
	Topology(Graphics& gfx,  D3D11_PRIMITIVE_TOPOLOGY topo) noexcept;
	void Bind(Graphics& gfx) noexcept override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY topo;
};