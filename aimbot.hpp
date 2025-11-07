#pragma once
#include "offsets.hpp"
#include <array>
#include"kmboxb+/kmbox_interface.hpp"
double PI = 3.141592653589793;
#define qabs(x) ((x) < 0 ? -(x) : (x))

inline void km_move(int X, int Y) {
	std::string command = "km.move(" + std::to_string(X) + "," + std::to_string(Y) + ")\r\n";
	send_command(hSerial, command.c_str());
}

inline void km_click() {
	std::string command = "km.left(" + std::to_string(1) + ")\r\n"; // left mouse button down
	Sleep(50); // to stop it from crashing idk
	std::string command1 = "km.left(" + std::to_string(0) + ")\r\n"; // left mouse button up
	send_command(hSerial, command.c_str());
	send_command(hSerial, command1.c_str());
}

inline void km_space() {
	std::string command = "km.left(" + std::to_string(KEY_LEFTCONTROL) + ")\r\n"; // left mouse button down
	Sleep(50); // to stop it from crashing idk
	std::string command1 = "km.left(" + std::to_string(0) + ")\r\n"; // left mouse button up
	send_command(hSerial, command.c_str());
	send_command(hSerial, command1.c_str());
}

/*
void mouseleft()
{
	M_LeftClick(m_hdl, 1);
	Sleep(5);
}
*/
//win10获取按键状态参数

uint64_t gafAsyncKeyStateExport = 0;
uint8_t state_bitmap[64]{ };
uint8_t previous_state_bitmap[256 / 8]{ };
uint64_t win32kbase = 0;

int win_logon_pid = 0;
enum class e_registry_type
{
	none = REG_NONE,
	sz = REG_SZ,
	expand_sz = REG_EXPAND_SZ,
	binary = REG_BINARY,
	dword = REG_DWORD,
	dword_little_endian = REG_DWORD_LITTLE_ENDIAN,
	dword_big_endian = REG_DWORD_BIG_ENDIAN,
	link = REG_LINK,
	multi_sz = REG_MULTI_SZ,
	resource_list = REG_RESOURCE_LIST,
	full_resource_descriptor = REG_FULL_RESOURCE_DESCRIPTOR,
	resource_requirements_list = REG_RESOURCE_REQUIREMENTS_LIST,
	qword = REG_QWORD,
	qword_little_endian = REG_QWORD_LITTLE_ENDIAN
};
inline LPSTR CC_TO_LPSTR(const char* in)
{
	LPSTR out = new char[strlen(in) + 1];
	strcpy_s(out, strlen(in) + 1, in);

	return out;
}
std::string QueryValue(const char* path, e_registry_type type)
{
	if (!globals::hVMM)
		return "";

	BYTE buffer[0x128];
	DWORD _type = (DWORD)type;
	DWORD size = sizeof(buffer);

	if (!VMMDLL_WinReg_QueryValueExU(globals::hVMM, CC_TO_LPSTR(path), &_type, buffer, &size))
	{

		return nullptr;
	}

	std::wstring wstr = std::wstring((wchar_t*)buffer);
	return std::string(wstr.begin(), wstr.end());
}

DWORD csrss_pid;
ULONG64 gafAsyncKeyState_;
//byte keyStateBitmap[50];
std::array<std::uint8_t, 256 * 2 / 8> key_state_bitmap;
std::array<std::uint8_t, 256 / 8> key_state_recent_bitmap;

