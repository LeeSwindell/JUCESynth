/*
  ==============================================================================

    OscData.cpp
    Created: 20 Feb 2022 3:04:39pm
    Author:  Lee Swindell

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
    fmOsc.prepare(spec);
    prepare(spec);
}

void OscData::setWavetype(const int wavetype)
{
    switch (wavetype) {
        case Wavetypes::Sine:
            initialise([](float x) { return std::sin(x); });
            break;
            
        case Wavetypes::Saw:
            initialise([](float x) { return x / juce::MathConstants<float>::pi; });
            break;
            
        case Wavetypes::Square:
            initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
            break;
            
        default:
            jassertfalse;
            break;
    }
}

void OscData::setWaveFrequency(const int midiNoteNumber)
{
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    lastMidiNote = midiNoteNumber;
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float> &audioBlock)
{
    for (int ch=0; ch < audioBlock.getNumChannels(); ++ch)
    {
        for (int s=0; s < audioBlock.getNumSamples(); ++s)
        {
            fmMod = fmDepth * fmOsc.processSample(audioBlock.getSample(ch, s));
        }
    }
    
    process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

void OscData::setFMParams(const float freq, const float depth)
{
    fmOsc.setFrequency(freq);
    fmDepth = depth;
    setWaveFrequency(lastMidiNote);
}
