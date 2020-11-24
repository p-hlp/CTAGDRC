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
#include "include/Compressor.h"

#include "../util/include/SIMDMath.h"


Compressor::~Compressor()
{
    rawSidechainSignal = nullptr;
}

void Compressor::prepare(const dsp::ProcessSpec& ps)
{
    procSpec = ps;
    ballistics.prepare(ps.sampleRate);
    delay.setDelay(0.005f);
    delay.prepare(ps);
    originalSignal.setSize(2, ps.maximumBlockSize);
    sidechainSignal.resize(ps.maximumBlockSize, 0.0f);
    rawSidechainSignal = sidechainSignal.data();
    originalSignal.clear();
    lookahead.prepare(procSpec.sampleRate, lookaheadDelay, ps.maximumBlockSize);
    smoothedAutoMakeup.prepare(ps.sampleRate);
    smoothedAutoMakeup.setAlpha(0.03);
}

void Compressor::setPower(bool newPower)
{
    bypassed = newPower;
}

void Compressor::setInput(float newInput)
{
    input = newInput;
}

void Compressor::setAttack(float attackTimeInMs)
{
    ballistics.setAttack(attackTimeInMs * 0.001);
}

void Compressor::setRelease(float releaseTimeInMs)
{
    ballistics.setRelease(releaseTimeInMs * 0.001);
}

void Compressor::setRatio(float rat)
{
    gainComputer.setRatio(rat);
}

void Compressor::setKnee(float kneeInDb)
{
    gainComputer.setKnee(kneeInDb);
}

void Compressor::setThreshold(float thresholdInDb)
{
    gainComputer.setThreshold(thresholdInDb);
}

void Compressor::setMakeup(float makeupGainInDb)
{
    makeup = makeupGainInDb;
}

void Compressor::setMix(float newMix)
{
    mix = newMix;
}

void Compressor::setAutoAttack(bool isEnabled)
{
    ballistics.setAutoAttack(isEnabled);
}

void Compressor::setAutoRelease(bool isEnabled)
{
    ballistics.setAutoRelease(isEnabled);
}

void Compressor::setAutoMakeup(bool isEnabled)
{
    autoMakeupEnabled = isEnabled;
}

void Compressor::setLookahead(bool newLookahead)
{
    lookaheadEnabled = newLookahead;
}

float Compressor::getMakeup()
{
    return makeup;
}

double Compressor::getSampleRate()
{
    return procSpec.sampleRate;
}


float Compressor::getMaxGainReduction()
{
    return maxGainReduction;
}

void Compressor::process(AudioBuffer<float>& buffer)
{
    if (!bypassed)
    {
        const auto numSamples = buffer.getNumSamples();
        const auto numChannels = buffer.getNumChannels();

        jassert(numSamples == static_cast<int>(sidechainSignal.size()));

        // Clear any old samples
        originalSignal.clear();
        FloatVectorOperations::fill(rawSidechainSignal, 0.0f, numSamples);
        maxGainReduction = 0.0f;

        // Apply input gain
        applyInputGain(buffer, numSamples);

        // Get max l/r amplitude values and fill sidechain signal
        FloatVectorOperations::abs(rawSidechainSignal, buffer.getReadPointer(0), numSamples);
        FloatVectorOperations::max(rawSidechainSignal, rawSidechainSignal, buffer.getReadPointer(1), numSamples);

        // Calculate crest factor on max. amplitude values of input buffer
        ballistics.processCrestFactor(rawSidechainSignal, numSamples);

        // Compute attenuation - converts side-chain signal from linear to logarithmic domain
        gainComputer.applyCompressionToBuffer(rawSidechainSignal, numSamples);

        // Smooth attenuation - still logarithmic
        ballistics.applyBallistics(rawSidechainSignal, numSamples);

        // Get minimum = max. gain reduction from side chain buffer
        maxGainReduction = FloatVectorOperations::findMinimum(rawSidechainSignal, numSamples);

        // Calculate auto makeup
        autoMakeup = calculateAutoMakeup(rawSidechainSignal, numSamples);

        // Do lookahead if enabled
        if (lookaheadEnabled)
        {
            // Delay input buffer
            delay.process(buffer);

            // Process side-chain (delay + gain reduction fade in)
            lookahead.process(rawSidechainSignal, numSamples);
        }

        // Add makeup gain and convert side-chain to linear domain
        for (int i = 0; i < numSamples; ++i)
            sidechainSignal[i] = Decibels::decibelsToGain(sidechainSignal[i] + makeup + autoMakeup);

        // Copy buffer to original signal
        for (int i = 0; i < numChannels; ++i)
            originalSignal.copyFrom(i, 0, buffer, i, 0, numSamples);

        // Multiply attenuation with buffer - apply compression
        for (int i = 0; i < numChannels; ++i)
            FloatVectorOperations::multiply(buffer.getWritePointer(i), rawSidechainSignal, buffer.getNumSamples());

        // Mix dry & wet signal
        for (int i = 0; i < numChannels; ++i)
        {
            float* channelData = buffer.getWritePointer(i); //wet signal
            FloatVectorOperations::multiply(channelData, mix, numSamples);
            FloatVectorOperations::addWithMultiply(channelData, originalSignal.getReadPointer(i), 1 - mix, numSamples);
        }
    }
}

inline void Compressor::applyInputGain(AudioBuffer<float>& buffer, int numSamples)
{
    if (prevInput == input)
        buffer.applyGain(0, numSamples, Decibels::decibelsToGain(prevInput));
    else
    {
        buffer.applyGainRamp(0, numSamples, Decibels::decibelsToGain(prevInput), Decibels::decibelsToGain(input));
        prevInput = input;
    }
}

inline float Compressor::calculateAutoMakeup(const float* src, int numSamples)
{
    const float sum = SIMDMath::sum(src, numSamples);
    smoothedAutoMakeup.process(-sum / static_cast<float>(numSamples));
    return autoMakeupEnabled ? static_cast<float>(smoothedAutoMakeup.getState()) : 0.0f;
}
