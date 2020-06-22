/*
Copyright (C) 2020 Phillip Lamp
This file is part of the CTAGDRC project <https://github.com/p-hlp/CTAGDRC>.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

/*Basic delay line implementation with Juce's AudioBuffer class*/
class DelayLine
{
public:

    DelayLine() = default;

    // Prepares delayline with ProcessSpec-Object, resizes delay-buffer, recalculates delayInSamples
    // and resets writePosition
    void prepare(const dsp::ProcessSpec& spec);

    // Delays given AudioBuffer by delayInSamples samples
    void process(AudioBuffer<float>& buffer);

    // Sets delay
    void setDelay(float delayInSeconds);

private:
    void pushSamples(const AudioBuffer<float>& srcBuffer, int nSam, int nCh);
    void readSamples(AudioBuffer<float>& dstBuffer, int nSam, int nCh);

    inline void getReadBlockSize(int numSamples, int& index, int& block1, int& block2);
    inline void getWriteBlockSize(int numSamples, int& index, int& block1, int& block2);

private:
    //Directly initialize process spec to avoid debugging problems
    dsp::ProcessSpec procSpec{-1, 0, 0};

    AudioBuffer<float> delayBuffer;
    int delayBufferSize{0};
    int delayInSamples{0};
    float delayInSeconds{0.0f};
    int writePosition{0};
    bool isBypassed{true};
};
