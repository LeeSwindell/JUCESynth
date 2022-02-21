/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Synth1AudioProcessorEditor::Synth1AudioProcessorEditor (Synth1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 450);
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    attackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "ATT", attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DEC", decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SUS", sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "REL", releaseSlider);
    oscAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC", oscSelector);
    fmFreqAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "FMFREQ", fmFreqSlider);
    fmDepthAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "FMDEPTH", fmDepthSlider);
    
    createSliderSettings(attackSlider);
    createSliderSettings(decaySlider);
    createSliderSettings(sustainSlider);
    createSliderSettings(releaseSlider);
    addAndMakeVisible(oscSelector);
    oscSelector.addItem("Sine", 1);
    oscSelector.addItem("Saw", 2);
    oscSelector.addItem("Square", 3);
    createSliderSettings(fmFreqSlider);
    createSliderSettings(fmDepthSlider);
}

Synth1AudioProcessorEditor::~Synth1AudioProcessorEditor()
{
}

//==============================================================================
void Synth1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    
}

void Synth1AudioProcessorEditor::resized()
{
//    auto bounds = getLocalBounds().reduced(10);
    attackSlider.setBounds(0, 20, 300, 40);
    decaySlider.setBounds(0, 60, 300, 40);
    sustainSlider.setBounds(0, 100, 300, 40);
    releaseSlider.setBounds(0, 140, 300, 40);
    oscSelector.setBounds(350, 200, 50, 50);
    fmFreqSlider.setBounds(0, 180, 200, 40);
    fmDepthSlider.setBounds(0, 220, 200, 40);
}

void Synth1AudioProcessorEditor::createSliderSettings(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    addAndMakeVisible(slider);
}
