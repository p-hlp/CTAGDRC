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
#include "CrestFactor.h"
#include "SmoothingFilter.h"


/*LevelDetector Class:
 * Used to have a smooth representation of the level
 * Might be used in linear or log. domain
 * In this compressor implementation it's used in log. domain after the gain computer to smooth the calculated attenuations,
 * therefore the detector does not have to work on the whole dynamic range of the input signal
 */
class LevelDetector
{
public:
    LevelDetector() = default;

    // Prepares LevelDetector with a ProcessSpec-Object containing samplerate, blocksize and number of channels
    void prepare(const double& fs);

    // Sets attack time constant
    void setAttack(const double&);

    // Sets release time constant
    void setRelease(const double&);

    // Sets auto attack to enabled/disabled
    void setAutoAttack(bool isEnabled);

    // Sets auto release to enabled/disabled
    void setAutoRelease(bool isEnabled);

    // Gets current attack time constant
    double getAttack();

    // Gets current release time constant
    double getRelease();

    // Gets calculated attack coefficient
    double getAlphaAttack();

    // gets calculated release coefficient
    double getAlphaRelease();

    // Processes a sample with smooth branched peak detector
    float processPeakBranched(const float&);

    // Processes a sample with smooth decoupled peak detector
    float processPeakDecoupled(const float&);

    // Applies ballistics to given buffer
    void applyBallistics(float*, int);

    // Processes crest factor and sets ballistics accordingly
    void processCrestFactor(const float* src, int numSamples);

private:
    CrestFactor crestFactor;
    SmoothingFilter attackSmoothingFilter;
    SmoothingFilter releaseSmoothingFilter;

    double attackTimeInSeconds{0.01}, alphaAttack{0.0};
    double releaseTimeInSeconds{0.14}, alphaRelease{0.0};
    double state01{0.0}, state02{0.0};
    double sampleRate{0.0};
    bool autoAttack{false};
    bool autoRelease{false};
};
