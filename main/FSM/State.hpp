#ifndef STATE_HPP
#define STATE_HPP

class VibrationSystem; // forward declaration

class State {
  protected:
    VibrationSystem* _vibration_system;
  public:
    virtual ~State(){}
    void SetVibrationSystem(VibrationSystem* vibration_system){
      _vibration_system = vibration_system;
    }
    virtual void Enter() = 0;
    virtual void Update() = 0;
    virtual void Exit() = 0;
};

#endif // STATE_HPP