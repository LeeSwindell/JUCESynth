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
    osc.setWaveFrequency(midiNoteNumber);
    adsr.noteOn();
};

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
    if (! allowTailOff || ! isVoiceActive())
        clearCurrentNote();
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
    
    osc.prepareToPlay(spec);
    gain.prepare(spec);
    
    gain.setGainLinear(0.5f);
        
    isPrepared = true;
};

void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
    
    adsr.setParameters(adsrParams);
}

void SynthVoice::renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);
    
    if (! isVoiceActive())
        return;
    
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlock { synthBuffer };
    osc.getNextAudioBlock(audioBlock);
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        
        if (! adsr.isActive())
            clearCurrentNote();
    }
    
    /*Takes all new information (notes that are played or stopped during the render block)
      and adds it to the existing audio output buffer via the intermediary synthBuffer.
      this prevents clicks compared to updating the whole output buffer in the render block*/
};
