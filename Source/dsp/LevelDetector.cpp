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
#include "include/LevelDetector.h"
#include "../JuceLibraryCode/JuceHeader.h"

void LevelDetector::prepare(const double& fs)
{
    sampleRate = fs;
    crestFactor.prepare(fs);
    attackSmoothingFilter.prepare(fs);
    releaseSmoothingFilter.prepare(fs);

    alphaAttack = exp(-1.0 / (sampleRate * attackTimeInSeconds));
    alphaRelease = exp(-1.0 / (sampleRate * releaseTimeInSeconds));
    state01 = 0.0;
    state02 = 0.0;
}

void LevelDetector::setAttack(const double& attack)
{
    if (attack != attackTimeInSeconds)
    {
        attackTimeInSeconds = attack; //Time it takes to reach 1-1/e = 0.63
        alphaAttack = exp(-1.0 / (sampleRate * attackTimeInSeconds)); //aA = e^(-1/TA*fs)
    }
}

void LevelDetector::setRelease(const double& release)
{
    if (release != releaseTimeInSeconds)
    {
        releaseTimeInSeconds = release; //Time it takes to reach 1 - (1-1/e) = 0.37
        alphaRelease = exp(-1.0 / (sampleRate * releaseTimeInSeconds)); //aR = e^(-1/TR*fs)
    }
}

void LevelDetector::setAutoAttack(bool isEnabled)
{
    autoAttack = isEnabled;
}

void LevelDetector::setAutoRelease(bool isEnabled)
{
    autoRelease = isEnabled;
}

double LevelDetector::getAttack()
{
    return attackTimeInSeconds;
}

double LevelDetector::getRelease()
{
    return releaseTimeInSeconds;
}

double LevelDetector::getAlphaAttack()
{
    return alphaAttack;
}

double LevelDetector::getAlphaRelease()
{
    return alphaRelease;
}

float LevelDetector::processPeakBranched(const float& in)
{
    //Smooth branched peak detector
    if (in < state01)
        state01 = alphaAttack * state01 + (1 - alphaAttack) * in;
    else
        state01 = alphaRelease * state01 + (1 - alphaRelease) * in;

    return static_cast<float>(state01); //y_L
}


float LevelDetector::processPeakDecoupled(const float& in)
{
    //Smooth decoupled peak detector
    const double input = static_cast<double>(in);
    state02 = jmax(input, alphaRelease * state02 + (1 - alphaRelease) * input);
    state01 = alphaAttack * state01 + (1 - alphaAttack) * state02;
    return static_cast<float>(state01);
}

void LevelDetector::applyBallistics(float* src, int numSamples)
{
    // Apply ballistics to src buffer
    for (int i = 0; i < numSamples; ++i)
        src[i] = processPeakBranched(src[i]);
}

void LevelDetector::processCrestFactor(const float* src, int numSamples)
{
    if (autoAttack || autoRelease)
    {
        //Crest factor calculation
        crestFactor.process(src, numSamples);
        attackSmoothingFilter.process(crestFactor.getAvgAttack());
        releaseSmoothingFilter.process(crestFactor.getAvgRelease());
        if (autoAttack) setAttack(attackSmoothingFilter.getState());
        if (autoRelease) setRelease(releaseSmoothingFilter.getState());
    }
}
