#include "../include/offsets.h"



namespace os 
{
  Player* local_player = nullptr;

  void init(uintptr_t _base) 
  {
    local_player_ptr += _base;
    os::local_player = reinterpret_cast<Player*>(*(uintptr_t*)local_player_ptr);
    std::cout << "Local player = " << std::hex << os::local_player << std::dec << "\n";
  }

}