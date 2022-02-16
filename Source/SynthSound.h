/*
  ==============================================================================

    SynthSound.h
    Created: 16 Feb 2022 9:53:32am
    Author:  Lee Swindell

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; };
    bool appliesToChannel(int midiChannel) override { return true; };
};
