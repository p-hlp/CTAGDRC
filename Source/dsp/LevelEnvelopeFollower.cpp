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
#include "include/LevelEnvelopeFollower.h"

#include <cassert>
#include <cmath>

void LevelEnvelopeFollower::prepare(const double& fs)
{
    sampleRate = fs;

    peakDecayInSamples = static_cast<int>(peakDecayInSeconds * sampleRate);
    peakDecay = 1.0f - 1.0f / static_cast<float>(peakDecayInSamples);

    rmsDecayInSamples = static_cast<int>(rmsDecayInSeconds * sampleRate);
    rmsDecay = 1.0f - 1.0f / static_cast<float>(rmsDecayInSamples);
}

// Set peak decay
void LevelEnvelopeFollower::setPeakDecay(float dc)
{
    peakDecayInSeconds = dc;
    prepare(sampleRate);
}

// Set rms decay
void LevelEnvelopeFollower::setRmsDecay(float dc)
{
    rmsDecayInSeconds = dc;
    prepare(sampleRate);
}

// Updates peak envelope follower from given audio buffer
void LevelEnvelopeFollower::updatePeak(const float* const* channelData, int numChannels, int numSamples)
{
    assert(numChannels >= 0 && numSamples >= 0 && channelData != nullptr);
    if (numChannels > 0 && numSamples > 0)
    {
        for (int i = 0; i < numSamples; ++i)
        {
            float sum = 0.0f;
            for (int j = 0; j < numChannels; ++j)
                sum += std::abs(channelData[j][i]);

            sum /= static_cast<float>(numChannels);

            if (sum > currMaxPeak)
                currMaxPeak = sum;
            else if (currMaxPeak > 0.001f)
                currMaxPeak *= peakDecay;
            else currMaxPeak = 0.0f;
        }
    }
}

// Updates rms envelope follower from given audio buffer
void LevelEnvelopeFollower::updateRMS(const float* const* channelData, int numChannels, int numSamples)
{
    assert(numChannels >= 0 && numSamples >= 0 && channelData != nullptr);
    if (numChannels > 0 && numSamples > 0)
    {
        for (int i = 0; i < numSamples; ++i)
        {
            float sum = 0.0f;
            for (int j = 0; j < numChannels; ++j)
                sum += std::abs(channelData[j][i]);

            sum /= static_cast<float>(numChannels);
            sum *= sum;

            if (sum > currMaxRMS)
                currMaxRMS = sum * rmsDecay;
            else if (currMaxRMS > 0.001f)
                currMaxRMS *= peakDecay;
            else currMaxRMS = 0.0f;
        }
    }
}

// Gets current peak, call after updatePeak
float LevelEnvelopeFollower::getPeak()
{
    return currMaxPeak;
}

// Gets current rms, vall after updateRMS
float LevelEnvelopeFollower::getRMS()
{
    return sqrt(currMaxRMS);
}
