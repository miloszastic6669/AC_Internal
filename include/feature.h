#pragma once
#include <Windows.h>
#include <map>

enum class FeatureID
{
  add_health,
  infinite_ammo,
  high_jump,
  COUNT
};

enum KeyBinds
{
  add_health = VK_F1,//button
  infinite_ammo = VK_F2,//toggle
  high_jump = VK_F3,//toggle
};

class FeatureStates 
{
private:
  std::map<FeatureID, bool> states;

public:
  void toggle(FeatureID f);
  void toggle(FeatureID f, bool state);
  FeatureStates();

};

