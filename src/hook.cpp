#include "../include/hook.h"
#include <utility>
#include "../include/mem.h"
#include <iostream>

//everything used for hooks in here...
bool Hook::hook(void** our_gateway_ptr)
{
  if (stolen_bytes_len < 5) return false;


  //initialize tramp func
  gateway_func_addr = VirtualAlloc(NULL, stolen_bytes_len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  *our_gateway_ptr = gateway_func_addr;
  std::cout << "Gateway Func Addr = 0x" << std::hex << gateway_func_addr << "\n";

  //write stolen bytes to gateway func
  mem::patch(RC_BP(gateway_func_addr), stolen_bytes, stolen_bytes_len);
  
  //write jump to (original func + stolen_bytes_len) at the end of gateway func
  BYTE jmp = '\xe9';
  BYTE* gateway_func_jmp_instr = RC_BP(gateway_func_addr) + stolen_bytes_len;
  int gateway_relative_offset = (int)orig_func_after_jmp_addr - ((int)gateway_func_addr + stolen_bytes_len + 5);//offset to original function from gateway
  mem::patch(gateway_func_jmp_instr, &jmp, 1);                               //jmp
  mem::patch(gateway_func_jmp_instr + 1, (BYTE*)&gateway_relative_offset, 4);// (orig_func_addr+5) - orig_func_jmp_addr
  
  //patch original func with jmp to our func and address
  int our_func_rel_addr = (BYTE*)our_func_addr - (BYTE*)orig_func_jmp_addr - 5;//offset to our function from original func
  mem::patch(RC_BP(orig_func_jmp_addr), &jmp, 1); //jmp
  mem::patch(RC_BP(orig_func_jmp_addr) + 1, RC_BP(&our_func_rel_addr), 4); //our func address
  
  hooked = true;
  return true;
}

bool Hook::hook()
{
  this->hook(our_gateway_func);
  
  return true;
}


bool Hook::unhook()
{
  //restore original bytes
  mem::patch(RC_BP(orig_func_addr), stolen_bytes, stolen_bytes_len);

  //deallocate tramp_func?
  VirtualFree(gateway_func_addr, 0, MEM_RELEASE); //unsure
  hooked = false;
  return true;
}

void** Hook::get_gateway_func()
{
  return this->our_gateway_func;
}

Hook* HookManager::get_hook_by_address(void* orig_func_address)
{
  for (auto& h : hooks) 
    if (h.get_orig_func() == orig_func_address) return &h;
  return nullptr;
}

bool HookManager::apply_all()
{
  for (auto& h : hooks) 
    if (!h.is_hooked()) return h.hook(h.get_gateway_func());

}

bool HookManager::apply_hook(Hook* p)
{
  if (!p->is_hooked())  return p->hook(p->get_gateway_func());
} 

bool HookManager::unhook_all()
{
  for (auto& h : hooks)
    if (h.is_hooked())
      if (!h.unhook()) return false;
  return true;
}

bool HookManager::unhook(Hook* h)
{
  if (h->is_hooked()) return h->unhook();
}
