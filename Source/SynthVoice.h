/*
  ==============================================================================

    SynthVoice.h
    Created: 16 Feb 2022 9:52:50am
    Author:  Lee Swindell

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;
    
//    bool isVoiceActive () const override;
//    void pitchWheelMoved (int newPitchWheelValue) override;
//    void aftertouchChanged (int newAftertouchValue) override;
//    void channelPressureChanged (int newChannelPressureValue) override;
//    void renderNextBlock (juce::AudioBuffer<double> &outputBuffer, int startSample, int numSamples) override;
private:
};
