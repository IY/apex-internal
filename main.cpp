#include "global.h"

ID3D11Device *g_pdevice = nullptr;
ID3D11DeviceContext *g_pcontext = nullptr;
ID3D11RenderTargetView *g_prendertargetview = nullptr;
WNDPROC o_wndproc = nullptr;

std::once_flag present;
std::once_flag wndproc;
bool b_showmenu = false;
bool b_box = false;
bool b_chams = false;


inline bool w2s(c_vec source, c_vec &destination) {
	return o_worldtoscreen(source, destination);
}

inline int get_entcount() {
	return *(int*)(dwbase + 0xC006788);
}

inline c_entity* get_player(uintptr_t idx) {
	return *(c_entity**)(dwbase + 0x1F96D88 + (idx << 5));
}

inline c_entity* get_localentity()
{
	uintptr_t local_entity_id = *(uintptr_t*)(dwbase + 0x1747EFC);
	for (int i = 0; i < get_entcount(); i++)
	{
		c_entity* ent = get_player(i);
		if (!ent) continue;

		if (!strcmp(ent->m_hHandle(), xorstr_("player"))) {
			if (ent->m_iId() == local_entity_id) {
				return ent;
			}
		}
	}
	return nullptr;
}

inline bool friendly(c_entity* e) {
	if (e->m_iTeam() == get_localentity()->m_iTeam())
		return true;
	return false;
}

void render_rect(const ImVec2& from, const ImVec2& to, float rounding, uint32_t roundingCornersFlags, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->AddRect(from, to, ImGui::GetColorU32({ 10 / 255.0f, 60 / 255.0f, 50 / 255.0f, 1.f }), rounding, roundingCornersFlags, thickness);
}

LRESULT APIENTRY hk_wndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	std::call_once(wndproc, [&] {std::cout << xorstr_("hooked wndproc") << std::endl; });


	if (uMsg == WM_KEYDOWN) {
		if (wParam == VK_INSERT)
			b_showmenu = !b_showmenu;
	}

	if (b_showmenu) {
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
	}

	return CallWindowProc(o_wndproc, hwnd, uMsg, wParam, lParam);
}

long __stdcall hk_present(IDXGISwapChain* p_swapchain, unsigned int syncintreval, unsigned int flags) {

	std::call_once(present, [&] {
		std::cout << xorstr_("hooked directx 11 present") << std::endl;

		p_swapchain->GetDevice(__uuidof(g_pdevice), reinterpret_cast<void**>(&g_pdevice));
		g_pdevice->GetImmediateContext(&g_pcontext);
		ID3D11Texture2D *pBackBuffer;
		p_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

		
		g_pdevice->CreateRenderTargetView(pBackBuffer, NULL, &g_prendertargetview);

		//shit way of doing it
		o_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(iat(FindWindowA).get()(0, xorstr_("Apex Legends")), GWLP_WNDPROC, reinterpret_cast<uintptr_t>(hk_wndproc)));

		ImGui_ImplDX11_Init(iat(FindWindowA).get()(0, xorstr_("Apex Legends")), g_pdevice, g_pcontext);
		ImGui_ImplDX11_CreateDeviceObjects();
	});


	g_pcontext->OMSetRenderTargets(1, &g_prendertargetview, NULL);


	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplDX11_NewFrame();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin(xorstr_("##esp"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
	for (int i = 0; i < get_entcount(); i++) {
		auto e = get_player(i);
		if (e && e->m_iHealth() > 0) {
			if(b_chams)
				e->hl_make_glow();
			
			if (b_box) {
				c_vec pos, pos3D, top, top3D;
				pos3D = e->m_vPos();
				top3D = pos3D + c_vec(0, 0, 64);

				if (w2s(pos3D, pos) && w2s(top3D, top))
				{
					int height = (pos.y - top.y);
					int width = height / 2;
					render_rect(ImVec2((pos.x - width / 2), top.y), ImVec2((pos.x - width / 2) + width, top.y + height), 5, 0, 3);
				}
				
			}
		}
	}
	
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->PushClipRectFullScreen();

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);

	if (b_showmenu) {

		if (ImGui::Begin(xorstr_("apexlegends internal"), 0, ImVec2(200, 200))) {
			ImGui::Checkbox(xorstr_("glow chams"), &b_chams);
			ImGui::Checkbox(xorstr_("box"), &b_box);
			ImGui::End();
		}
	}
	ImGui::Render();
	return o_present(p_swapchain, syncintreval, flags);
}



