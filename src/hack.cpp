#include "../include/hack.h"
#include "../include/feature.h"
#include "../include/offsets.h"


//hacks 

void hack::add_health(bool enabled, int amount)
{
  if (!(GetAsyncKeyState(KeyBinds::add_health) & 1)) return;
  
  os::local_player->health += amount;
}

void hack::infinite_ammo(bool enabled)
{
  if (!enabled) return;
  *os::local_player->cur_weapon_ptr->cur_ammo_ptr = 666;
}

void hack::high_jump(bool enabled)
{
  if (!enabled) return;
  
}




