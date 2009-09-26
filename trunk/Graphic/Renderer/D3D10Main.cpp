#include "D3D10Renderer.h"

namespace Flagship
{
	HINSTANCE g_hInstance;
}

BOOLEAN WINAPI    DllMain( HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved )
{
	Flagship::g_hInstance = hDllHandle;
	return true;
}
