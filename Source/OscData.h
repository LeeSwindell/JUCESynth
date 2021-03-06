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
    void prepareToPlay(juce::dsp::ProcessSpec& spec);
    void setWavetype(const int wavetype);
    void setWaveFrequency(const int midiNoteNumber);
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& audioBlock);
    void setFMParams(const float freq, const float depth);
    
private:
    juce::dsp::Oscillator<float> fmOsc { [](float x) { return std::sin(x); } };
    float fmMod { 0.0f };
    float fmDepth { 0.0f };
    int lastMidiNote;
};
