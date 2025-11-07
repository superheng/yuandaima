
#pragma once
#include <windows.h>
#include "pcileech/vmmdll.h"            // include PCILeech memory manipulation library header
#include <string>
#include "struct.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#pragma comment(lib, "pcileech/vmm")    // include PCILeech memory manipulation library
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_win32.h"
#include "IMGUI/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include "struct.h"
#pragma comment(lib,"d3d11.lib")
#include"curl/include/curl/curl.h"
#include "encode.h"
#include"json/include/json.h"
#include"IMGUI/Font.h"
#include "msdk/msdk.h"
#include"NetConfig/kmboxNet.h"
#include"NetConfig/HidTable.h"
#include"d3dx9.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "libcurl_a.lib")

//HANDLE m_hdl = M_Open(1);

namespace globals
{
    // application
    int	                    INIT_CHOICE = NULL;
    VMM_HANDLE              hVMM = NULL;
    VMMDLL_SCATTER_HANDLE   hS = NULL;
    VMMDLL_SCATTER_HANDLE   oS = NULL;
    bool                    INIT_SUCCESS = NULL;
    bool		            CHEAT_RUNNING = true;
    bool		            IN_GAME = NULL;

    // board data
    ULONG64		            qwID = NULL;
    ULONG64		            qwVersionMajor = NULL;
    ULONG64		            qwVersionMinor = NULL;
    std::string             fpgaName = "";

    // process data
    DWORD		            dwPID = NULL;
    QWORD		            qProcessBase = NULL;
    QWORD		            qProcessBase2 = NULL;
    bool                    自动分辨率 = true;
    DWORD		            ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD		            ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    bool km = false;
    bool kmnet = false;
    bool autokmnet = false;
    bool autokm = false;
    int 波特率=115200;
    byte 观察[64] = { NULL };
    byte 投影[64] = { NULL };
    byte 视口[64] = { NULL };
    uintptr_t shuzu;
    D3DXVECTOR3 敌人;
    D3DXVECTOR3 镜像;
    byte 世界[64] = { 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63 };
    int 红名 = 0;
    float fov = 0.0f;
    Matrix						matrix;

    
    bool ProcessStatus = false;
    bool aimkey = false;
    bool triggerkey = false;
    bool straimkey = false;
    bool step = false; //MapRadar open or close
    bool superGlideKey;
    bool keyinit;

    int aimbotkey = 1;

    bool inireload = false;
    uintptr_t localplayer = NULL;
    struct EntityInfo
    {
        uintptr_t xx = 0;
        int count = 0;
        uintptr_t pEntity = 0;
        int entityHealth = 0;
        int bleedout = 0;
        int entityTeam = 0;
        Vector entHead = {};
        Vector entityPos = {};
        Vector boxhead = {};
    };
    std::vector<EntityInfo*> entityInfoVector;
    ImVec4 rectangleColor = ImVec4(1.0, 0.647, 0.0 ,1.0);
}


namespace aim
{
    bool 生化模式 = false;
    int count;
    uintptr_t xx;
    int aimbone = 5;
    Vector aimpos;
    bool 显示名字 = false;
    bool 显示头部 = false;
    bool 红名扳机 = false;
    bool 红名判断 = false;
    bool 范围判断 = false;
    bool 鬼跳判断 = false;
    bool 红名吸附 = false;
    bool 碎步判断 = false;
    bool 锁头判断 = false;
    bool net判断 = false;
    bool pro判断 = false;
    bool USP判断 = false;
    static char netip[14] = "192.168.2.188"; // must replace with your devices ip.
    static char netport[5] = "xxxx"; // must replace with your devices port.
    static char netuuid[9] = "xxxxxxxx"; // must replace with your devices uuid.
    bool 融合器 = false;
    bool crosshair = false;
    int triggerbotkey = 0;
    int aimkey = 0;
    int guikey = 5;
    int suikey = 6;
    int USPkey = 32;
    float guitime = 30;
    float suitime = 20;
    float bantime = 15;
    float USPtime = 100;
    bool triggerbot = false;
    uintptr_t addr = NULL;
    bool lock = false;
    int  fov = 60;
    int  screenfov = 0;
    int  bone = 5;
    bool vis = false;
    float factorx = 5.0f;
    float factory = 5.0f;
    bool boxesp = false;
    bool hp = false;
    bool aimbot = false;
    bool skeletonesp = false;
    ImColor color;
}

