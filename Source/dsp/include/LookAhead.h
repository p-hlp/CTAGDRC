/*
Copyright (C) 2020 Phillip Lamp
Copyright (C) 2019 Daniel Rudrich
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
#include <vector>

/*Credits to Daniel Rudrich for the idea https://github.com/DanielRudrich/SimpleCompressor/blob/master/docs/lookAheadLimiter.md */
/*LookAhead-Class:
 *A LookAhead implementation that not only delays the input signal, but also fades in aggressive gain reduction values to avoid distortion*/
class LookAhead
{
public:
    ~LookAhead();
    void prepare(double fs, double delay, int blockSize);
    void process(float* src, int numSamples);

private:
    void pushSamples(const float* src, int numSamples);
    void readSamples(float* dst, int numSamples);
    void processSamples();

    inline void fadeIn(int& index, int range, float& slope, float& nextValue);
    inline void fadeInWithBreak(int& index, int range, float& slope, float& nextValue, bool& procMinimumFound);

    inline void getReadBlockSize(int numSamples, int& index, int& block1, int& block2);
    inline void getWriteBlockSize(int numSamples, int& index, int& block1, int& block2);
    inline void getProcessBlockSize(int numSamples, int index, int& block1, int& block2);
private:
    std::vector<float> buffer;
    float* rawBuffer{nullptr};

    int delayInSamples{0};
    int bufferSize{0};
    int writePosition{0};
    int numLastPushed{0};
};