//windows获取按键状态，win11没法正常获取，解引用两次返回值就变为0
bool getwinlogon()
{
	std::string win = QueryValue("HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\CurrentBuild", e_registry_type::sz);
	int Winver = 0;
	if (!win.empty())
		Winver = std::stoi(win);
	else
		return false;

	win_logon_pid = GetPidFromName("winlogon.exe");
	std::cout << "win_logon_pid: " << win_logon_pid << std::endl;
	std::cout << "Winver: " << Winver << std::endl;
	if (Winver > 22000)
	{
		auto pids = GetPidListFromName("csrss.exe");

		for (size_t i = 0; i < pids.size(); i++)
		{
			std::cout << "csrss: " << pids[i] << std::endl;
			auto pid = pids[i];
			uintptr_t tmp = VMMDLL_ProcessGetModuleBaseU(globals::hVMM, pid, (LPSTR)"win32ksgd.sys");
			std::cout << "tmp: " << tmp << std::endl;
			uintptr_t g_session_global_slots = tmp + 0x3110;
			uintptr_t user_session_state = Read<uintptr_t>(Read<uintptr_t>(Read<uintptr_t>(g_session_global_slots, pid), pid), pid);
			gafAsyncKeyStateExport = user_session_state + 0x3690;
			std::cout << "user_session_state: " << user_session_state << std::endl;

			
			if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
				break;
		}
		if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
		{
			std::cout << "获取按键成功: " << gafAsyncKeyStateExport << std::endl;
			return true;
		}
		return false;
	}
	else
	{
		PVMMDLL_MAP_EAT eat_map = NULL;
		PVMMDLL_MAP_EATENTRY eat_map_entry;
		bool result = VMMDLL_Map_GetEATU(globals::hVMM, GetPidFromName("winlogon.exe") | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, (LPSTR)"win32kbase.sys", &eat_map);
		if (!result)
			return false;

		if (eat_map->dwVersion != VMMDLL_MAP_EAT_VERSION)
		{
			VMMDLL_MemFree(eat_map);
			eat_map_entry = NULL;
			return false;
		}

		for (int i = 0; i < eat_map->cMap; i++)
		{
			eat_map_entry = eat_map->pMap + i;
			if (strcmp(eat_map_entry->uszFunction, "gafAsyncKeyState") == 0)
			{
				gafAsyncKeyStateExport = eat_map_entry->vaFunction;

				break;
			}
		}

		VMMDLL_MemFree(eat_map);
		eat_map = NULL;
		if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
		{
			std::cout << "获取按键成功: " << gafAsyncKeyStateExport << std::endl;
			return true;
		}
		return false;
	}
}

void update_key_state_bitmap() {

	uint8_t previous_key_state_bitmap[64] = { 0 };
	memcpy(previous_key_state_bitmap, state_bitmap, 64);

	VMMDLL_MemReadEx(globals::hVMM, win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, gafAsyncKeyStateExport, (PBYTE)&state_bitmap, 64, NULL, VMMDLL_FLAG_NOCACHE);
	for (int vk = 0; vk < 256; ++vk)
		if ((state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2) && !(previous_key_state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2))
			previous_state_bitmap[vk / 8] |= 1 << vk % 8;
}

bool is_key_down(std::uint8_t const vk) {
	if (gafAsyncKeyStateExport < 0x7FFFFFFFFFFF)
		return false;

	return state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2;
}

bool was_key_pressed(std::uint8_t const vk) {
	bool const result = key_state_recent_bitmap[vk / 8] & 1 << vk % 8;
	key_state_recent_bitmap[vk / 8] &= ~(1 << vk % 8);
	return result;
}


Vector2D ClampAngle(Vector2D angle)
{
	if (angle.y < -89)
	{
		angle.y = -89.0;
	}
	if (angle.y > 89)
	{
		angle.y = 89.0;
	}
	while (angle.x < -180)
	{
		angle.x +=  360;

	}
	while (angle.x > 180)
	{
		angle.x -=  360;

	}
	return angle;
}
Vector2D smoothangle(Vector2D src, Vector2D dst, float factorx, float factory)
{
	Vector2D delta;
	delta.x = dst.x - src.x;
	delta.y = dst.y - src.y;

	delta = ClampAngle(delta);
	dst.x = src.x + delta.x / factorx;
	dst.y = src.y + delta.y / factory;

	return dst;

}

