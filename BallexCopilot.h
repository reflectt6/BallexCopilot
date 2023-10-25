#pragma once
// imgui
#include "imgui-docking/imconfig.h"
#include "imgui-docking/imgui.h"
#include "imgui-docking/imgui_impl_win32.h"
#include "imgui-docking/imgui_internal.h"
#include "imgui-docking/imstb_rectpack.h"
#include "imgui-docking/imstb_textedit.h"
#include "imgui-docking/imstb_truetype.h"
// dx12
#include "imgui-docking/imgui_impl_dx12.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <tchar.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
// dx11
//#include "imgui/imgui_impl_dx11.h"
//#include <d3d11.h>
//#pragma comment(lib, "d3d11.lib")
// hack code
#include "hack/hack.h"