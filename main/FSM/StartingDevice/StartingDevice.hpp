#ifndef STARTINGDEVICE_HPP
#define STARTINGDEVICE_HPP
#include "State.hpp"

class StartingDevice : public State{
  public:
    void Enter() override;
    void Update() override;
    void Exit() override;
};

#endif 