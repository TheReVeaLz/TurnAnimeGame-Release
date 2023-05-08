#include "DirectX11.h"
#include "dllmain.h"
#include "cheats.h"
#include <cstdio>

#include "../external/ImGui/imgui.h"
#include "../external/ImGui/imgui_impl_dx11.h"
#include "../external/ImGui/imgui_impl_win32.h"

typedef HRESULT(APIENTRY* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
IDXGISwapChainPresent oIDXGISwapChainPresent;

typedef void(APIENTRY* ID3D11DrawIndexed)(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
ID3D11DrawIndexed oID3D11DrawIndexed;

bool ShowMenu = true;
bool ImGui_Initialised = false;

namespace DirectX11Interface {
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;
	ID3D11RenderTargetView* RenderTargetView;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_KEYUP) {
		switch (wParam) {
		case VK_F1:
			ShowMenu = !ShowMenu;
			break;
		case VK_F2:
			cheat::bSpeedhack = !cheat::bSpeedhack;
			if (cheat::bSpeedhack) CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)cheat::SpeedHackTick, 0, 0, 0);
			break;
		default:
			break;
		}
	}

	if (ShowMenu) {
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		return true;
	}
	return CallWindowProc(Process::WndProc, hwnd, uMsg, wParam, lParam);
}