bool LoadConfig(const std::string& filename)
{
    ImGuiContext* ctx = ImGui::GetCurrentContext();

    std::ifstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '='))
        {
            std::string value;
            if (std::getline(iss, value))
            {
                if (key == "fov")
                {
                    aim::fov = std::stoi(value);
                }
                else if (key == "自瞄热键")
                {
                    aim::aimkey = std::stoi(value);
                }
                else if (key == "USP热键")
                {
                    aim::USPkey = std::stoi(value);
                }
                else if (key == "扳机热键")
                {
                    aim::triggerbotkey = std::stoi(value);
                }
                else if (key == "netip")
                {
                    strcpy(aim::netip, value.c_str());
                }
                else if (key == "netuuid")
                {
                    strcpy(aim::netuuid, value.c_str());
                }
                else if (key == "netport")
                {
                    strcpy(aim::netport, value.c_str());
                }
                else if (key == "融合器")
                {
                    aim::融合器 = (value == "true");
                }
                else if (key == "screenfov")
                {
                    aim::screenfov = std::stoi(value);
                }
                else if (key == "自瞄部位")
                {
                    aim::aimbone = std::stoi(value);
                }
                else if (key == "宽")
                {
                    globals::ScreenWidth = std::stoi(value);
                }
                else if (key == "高")
                {
                    globals::ScreenHeight = std::stoi(value);
                }
                else if (key == "自动分辨率")
                {
                    globals::自动分辨率 = (value == "true");
                }
                else if (key == "波特率")
                {
                    globals::波特率 = std::stoi(value);
                }
                else if (key == "bone")
                {
                    aim::bone = std::stoi(value);
                }
                else if (key == "aimbone")
                {
                    aim::aimbone = std::stoi(value);
                }
                else if (key == "vis")
                {
                    aim::vis = (value == "true");
                }
                else if (key == "factorx")
                {
                    aim::factorx = std::stof(value);
                }
                else if (key == "USPtime")
                {
                    aim::USPtime = std::stof(value);
                }
                else if (key == "factory")
                {
                    aim::factory = std::stof(value);
                }
                else if (key == "boxesp")
                {
                    aim::boxesp = (value == "true");
                }
                else if (key == "hp")
                {
                    aim::hp = (value == "true");
                }
                else if (key == "triggerbot")
                {
                    aim::triggerbot = (value == "true");
                }
                else if (key == "aimbot")
                {
                    aim::aimbot = (value == "true");
                }
                else if (key == "显示名字")
                {
                    aim::显示名字 = (value == "true");
                }
                else if (key == "USP判断")
                {
                    aim::USP判断 = (value == "true");
                }
                else if (key == "显示头部")
                {
                    aim::显示头部 = (value == "true");
                }
                else if (key == "红名吸附")
                {
                    aim::红名吸附 = (value == "true");
                }
                else if (key == "显示范围")
                {
                    aim::范围判断 = (value == "true");
                }
                else if (key == "Shift锁头")
                {
                    aim::锁头判断 = (value == "true");
                }
                else if (key == "显示准星")
                {
                    aim::crosshair = (value == "true");
                }
                else if (key == "skeletonesp")
                {
                    aim::skeletonesp = (value == "true");
                }
                else if (key == "鬼跳热键")
                {
                    aim::guikey = std::stoi(value);
                }
                else if (key == "碎步热键")
                {
                    aim::suikey = std::stoi(value);
                }
                else if (key == "鬼跳延时")
                {
                    aim::guitime = std::stoi(value);
                }
                else if (key == "碎步延时")
                {
                    aim::suitime = std::stoi(value);
                }
                else if (key == "扳机延时")
                {
                    aim::bantime = std::stoi(value);
                }
                else if (key == "autokmnet")
                {
                    globals::autokmnet = (value == "true");
                    }
                else if (key == "autokm")
                {
                    globals::autokm = (value == "true");
                    }
                else
                {
                    std::cout << "Unknown key: " << key << std::endl;
                }
            }
        }
    }

    file.close();

    return true;
}

