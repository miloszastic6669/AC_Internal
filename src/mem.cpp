#include "../include/mem.h"


void mem::patch(BYTE* dst, BYTE* src, unsigned int size)
{
  DWORD oldprotect;
  VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
  memcpy(dst, src, size);
  VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void mem::copy(BYTE* from, BYTE* to, unsigned int size)
{
  patch(from, to, size);
}

void mem::nop(BYTE* dst, unsigned int size)
{
  DWORD oldprotect;
  VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
  memset(dst, 0x90, size);
  VirtualProtect(dst, size, oldprotect, &oldprotect);
}

uintptr_t mem::find_dma_addy(uintptr_t ptr, std::vector<uintptr_t> offsets)
{
  uintptr_t addr = ptr;
  for (const auto& i : offsets)
  {
    addr = *(uintptr_t*)addr;
    addr += i;
  }
  return addr;
}




