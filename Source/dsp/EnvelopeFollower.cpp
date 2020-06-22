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
#include "include/EnvelopeFollower.h"

void EnvelopeFollower::prepare(const double& fs)
{
    sampleRate = fs;
    decayInSamples = static_cast<int>(sampleRate * decayInSecods);
}

void EnvelopeFollower::setDecay(float dcInSeconds)
{
    decayInSecods = dcInSeconds;
    prepare(sampleRate);
}

float EnvelopeFollower::update(float sample)
{
    const double s = static_cast<double>(sample);
    if (s > state) state = s;
    else state *= a;
    return static_cast<float>(state);
}

float EnvelopeFollower::updateLog(float sample)
{
    const double s = static_cast<double>(sample);

    if (s < state) state = s; //attack phase	
    else state *= a; //release phase

    return static_cast<float>(state);
}
