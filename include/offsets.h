#pragma once
#include <Windows.h>
#include <iostream>
#include "mem.h"

// for pointers relative to base
namespace 
{
  uintptr_t local_player_ptr = 0x10f4f4;
}

struct Weapon // sizeof=0x18
{
  char field_0[20];
  int* cur_ammo_ptr;
};

struct Player // sizeof=0x37C
{
  char padding1[16];
  float x_force;
  float y_force;
  float z_force;
  char padding2[24];
  float x_coord;
  float y_coord;
  float z_coord;
  float yaw;
  float pitch;
  float roll;
  char padding3[172];
  int health;
  int armor;
  char padding4[24];
  int sec_weapon_reserve_ammo;
  int prim_weapon_reserve_ammo;
  char padding5[28];
  int sec_weapon_ammo;
  int prim_weapon_ammo;
  char padding6[20];
  int grenade_ammo;
  char padding7[4];
  int melee_delay;
  int sec_weapon_delay;
  int prim_weapon_delay;
  char padding8[32];
  int sec_weapon_bullets_fired;
  char padding9[4];
  int prim_weapon_bullets_fired;
  char padding10[141];
  char field_225[83];
  char nickname[128];
  char padding11[128];
  Weapon* cur_weapon_ptr;
};

namespace os 
{

  extern Player* local_player;

  void init(uintptr_t _base);
  

  

}