HRESULT APIENTRY MJPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (!ImGui_Initialised) {
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&DirectX11Interface::Device))) {
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			DirectX11Interface::Device->GetImmediateContext(&DirectX11Interface::DeviceContext);

			DXGI_SWAP_CHAIN_DESC Desc;
			pSwapChain->GetDesc(&Desc);
			WindowHwnd = Desc.OutputWindow;

			ID3D11Texture2D* BackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
			DirectX11Interface::Device->CreateRenderTargetView(BackBuffer, NULL, &DirectX11Interface::RenderTargetView);
			BackBuffer->Release();

			io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\comic.ttf", 15.f);

			ImGui_ImplWin32_Init(WindowHwnd);
			ImGui_ImplDX11_Init(DirectX11Interface::Device, DirectX11Interface::DeviceContext);
			ImGui_ImplDX11_CreateDeviceObjects();
			ImGui::GetIO().ImeWindowHandle = Process::Hwnd;
			Process::WndProc = (WNDPROC)SetWindowLongPtr(Process::Hwnd, GWLP_WNDPROC, (__int3264)(LONG_PTR)WndProc);
			ImGui_Initialised = true;
		}
	}
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::GetIO().MouseDrawCursor = ShowMenu;
	if (ShowMenu == true) {
		const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f - (275*.5f), ImGui::GetIO().DisplaySize.y * 0.5f - (325*.5f)), ImGuiCond_Once);
		ImGui::SetNextWindowSize({ 275, 300 }, ImGuiCond_Once);
		ImGui::Begin(
			"Raimu-SR",
			&ShowMenu,
			0
		);
		ImGui::SetWindowFontScale(1.5f);
		ImGui::BeginTabBar("##tabs");
		if (ImGui::BeginTabItem("World")) {
			ImGui::Checkbox("Auto Battle", &cheat::bAutoBattle);
			if (ImGui::Checkbox("Auto Talk", &cheat::bAutoTalk)) CreateThread(0, 0, (LPTHREAD_START_ROUTINE)cheat::AutoTalkTick, 0, 0, 0);
			ImGui::Checkbox("Fast Dialog", &cheat::bFastDialogue);
			ImGui::SameLine();
			ImGui::PushItemWidth(75.f);
			ImGui::InputFloat("##FastDialog", &cheat::fFastDialogue, 0.0f, 0.0f, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::PopItemWidth();

			if (ImGui::Checkbox("Blind Enemies", &cheat::bBlind)) cheat::CheatThread();

			if (ImGui::Checkbox("Speedhack", &cheat::bSpeedhack)) CreateThread(0, 0, (LPTHREAD_START_ROUTINE)cheat::SpeedHackTick, 0, 0, 0);
			ImGui::SameLine();
			ImGui::PushItemWidth(75.f);
			if (ImGui::InputFloat("##Speed", &cheat::fSpeedhack, 0.0f, 0.0f, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))  CreateThread(0, 0, (LPTHREAD_START_ROUTINE)cheat::SpeedHackTick, 0, 0, 0);
			ImGui::PopItemWidth();

			if (ImGui::Checkbox("Magnet", &cheat::bMagnet)) cheat::Magnet();
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) && ImGui::BeginTooltip())
			{
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("==========[WARNING]==========\nMight stuck your account\nTest on dummy account.");
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Visual"))
		{
			if (ImGui::Checkbox("FPS Unlock", &cheat::bFPSUnlock)) cheat::CheatThread();
			ImGui::SameLine();
			ImGui::PushItemWidth(75.f);
			if (ImGui::InputInt("##FPS", &cheat::iFPSUnlock, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue)) cheat::CheatThread();
			ImGui::PopItemWidth();


			if (ImGui::Checkbox("Enable Peeking", &cheat::bPeeking)) cheat::CheatThread();

			ImGui::Checkbox("UID Changer", &cheat::bUIDChanger);
			ImGui::SameLine();
			ImGui::PushItemWidth(75.f);
			ImGui::InputInt("##UID", &cheat::iUID, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::PopItemWidth();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Debug"))
		{

			ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("About"))
		{
			ImGui::Text("Credits: ");
			if (ImGui::Button("TheReVeaLz")) { ShellExecute(NULL, "open", "https://github.com/TheReVeaLz", 0, 0, SW_SHOWDEFAULT); }
			ImGui::SameLine();
			if (ImGui::Button("Z4ee")) { ShellExecute(NULL, "open", "https://github.com/Z4ee/StarRail-S-GC", 0, 0, SW_SHOWDEFAULT); }

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
		ImGui::End();
	}
	ImGui::EndFrame();
	ImGui::Render();
	DirectX11Interface::DeviceContext->OMSetRenderTargets(1, &DirectX11Interface::RenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oIDXGISwapChainPresent(pSwapChain, SyncInterval, Flags);
}

void APIENTRY MJDrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation) {

}

void OpenConsole()
{
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);

	auto consoleWindow = GetConsoleWindow();
	SetForegroundWindow(consoleWindow);
	ShowWindow(consoleWindow, SW_RESTORE);
	ShowWindow(consoleWindow, SW_SHOW);
}

DWORD WINAPI MainThread(LPVOID lpParameter) {
	bool WindowFocus = false;
	OpenConsole();
	hooks::init();
	
	while ((cheat::qwGameAssembly = (uint64_t)GetModuleHandle("GameAssembly.dll")) == 0) {
		printf("[Info] GameAssembly isn't initialized...\n");
		Sleep(2000);
	}
	while ((cheat::qwUnityPlayer = (uint64_t)GetModuleHandle("UnityPlayer.dll")) == 0) Sleep(2000);

	Sleep(10000);
	printf("[Info] Intialize ImGui\n");
	while (WindowFocus == false) {
		if (FindWindowA("UnityWndClass", nullptr)) {
			Process::ID = GetCurrentProcessId();
			Process::Handle = GetCurrentProcess();
			Process::Hwnd = FindWindowA("UnityWndClass", nullptr);

			RECT TempRect;
			GetWindowRect(Process::Hwnd, &TempRect);
			Process::WindowWidth = TempRect.right - TempRect.left;
			Process::WindowHeight = TempRect.bottom - TempRect.top;

			char TempTitle[MAX_PATH];
			GetWindowText(Process::Hwnd, TempTitle, sizeof(TempTitle));
			Process::Title = TempTitle;

			char TempClassName[MAX_PATH];
			GetClassName(Process::Hwnd, TempClassName, sizeof(TempClassName));
			Process::ClassName = TempClassName;

			char TempPath[MAX_PATH];
			GetModuleFileNameEx(Process::Handle, NULL, TempPath, sizeof(TempPath));
			Process::Path = TempPath;

			WindowFocus = true;
			continue;
		}
		Sleep(200);
	}
	bool InitHook = false;
	while (InitHook == false) {
		printf("[Info] Creating ImGui Hook\n");
		if (DirectX11::Init() == true) {
			printf("[Info] Hook successfully\n");
			CreateHook(8, (void**)&oIDXGISwapChainPresent, MJPresent);
			CreateHook(12, (void**)&oID3D11DrawIndexed, MJDrawIndexed);
			InitHook = true;
			continue;
		}
		Sleep(200);
	}
	ExitThread(0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		Process::Module = hModule;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, new HMODULE(hModule), 0, NULL);
		break;
	case DLL_PROCESS_DETACH:
		FreeLibraryAndExitThread(hModule, TRUE);
		DisableAll();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		break;
	}
	return TRUE;
}