void __stdcall _thr() {
	AllocConsole();
	freopen(xorstr_("con"), xorstr_("w"), stdout);

	std::cout << xorstr_("use at ownd risk") << std::endl;

	while (!dwbase || !dwdiscord) {
		dwbase = (uintptr_t)iat(GetModuleHandleA).get()(xorstr_("r5apex.exe"));
		dwdiscord = (uintptr_t)iat(GetModuleHandleA).get()(xorstr_("DiscordHook64.dll"));
	}

	std::cout << xorstr_("r5apex.exe ") << std::hex << dwdiscord << std::endl;
	std::cout << xorstr_("discordhook64.dll ") << std::hex << dwdiscord << std::endl;
	
	std::cout << xorstr_("loading discord hook methods") << std::endl;
	uintptr_t dwpresent = memory::occurence(xorstr_("DiscordHook64.dll"), xorstr_("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 8B F2 48 8B D9 48 8B D1"));
	o_getasynckeystate = (getasynckeystate_fn)memory::occurence(xorstr_("DiscordHook64.dll"), xorstr_("40 53 48 83 EC 20 8B D9 FF 15 ? ? ? ?"));
	CreateHook = (createhook_fn)(memory::occurence(xorstr_("DiscordHook64.dll"), xorstr_("40 53 55 56 57 41 54 41 56 41 57 48 83 EC 60")));
	EnableHook = (enablehook_fn)(memory::occurence(xorstr_("DiscordHook64.dll"), xorstr_("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 20 33 F6 8B FA")));
	EnableHookQue = (applyqueued_fn)(memory::occurence(xorstr_("DiscordHook64.dll"), xorstr_("48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 57")));

	std::cout << xorstr_("directx 11 present ") << std::hex << dwpresent << std::endl;
	std::cout << xorstr_("getaynckeystate ") << std::hex << o_getasynckeystate << std::endl;
	std::cout << xorstr_("create hook ") << std::hex << CreateHook << std::endl;
	std::cout << xorstr_("push hook") << std::hex << EnableHook << std::endl;
	std::cout << xorstr_("enable hook") << std::hex << EnableHookQue << std::endl;
	
	std::cout << xorstr_("loading game classes") << std::endl;
	
	g_pinput = (c_input*)memory::dereference(memory::occurence(xorstr_("r5apex.exe"), xorstr_("48 8D 0D ? ? ? ? 41 FF 90 ? ? ? ? EB E9")), 3);
	g_pglobals = (c_globalvars*)memory::dereference(memory::occurence(xorstr_("r5apex.exe"), xorstr_("48 8B 05 ? ? ? ? F3 0F 10 50 ? 74 38")), 4);
	createinterface = (createinterface_fn)memory::dereference(memory::occurence(xorstr_("r5apex.exe"), xorstr_("E8 ? ? ? ? 48 89 05 ? ? ? ? 48 83 3D ? ? ? ? ? 0F 84 ? ? ? ? 33 D2")), 1);
	o_worldtoscreen = (worldtoscreen_fn)(dwbase + 0x642B10);

	std::cout << xorstr_("g_pinput ") << std::hex << g_pinput << std::endl;
	std::cout << xorstr_("g_pglobals ") << std::hex << g_pglobals << std::endl;
	std::cout << xorstr_("createinterface ") << std::hex << createinterface << std::endl;
	std::cout << xorstr_("o_worldtoscreen ") << std::hex << o_worldtoscreen << std::endl;

	std::cout << xorstr_("enable hooks") << std::endl;

	CreateHook((void*)dwpresent, (void*)hk_present, (void**)&o_present);
	EnableHook((void*)dwpresent, 1);
	EnableHookQue();
}

bool __stdcall DllMain(void* module, unsigned long reason, void* buffer) {//inject with hack thread option
	if (reason == 1) {
		_thr();
	}
	return true;
}

