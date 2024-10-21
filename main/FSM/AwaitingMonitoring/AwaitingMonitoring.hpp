#ifndef AWAITINGMONITORING_HPP
#define AWAITINGMONITORING_HPP
#include "State.hpp"

class AwaitingMonitoring : public State{
  public:
    void Enter() override;
    void Update() override;
    void Exit() override;
};

#endif 