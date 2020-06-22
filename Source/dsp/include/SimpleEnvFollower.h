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

/* Basic envelope-follwer, to track peak & rms signal level with configurable decay time*/
class SimpleEnvFollower
{
public:
    SimpleEnvFollower() = default;

    // Prepares envelope follower with given sample rate and recalculates decayInSamples
    // aswell as the peak/rms coefficient
    void prepare(const double& fs)
    {
        sampleRate = fs;

        peakDecayInSamples = static_cast<float>(peakDecayInSeconds * sampleRate);
        peakDecay = 1.0f - 1.0f / peakDecayInSamples;

        rmsDecayInSamples = static_cast<float>(rmsDecayInSeconds * sampleRate);
        rmsDecay = 1.0f - 1.0f / rmsDecayInSamples;
    }

    // Set peak decay
    void setPeakDecay(float dc)
    {
        peakDecayInSeconds = dc;
        prepare(sampleRate);
    }

    // Set rms decay
    void setRmsDecay(float dc)
    {
        rmsDecayInSeconds = dc;
        prepare(sampleRate);
    }

    // Updates peak envelope follower from given audio buffer
    void updatePeak(const float* const* channelData, int numChannels, int numSamples)
    {
        jassert(numChannels >= 0 && numSamples >= 0 && channelData != nullptr);
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
    void updateRMS(const float* const* channelData, int numChannels, int numSamples)
    {
        jassert(numChannels >= 0 && numSamples >= 0 && channelData != nullptr);
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
    float getPeak() { return currMaxPeak; }

    // Gets current rms, vall after updateRMS
    float getRMS() { return sqrt(currMaxRMS); }

private:
    // TODO: Change DecayInSamples to Integer
    float currMaxPeak{0.0f};
    float currMaxRMS{0.0f};
    float peakDecay{0.99992f}, rmsDecay{0.95f};
    float peakDecayInSeconds{0.5f}, rmsDecayInSeconds{0.0f};
    float peakDecayInSamples{0.0f}, rmsDecayInSamples{0.0f};
    double sampleRate{0.0f};
};
