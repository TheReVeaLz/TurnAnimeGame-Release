#pragma once

#include <d3d11.h>
#include "hook.h"
#include "bypass.h"
#pragma comment(lib, "d3d11.lib")

namespace Process {
	inline DWORD ID;
	inline HANDLE Handle;
	inline HWND Hwnd;
	inline HMODULE Module;
	inline WNDPROC WndProc;
	inline int WindowWidth;
	inline int WindowHeight;
	inline LPCSTR Title;
	inline LPCSTR ClassName;
	inline LPCSTR Path;
}

//typedef HRESULT(APIENTRY* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
//IDXGISwapChainPresent oIDXGISwapChainPresent;
//
//typedef void(APIENTRY* ID3D11DrawIndexed)(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
//ID3D11DrawIndexed oID3D11DrawIndexed;