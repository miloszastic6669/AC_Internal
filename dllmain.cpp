// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "include/offsets.h"
#include "include/hack.h"

DWORD __stdcall EjectThread(LPVOID lpParameter);

int counter = 0;

BOOL (__stdcall* swap_buffers_gateway)(HDC hDc);

BOOL __stdcall gl_swap_buffers_hook(HDC hDc) 
{
  counter++;
  return swap_buffers_gateway(hDc);
  
}



void main_hack() 
{ 
  uintptr_t game_base = reinterpret_cast<uintptr_t>(GetModuleHandle("ac_client.exe"));
  void* gl_swap_buffers_addr = GetProcAddress(GetModuleHandle("opengl32.dll"), "wglSwapBuffers");
  Hook swap_buffers_hk(gl_swap_buffers_addr, gl_swap_buffers_hook, 5, (void**)&swap_buffers_gateway);

  AllocConsole();
  freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
  std::cout << "Game Base  = 0x" << std::hex << game_base << std::dec << "\n";
  std::cout << "SwapBuffers Address = 0x" << std::hex << gl_swap_buffers_addr << std::dec << "\n";
  std::cout << "SwapBuffersHook Address = 0x" << std::hex << gl_swap_buffers_hook << std::dec << "\n";
  //offsets
  std::cout << "hooking...\n";
  swap_buffers_hk.hook();
  os::init(game_base);
  
  while (!GetAsyncKeyState(VK_END))
  {
    //call hacks
    hack::add_health(true, 50);
  }
    //deallocate console
  std::cout << "Broken out of main hack loop!\n";
  std::cout << "Unhooking all hooks...\n";
  swap_buffers_hk.unhook();
  CreateThread(0, 0, EjectThread, 0, 0, 0);
  return;
}
HINSTANCE DllHandle;

DWORD __stdcall EjectThread(LPVOID lpParameter) //+ deallocate and close console
{
  std::cout << "Unloading...\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::cout << "Done unloading...\n";
  
  std::cout << "Deallocating and closing console...\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  //FreeConsole();
 
  if (HWND hwnd = GetConsoleWindow())
  {
    FreeConsole();
    PostMessage(hwnd, WM_CLOSE, 0, 0);
  }
  FreeLibraryAndExitThread(DllHandle, 0);
  return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved)
{
  if (ul_reason_for_call == DLL_PROCESS_ATTACH)
  {
    DllHandle = hModule;
    DisableThreadLibraryCalls(hModule);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main_hack, hModule, 0, 0);
  }
  if (ul_reason_for_call == DLL_PROCESS_DETACH) 
    FreeConsole();
  

  return TRUE;
}

