#ifndef MONITORING_HPP
#define MONITORING_HPP
#include "State.hpp"

class Monitoring : public State{
  public:
    void Enter() override;
    void Update() override;
    void Exit() override;
};

#endif 