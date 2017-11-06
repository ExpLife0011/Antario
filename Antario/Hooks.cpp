#include "Hooks.h"
#include "SDK\IClientMode.h"
#include "Utils\Utils.h"

Hooks g_Hooks;

void Hooks::Init()
{
    // Get interfaces
    Interfaces::Init();  

    // D3D Device pointer
    uintptr_t d3dDevice = **(uintptr_t**)(g_Utils.FindSignature("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);

    // VMTHooks
    //g_pHooks->pD3DDevice9Hook = std::make_unique<VMTHook>(reinterpret_cast<void*>(d3dDevice));
    g_Hooks.pClientModeHook = std::make_unique<VMTHook>(g_pClientMode);
    g_Hooks.pClientModeHook->Hook(24, (void*)Hooks::CreateMove);
}



void Hooks::Restore()
{
    // Unhook every function we hooked and restore original one to VMT
    g_Hooks.pClientModeHook->Unhook(24);
}



bool __fastcall Hooks::CreateMove(IClientMode* thisptr, void* edx, float sampleInputFrametime, CUserCmd* cmd)
{
    // Call original createmove before we start screwing with it
    auto oCreateMove = g_Hooks.pClientModeHook->GetOriginal<CreateMove_t>(24);
    oCreateMove(thisptr, sampleInputFrametime, cmd);

    if (!cmd || cmd->command_number == 0)
        return false;

    // call shit
    return false;
}