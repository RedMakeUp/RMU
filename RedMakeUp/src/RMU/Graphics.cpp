#include "stdafx.h"
#include "Graphics.h"

namespace RMU {
	void Graphics::EnableDebugLayer() const
	{
#ifdef RMU_DEBUG
		ComPtr<ID3D12Debug> d;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&d)));
		d->EnableDebugLayer();
		RMU_LOG_TRACE("D3D debug layer is on");
#else
		RMU_LOG_TRACE("D3D debug layer is off");
#endif // RMU_DEBUG
	}
}
