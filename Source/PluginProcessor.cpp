/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Synth1AudioProcessor::Synth1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
}

Synth1AudioProcessor::~Synth1AudioProcessor()
{
}

//==============================================================================
const juce::String Synth1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Synth1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Synth1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Synth1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Synth1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Synth1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Synth1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Synth1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Synth1AudioProcessor::getProgramName (int index)
{
    return {};
}

void Synth1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Synth1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    }
}

void Synth1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Synth1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Synth1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            auto& oscWavetype = *apvts.getRawParameterValue("OSC");
            auto& attack = *apvts.getRawParameterValue("ATT");
            auto& decay = *apvts.getRawParameterValue("DEC");
            auto& sustain = *apvts.getRawParameterValue("SUS");
            auto& release = *apvts.getRawParameterValue("REL");
            auto& fmFreq = *apvts.getRawParameterValue("FMFREQ");
            auto& fmDepth = *apvts.getRawParameterValue("FMDEPTH");

            voice->getOscillator().setWavetype(oscWavetype);
            voice->getOscillator().setFMParams(fmFreq, fmDepth);
            voice->updateADSR(attack.load(), decay.load(), sustain.load(), release.load());
        }
    }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool Synth1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Synth1AudioProcessor::createEditor()
{
    return new Synth1AudioProcessorEditor (*this);
}

//==============================================================================
void Synth1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Synth1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Synth1AudioProcessor();
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout Synth1AudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    //Osc1 Selector
    params.push_back(std::make_unique<juce::AudioParameterChoice> ("OSC", "Oscillator", juce::StringArray { "Sine", "Saw", "Square" }, 0));
    
    //ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat> ("ATT", "Attack", juce::NormalisableRange<float> { 0.1f, 1.0f }, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat> ("DEC", "Decay", juce::NormalisableRange<float> { 0.1f, 1.0f }, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat> ("SUS", "Sustain", juce::NormalisableRange<float> { 0.1f, 1.0f }, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat> ("REL", "Release", juce::NormalisableRange<float> { 0.1f, 3.0f }, 0.4f));
    
    //FM
    params.push_back(std::make_unique<juce::AudioParameterFloat> ("FMFREQ", "FM Frequency", juce::NormalisableRange<float> { 0.0f, 1000.0f }, 5.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat> ("FMDEPTH", "FM Depth", juce::NormalisableRange<float> { 0.0f, 1000.0f }, 500.0f));
    
    return { params.begin(), params.end() };
}
