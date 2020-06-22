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

/* GainComputer Class:
 * Calculates the needed attenuation to compress a signal with given characteristics
 */
class GainComputer
{
public:

    GainComputer();

    // Sets the threshold in dB
    void setThreshold(float);

    // Sets the ratio in dB
    void setRatio(float);

    // Sets the knee-width in dB (if > 0, 2nd order interpolation for soft knee)
    void setKnee(float);

    // Applies characteristics to a given sample
    // returns attenuation
    float applyCompression(float&);

    void applyCompressionToBuffer(float*, int);

private:
    float threshold{-20.0f};
    float ratio{2.0f};
    float knee{6.0f}, kneeHalf{3.0f};
    float slope{-0.5f};
};
