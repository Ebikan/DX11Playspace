#include "stdafx.h"
/******************************************************************************

	file:	Topology.cpp

	brief:	none

	date:	8/18/2020 7:23:57 PM

	author:	Evie Brown

	email:	evie@eviebrown.com

******************************************************************************/



#include "Topology.h"

Topology::Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY topo) noexcept : topo(topo)
{
	UNREFERENCED_PARAMETER(gfx);
}

void Topology::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetPrimitiveTopology(topo);
}
