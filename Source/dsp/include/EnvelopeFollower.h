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

/*Basic sample-wise envenlope follower with instant attack*/
class EnvelopeFollower
{
public:

    EnvelopeFollower() = default;

    // Prepares the the envelope follower with given sample rate
    void prepare(const double& fs);

    // Sets decay/release time constant
    void setDecay(float dcInSeconds);

    // Updates envelope follower, use buffer that contains linear values i.e. 0.0-1.0
    float update(float sample);

    // Updates envelope follower, use when buffer containts logarithmic values i.e. -infdB - 0dB
    float updateLog(float sample);

private:
    double decayInSecods{0.0};
    int decayInSamples{0};
    double a{1.0};
    double sampleRate{0.0};
    double state{0.0};
};
