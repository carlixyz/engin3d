#pragma once
#ifndef INPUT_ACTION_H
#define INPUT_ACTION_H

class cInputAction
{
  private:
    float mfValue;
    float mfTimePressed;
    bool mbIsPressed;
    bool mbWasPressed;
    bool mbBecomeReleased;
    bool mbBecomePressed;

  public:
    void Init(void);
    void Update(float lfTimeStep, float lfValue);
    inline bool GetIsPressed(void) const {return mbIsPressed;}
    inline bool GetIsReleased(void) const {return !mbIsPressed;}
    inline bool GetBecomePressed(void) const {return mbBecomePressed;}
    inline bool GetBecomeReleased(void) const {return mbBecomeReleased;}
    inline float GetPressedTime(void) const {return mfTimePressed;}
    inline float GetValue(void) const {return mfValue;}
};

#endif