bool SaveConfig(const std::string& filename)
{
    ImGuiContext* ctx = ImGui::GetCurrentContext();

    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Failed to open config file for writing: " << filename << std::endl;
        return false;
    }
    file << "autokmnet=" << (globals::autokmnet ? "true" : "false") << std::endl;
    file << "autokm=" << (globals::autokm ? "true" : "false") << std::endl;
    file << "融合器=" << (aim::融合器 ? "true" : "false") << std::endl;
    file << "netip=" << aim::netip << std::endl;
    file << "netuuid=" << aim::netuuid << std::endl;
    file << "netport=" << aim::netport << std::endl;
    file << "自瞄热键=" << aim::aimkey << std::endl;
    file << "USP热键=" << aim::USPkey << std::endl;
    file << "扳机热键=" << aim::triggerbotkey << std::endl;

    file << "自动分辨率=" << (globals::自动分辨率 ? "true" : "false") << std::endl;
    file << "宽=" << globals::ScreenWidth << std::endl;
    file << "高=" << globals::ScreenHeight << std::endl;
    file << "fov=" << aim::fov << std::endl;
    file << "screenfov=" << aim::screenfov << std::endl;
    file << "自瞄部位=" << aim::aimbone << std::endl;
    file << "bone=" << aim::bone << std::endl;
    file << "aimbone=" << aim::aimbone << std::endl;
    file << "vis=" << (aim::vis ? "true" : "false") << std::endl;
    file << "factorx=" << aim::factorx << std::endl;
    file << "factory=" << aim::factory << std::endl;
    file << "boxesp=" << (aim::boxesp ? "true" : "false") << std::endl;
    file << "hp=" << (aim::hp ? "true" : "false") << std::endl;
    file << "triggerbot=" << (aim::triggerbot ? "true" : "false") << std::endl;
    file << "aimbot=" << (aim::aimbot ? "true" : "false") << std::endl;
    file << "显示名字=" << (aim::显示名字 ? "true" : "false") << std::endl;
    file << "USP判断=" << (aim::USP判断 ? "true" : "false") << std::endl;
    file << "显示准星=" << (aim::crosshair ? "true" : "false") << std::endl;
    file << "红名吸附=" << (aim::红名吸附 ? "true" : "false") << std::endl;
    file << "显示范围=" << (aim::范围判断 ? "true" : "false") << std::endl;
    file << "Shift锁头=" << (aim::锁头判断 ? "true" : "false") << std::endl;
    file << "显示头部=" << (aim::显示头部 ? "true" : "false") << std::endl;
    file << "skeletonesp=" << (aim::skeletonesp ? "true" : "false") << std::endl;
    file << "鬼跳热键=" << aim::guikey << std::endl;
    file << "碎步热键=" << aim::suikey << std::endl;
    file << "鬼跳延时=" << aim::guitime << std::endl;
    file << "碎步延时=" << aim::suitime << std::endl;
    file << "扳机延时=" << aim::bantime << std::endl;
    file << "USPtime=" << aim::USPtime << std::endl;
    file << "波特率=" << globals::波特率 << std::endl;

    file.close();

    return true;
}

void ReloadConfig()
{

        if (globals::inireload == true)
        {
            SaveConfig("CFconfig.ini");
            LoadConfig("CFconfig.ini");
        
            // 配置文件加载成功后，你可以在这里执行相应的操作以应用新的配置
        }
        
        globals::inireload = false;

    
}

WORD GetPidFromName(std::string process_name)
{
    DWORD pid = 0;
    VMMDLL_PidGetFromName(globals::hVMM, (LPSTR)process_name.c_str(), &pid);
    return pid;
}


std::vector<int> GetPidListFromName(std::string name)
{
    PVMMDLL_PROCESS_INFORMATION process_info = NULL;
    DWORD total_processes = 0;
    std::vector<int> list = { };

    if (!VMMDLL_ProcessGetInformationAll(globals::hVMM, &process_info, &total_processes))
    {

        return list;
    }

    for (size_t i = 0; i < total_processes; i++)
    {
        auto process = process_info[i];
        if (strstr(process.szNameLong, name.c_str()))
            list.push_back(process.dwPID);
    }

    return list;
}

template <typename T> static T inline Read(uintptr_t address, DWORD pid) {
    T buffer{};
    DWORD output;
    VMMDLL_MemReadEx(globals::hVMM, pid, address, (BYTE*)&buffer, sizeof(buffer), &output, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING | VMMDLL_FLAG_NOCACHEPUT | VMMDLL_FLAG_NOPAGING_IO);
    return buffer;
}

