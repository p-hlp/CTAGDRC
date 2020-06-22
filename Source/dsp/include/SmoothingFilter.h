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

/*Simple exponential moving average filter, also known as 1-pole iir filter
 * This class can be used to smooth values over a certain time frame
 */
class SmoothingFilter
{
public:

    SmoothingFilter() = default;

    // Prepares the SmoothingFilter with a sampleRate
    void prepare(const double& fs);

    // Processes a given sample
    void process(const double& sample);

    // Sets coefficient manually
    void setAlpha(double a);

    // Set time-frame in seconds, recalculates needed coefficients
    void setAlphaWithTime(float timeInSeconds);

    // Gets current value
    double getState();

private:
    double a1{1.0}, b1{0.0};
    double state{0.0};
    double sampleRate{0.0};
    bool first{true};
};
