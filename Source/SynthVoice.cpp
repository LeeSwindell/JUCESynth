/*
  ==============================================================================

    SynthVoice.cpp
    Created: 16 Feb 2022 9:52:50am
    Author:  Lee Swindell

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
};

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    adsr.noteOn();
};

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
};
void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
};

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
    
};

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    osc.prepare(spec);
    gain.prepare(spec);
    
    gain.setGainLinear(0.01f);
    
    isPrepared = true;
};

void SynthVoice::renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);
    
    juce::dsp::AudioBlock<float> audioBlock { outputBuffer };
    osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
    /*The audio block is actually just an alias for the output buffer,
      so processing the osc and gain in the audio block will still be affected
      when the envelope is applied to the output buffer later */
};