double angle(double rad)
{
	double angle1;   // 声明：角度

	angle1 = 180.0f/M_PI;             //  取出 秒

	return (rad * angle1);
}
float Getfov(float x1, float x2, float y1, float y2)
{
	double dis;
	float deltax = (x1 - x2);
	float deltay = (y1 - y2);
	return dis = sqrt(pow(deltax, 2.0f) + pow(deltay, 2.0f));
}
float ScreenWidth = static_cast<float>(globals::ScreenWidth);
float ScreenHeight = static_cast<float>(globals::ScreenHeight);

void feiaim(uintptr_t player, Vector AimBonepos)
{

	float TargetX, TargetY;
	if (AimBonepos.x != 0) {
		if (AimBonepos.x > ScreenWidth / 2) {
			TargetX = -(ScreenWidth / 2 - AimBonepos.x);
			TargetX = TargetX / aim::factorx;

			if (TargetX + ScreenWidth / 2 + 1 > ScreenWidth) {
				TargetX = 0;
			}

			if (TargetX < 1 && TargetX > 0.3) {
				TargetX = 1;
			}
		}
		else {
			TargetX = AimBonepos.x - ScreenWidth / 2;
			TargetX = TargetX / aim::factorx;

			if (TargetX + ScreenWidth / 2 - 1 < 0) {
				TargetX = 0;
			}

			if (TargetX > -1 && TargetX < 0.3) {
				TargetX = -1;
			}
		}
	}

	if (AimBonepos.y != 0) {
		if (AimBonepos.y >ScreenHeight / 2) {
			TargetY = -(ScreenHeight / 2 - AimBonepos.y);
			TargetY = TargetY / aim::factory;

			if (TargetY + ScreenHeight / 2 + 1 > ScreenHeight) {
				TargetY = 0;
			}

			if (TargetY < 1 && TargetY > 0.3) {
				TargetY = 1;
			}
		}
		else {
			TargetY = AimBonepos.y - ScreenHeight / 2;
			TargetY = TargetY / aim::factory;

			if (TargetY + ScreenHeight / 2 - 1 < 0) {
				TargetY = 0;
			}

			if (TargetY > -1 && TargetY < 0.3) {
				TargetY = -1;
			}
		}
	}
	if (AimBonepos.x != 0 && AimBonepos.y != 0)
	{
			if (player > 0 && aim::screenfov <= aim::fov && aim::aimbot)
			{
				if (globals::km)
				{
					km_move(TargetX, TargetY);
				}

				if (globals::kmnet)
				{
					kmNet_mouse_move_auto(TargetX, TargetY, 8);
				}

			}
		


	}
}


//IMGUI

