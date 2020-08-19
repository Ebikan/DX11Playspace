#pragma once
/******************************************************************************

	file:	VertexBuffer.h

	brief:	none

	date:	8/18/2020 9:42:27 PM

	author:	Evie Brown

	email:	evie@eviebrown.com

******************************************************************************/

#include "Bindable.h"

class VertexBuffer : public Bindable {
public:
	template<class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices) : stride(sizeof(V))
	{
		//start info manager
		// create buffer data
		D3D11_BUFFER_DESC const desc = {
			UINT(sizeof(V) * vertices.size()),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_VERTEX_BUFFER,
			0u,
			0u,
			sizeof(V)
		};


		D3D11_SUBRESOURCE_DATA subData = {
			&vertices,0u,0u
		};



		GetDevice()->CreateBuffer(&desc, &subData,);
		// throw info if err
	}

	void Bind(Graphics& gfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	 size_t stride;
};

