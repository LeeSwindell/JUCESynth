/*
  ==============================================================================

    OscData.cpp
    Created: 20 Feb 2022 3:04:39pm
    Author:  Lee Swindell

  ==============================================================================
*/

#include "OscData.h"

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