// Data
	static ID3D11Device* g_pd3dDevice = nullptr;
	static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
	static IDXGISwapChain* g_pSwapChain = nullptr;
	static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
	static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

	// Forward declarations of helper functions
	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


	// Helper functions
	bool CreateDeviceD3D(HWND hWnd)
	{
		// Setup swap chain
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		UINT createDeviceFlags = 0;
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
		HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
		if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
			res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
		if (res != S_OK)
			return false;

		CreateRenderTarget();
		return true;
	}

	void CleanupDeviceD3D()
	{
		CleanupRenderTarget();
		if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
		if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
		if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
	}

	void CreateRenderTarget()
	{
		ID3D11Texture2D* pBackBuffer;
		g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
		pBackBuffer->Release();
	}

	void CleanupRenderTarget()
	{
		if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
	}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

	// Forward declare message handler from imgui_impl_win32.cpp
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Win32 message handler
	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		switch (msg)
		{
		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED)
				return 0;
			g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
			g_ResizeHeight = (UINT)HIWORD(lParam);
			return 0;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
		case WM_DPICHANGED:
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
			{
				//const int dpi = HIWORD(wParam);
				//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
				const RECT* suggested_rect = (RECT*)lParam;
				::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
			}
			break;
		}
		return ::DefWindowProcW(hWnd, msg, wParam, lParam);
	}

	std::string AnisToUTF8(const std::string& Str)
	{
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, Str.c_str(), -1, NULL, 0);

		wchar_t* pwBuf = new wchar_t[(size_t)nwLen + 1];
		ZeroMemory(pwBuf, (size_t)nwLen * 2 + 2);
		::MultiByteToWideChar(CP_ACP, 0, Str.c_str(), Str.length(), pwBuf, nwLen);
		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
		char* pBuf = new char[(size_t)nLen + 1];
		ZeroMemory(pBuf, (size_t)nLen + 1);
		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
		std::string retStr(pBuf);
		delete[]pwBuf;
		delete[]pBuf;
		pwBuf = NULL;
		pBuf = NULL;
		return retStr;
	}

	void drawBox(float x, float y, float w, float h, ImColor color, float t)
	{
		float cx = x + w;
		float cy = y + h;
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(cx, y), color, t);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(cx, y), ImVec2(cx, cy), color, t);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(cx, cy), ImVec2(x, cy), color, t);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, cy), ImVec2(x, y), color, t);
	}

	void drawstr(const char* Text, float x, float y, ImColor color)
	{
		ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), color, Text);
	}

	static void DrawFilledRectImGui(int x, int y, int w, int h, ImVec4 color)
	{
		ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(color), 0, 0);

	}

	void Text(const char* Text, int x, int y, int color)
	{

		std::string UTF8 = AnisToUTF8(std::string(Text));

		ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), color, UTF8.c_str());
	}

	void Rectangle(int x, int y, int width, int height, int color, int thickness)
	{
		ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + width, y + height), color, 0, 15, thickness);
	}

	void Line(Vector2D From, Vector2D To, int color, int thickness)
	{
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(From.x, From.y), ImVec2(To.x, To.y), color, thickness);
	}
	void Helpmarker(const char* Text, ImVec4 Color)
	{
		ImGui::TextColored(Color, "(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(Text);
		}
	}
	void DrawCircle(ImVec2 center, float radius, ImColor color) {
		ImGui::GetForegroundDrawList()->AddCircle(center, radius, color);
	}
	bool IsCenterInView(ImVec2 center, float radius)
	{
		ImVec2 screen_center = ImVec2(globals::ScreenWidth / 2, globals::ScreenHeight / 2);
		float distance = std::sqrt(std::pow(center.x - screen_center.x, 2) + std::pow(center.y - screen_center.y, 2));

		return distance <= radius;
	}
	void DrawCrosshair(ImColor color)
	{
		ImDrawList* draw_list = ImGui::GetForegroundDrawList();
		draw_list->AddLine(ImVec2(globals::ScreenWidth / 2 - 10, globals::ScreenHeight / 2), ImVec2(globals::ScreenWidth / 2 + 10, globals::ScreenHeight / 2), color, 1.0f);
		draw_list->AddLine(ImVec2(globals::ScreenWidth / 2, globals::ScreenHeight / 2 - 10), ImVec2(globals::ScreenWidth / 2, globals::ScreenHeight / 2 + 10), color, 1.0f);
	}
	void DrawEntityInfo(const char* info, float x, float y)
	{
		ImDrawList* draw_list = ImGui::GetForegroundDrawList();

		ImFont* font = ImGui::GetFont();

		// 计算文本的长度
		ImVec2 text_size = font->CalcTextSizeA(font->FontSize, FLT_MAX, 0.0f, info);

		// 将文本的中心点设置为 (x, y)
		float text_x = x - text_size.x / 2.0f;
		float text_y = y - text_size.y / 2.0f;

		//Text(info, p1.x, p1.y- box_offset, ImColor(0, 0, 0, 255));
		//ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(p1.x, p1.y), ImVec2(p1.x + box_width, p1.y - box_offset), ImColor(r, g, b ,80), 0, 0);
		Text(info, text_x, text_y - 20, ImColor(255, 255, 255, 255));


	}