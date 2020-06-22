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
#include "include/CrestFactor.h"
#include "../JuceLibraryCode/JuceHeader.h"

void CrestFactor::prepare(const double& fs)
{
    sampleRate = fs;
    //Calculate alpha for release time of 200ms, same release time for peak & rms detector
    a1 = exp(-1.0 / (sampleRate * 0.2));
    b1 = 1 - a1;
}


void CrestFactor::
process(const float* src, const int numSamples)
{
    //Init accumulators
    if (!peakState) peakState = src[0];
    if (!rmsState) rmsState = src[0];

    //Reset avg attack/release
    avgAttackTime = 0.0;
    avgReleaseTime = 0.0;

    //Calculate averages of auto - attack/release times for a single buffer
    for (int i = 0; i < numSamples; ++i)
    {
        //Square of input signal
        const double s = static_cast<double>(src[i]) * static_cast<double>(src[i]);

        //Update peak state
        peakState = jmax(s, a1 * peakState + b1 * s);

        //Update rms state
        rmsState = a1 * rmsState + b1 * s;

        //calculate squared crest factor
        const double c = peakState / rmsState;
        cFactor = c > 0.0 ? c : 0.0;

        //calculate ballistics
        if (cFactor > 0.0)
        {
            attackTimeInSeconds = 2 * (maxAttackTime / cFactor);
            releaseTimeInSeconds = 2 * (maxReleaseTime / cFactor) - attackTimeInSeconds;

            //Update avg ballistics
            avgAttackTime += attackTimeInSeconds;
            avgReleaseTime += releaseTimeInSeconds;
        }
    }

    // Calculate average ballistics & crest factor
    avgAttackTime /= numSamples;
    avgReleaseTime /= numSamples;
}

double CrestFactor::getAvgAttack()
{
    return avgAttackTime;
}

double CrestFactor::getAvgRelease()
{
    return avgReleaseTime;
}
