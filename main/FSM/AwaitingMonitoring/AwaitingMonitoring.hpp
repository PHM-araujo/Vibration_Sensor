#ifndef AWAITINGMONITORING_HPP
#define AWAITINGMONITORING_HPP
#include "State.hpp"
#include "stdint.h"

class AwaitingMonitoring : public State{
  public:
    void Enter() override;
    void Update() override;
    void Exit() override;
  
  private: 
    int64_t start_time = 0;
};

#endif 