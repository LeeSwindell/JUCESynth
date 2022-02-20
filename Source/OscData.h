/*
  ==============================================================================

    OscData.h
    Created: 20 Feb 2022 3:04:39pm
    Author:  Lee Swindell

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

enum Wavetypes
{
    Sine=0,
    Saw=1,
    Square=2
};

class OscData : public juce::dsp::Oscillator<float>
{
public:
    void setWavetype(const int wavetype);
    
private:
    
};