template <typename T> T inline read_kernel(uintptr_t address) {
    T t{};
    DWORD output;
    VMMDLL_MemReadEx(globals::hVMM, 4, address, (BYTE*)&t, sizeof(t), &output, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING | VMMDLL_FLAG_NOCACHEPUT | VMMDLL_FLAG_NOPAGING_IO | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY);
    return t;
}

template <typename T> static T inline read(uintptr_t address) {
    T buffer{};
    DWORD output;
    VMMDLL_MemReadEx(globals::hVMM, globals::dwPID, address, (BYTE*)&buffer, sizeof(buffer), &output, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING | VMMDLL_FLAG_NOCACHEPUT | VMMDLL_FLAG_NOPAGING_IO);
    return buffer;
}
template <typename T> static T inline readarray(uintptr_t address, T out[], size_t len) {
    DWORD output;
    return VMMDLL_MemReadEx(globals::hVMM, globals::dwPID, address, (BYTE*)out, sizeof(T) * len, &output, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING | VMMDLL_FLAG_NOCACHEPUT | VMMDLL_FLAG_NOPAGING_IO);
    ;
}


template <typename T> static T  inline write(uintptr_t address, const T& value) {

    VMMDLL_MemWrite(globals::hVMM, globals::dwPID, address, (PBYTE)&value, sizeof(T));

    return 1;
}


template <typename TP>
void scatterPrep(VMMDLL_SCATTER_HANDLE h, uint64_t address) {
    //if (h == NULL) scatterInit(h);
    VMMDLL_Scatter_Prepare(h, address, sizeof(TP));
}

template <typename TV>
static TV scatterRead(VMMDLL_SCATTER_HANDLE h, uint64_t address) {
    TV buffer;
    DWORD bytesRead = 0;
    return VMMDLL_Scatter_Read(h, address, sizeof(TV), (PBYTE)&buffer, &bytesRead) ? buffer : TV();
};
void scatterClear(VMMDLL_SCATTER_HANDLE h) {
    VMMDLL_Scatter_Clear(h, globals::dwPID, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOCACHEPUT);
}

void scatterExec(VMMDLL_SCATTER_HANDLE h) {
    VMMDLL_Scatter_ExecuteRead(h);
}

void scatterInit() {
    globals::hS = VMMDLL_Scatter_Initialize(globals::hVMM, globals::dwPID, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO | VMMDLL_FLAG_NOCACHEPUT);
}

void scatterDestroy() {
    VMMDLL_Scatter_CloseHandle(globals::hS);
}
VMMDLL_SCATTER_HANDLE CreateScatterHandle()
{
    return VMMDLL_Scatter_Initialize(globals::hVMM, globals::dwPID, VMMDLL_FLAG_NOCACHE);
}

void AddScatterReadRequest(VMMDLL_SCATTER_HANDLE handle, uint64_t address, void* buffer, size_t size)
{
    VMMDLL_Scatter_PrepareEx(handle, address, size, (PBYTE)buffer, NULL);
}
void ExecuteReadScatter(VMMDLL_SCATTER_HANDLE handle)
{
    VMMDLL_Scatter_ExecuteRead(handle);
    VMMDLL_Scatter_Clear(handle, globals::dwPID, VMMDLL_FLAG_NOCACHE);
}
void CloseScatterHandle(VMMDLL_SCATTER_HANDLE handle)
{
    VMMDLL_Scatter_CloseHandle(handle);
}



void AddScatterWriteRequest(VMMDLL_SCATTER_HANDLE handle, uint64_t address, void* buffer, size_t size)
{
    if (!VMMDLL_Scatter_PrepareWrite(handle, address, (PBYTE)buffer, size))
    {
        printf("[!] Failed to prepare scatter write at 0x%p\n", address);
    }
}


void ExecuteWriteScatter(VMMDLL_SCATTER_HANDLE handle)
{

    if (!VMMDLL_Scatter_Execute(handle))
    {
        printf("[-] Failed to Execute Scatter Read\n");
    }
    //Clear after using it
    if (!VMMDLL_Scatter_Clear(handle, globals::dwPID, NULL))
    {
        printf("[-] Failed to clear Scatter\n");
    }
}
