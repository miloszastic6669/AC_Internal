#pragma once
#include <Windows.h>
#include <vector>

namespace mem 
{
  void patch(BYTE* dst, BYTE* src, unsigned int size);
  void copy(BYTE* dst, BYTE* src, unsigned int size);
  void nop(BYTE* dst, unsigned int size);
  uintptr_t find_dma_addy(uintptr_t ptr, std::vector<uintptr_t> offsets);
}

