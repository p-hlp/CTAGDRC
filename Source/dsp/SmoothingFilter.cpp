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
#include "include/SmoothingFilter.h"
#include "../JuceLibraryCode/JuceHeader.h"

void SmoothingFilter::prepare(const double& fs)
{
    sampleRate = fs;
    a1 = 1;
    b1 = 1 - a1;
}

void SmoothingFilter::process(const double& sample)
{
    if (first)
    {
        state = sample;
        first = false;
    }
    state = a1 * sample + b1 * state;
}

double SmoothingFilter::getState()
{
    return state;
}

void SmoothingFilter::setAlpha(double a)
{
    a1 = a;
    b1 = 1 - a1;
}

void SmoothingFilter::setAlphaWithTime(float timeInSeconds)
{
    a1 = exp(-1.0 / (sampleRate * timeInSeconds));
    b1 = 1 - a1;
}
