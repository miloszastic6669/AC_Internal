#pragma once
#include <utility>
#include <vector>
#include <iostream>
#include <Windows.h>
#include "../include/mem.h"
#define RC_UI(x) reinterpret_cast<unsigned int>(x)
#define RC_VP(x) reinterpret_cast<void*>(x)
#define RC_BP(x) reinterpret_cast<BYTE*>(x)


class Hook
{
private:
  unsigned int jump_rel_offset;//in case you want to place hook further inside the function(may break something i think)
  //stolen instructions can be max 20 bytes 
  BYTE stolen_bytes[20] = "\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0";
  unsigned int stolen_bytes_len = 20;
  void* orig_func_addr;
  void* our_func_addr;
  void* orig_func_jmp_addr;//where jmp to our function is
  void* orig_func_after_jmp_addr;//where our gateway func will jump to after executing stolen byte
  void* gateway_func_addr = nullptr;//our gateway function
  void** our_gateway_func = nullptr;
  bool hooked;

public:
  //automatically get stolen bytes
  Hook(void* _orig_func,
       void* _our_func,
       unsigned int stolen_bytes_size,
       void** gateway_func,
       bool _apply_hook = false, // will activate the hook instantly
       unsigned int _jump_rel_offset = 0)
    : orig_func_addr{ _orig_func }, our_func_addr{ _our_func }, jump_rel_offset{ _jump_rel_offset }, hooked{ _apply_hook }, stolen_bytes_len{stolen_bytes_size}, our_gateway_func{gateway_func}
  {
    orig_func_jmp_addr = RC_BP(_orig_func)+ _jump_rel_offset;
    orig_func_after_jmp_addr = RC_BP(orig_func_jmp_addr) + stolen_bytes_len;
    

    //copy stolen bytes
    mem::copy(stolen_bytes, RC_BP(orig_func_jmp_addr), stolen_bytes_size);
    if (_apply_hook)
      hook(gateway_func);
  }
   
    //returns true if hook is active... duh...
  bool is_hooked() const
  {
    return hooked;
  }

  void* get_orig_func() const { return orig_func_addr; }
  bool hook(void** our_gateway_ptr);
  bool hook();
  bool unhook();
  void** get_gateway_func();
};


class HookManager
{
private:
  std::vector<Hook> hooks;

public:
  Hook* get_hook_by_address(void* func_address);
  bool apply_all();
  bool apply_hook(Hook* h);
  bool unhook_all();
  bool unhook(Hook* h);
  void register_hook(Hook h) { hooks.push_back(h); }


};

