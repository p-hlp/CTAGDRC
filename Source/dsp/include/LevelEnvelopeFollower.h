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

/* Basic envelope-follwer, to track peak & rms signal level with configurable decay time*/
class LevelEnvelopeFollower
{
public:
    LevelEnvelopeFollower() = default;

    // Prepares envelope follower with given sample rate and recalculates decayInSamples
    // as well as the peak/rms coefficient
    void prepare(const double& fs);

    // Set peak decay
    void setPeakDecay(float dc);

    // Set rms decay
    void setRmsDecay(float dc);

    // Updates peak envelope follower from given audio buffer
    void updatePeak(const float* const* channelData, int numChannels, int numSamples);

    // Updates rms envelope follower from given audio buffer
    void updateRMS(const float* const* channelData, int numChannels, int numSamples);

    // Gets current peak, call after updatePeak
    float getPeak();

    // Gets current rms, vall after updateRMS
    float getRMS();

private:
    float currMaxPeak{0.0f};
    float currMaxRMS{0.0f};
    float peakDecay{0.99992f};
    float rmsDecay{0.95f};
    float peakDecayInSeconds{0.5f};
    float rmsDecayInSeconds{0.0f};

    int peakDecayInSamples{0};
    int rmsDecayInSamples{0};

    double sampleRate{0.0f};
};
