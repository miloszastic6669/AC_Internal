#include "../include/feature.h"

void FeatureStates::toggle(FeatureID f)
{
  states[f] = !states[f];
}

void FeatureStates::toggle(FeatureID f, bool state)
{
  states[f] = state;
}

FeatureStates::FeatureStates()
{

}
