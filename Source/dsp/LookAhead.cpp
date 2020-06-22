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
#include "include/LookAhead.h"
#include <algorithm>
#include "../JuceLibraryCode/JuceHeader.h"

/*Credits to Daniel Rudrich for the idea https://github.com/DanielRudrich/SimpleCompressor/blob/master/docs/lookAheadLimiter.md */
LookAhead::~LookAhead()
{
    rawBuffer = nullptr;
}

void LookAhead::prepare(double fs, double delay, int blockSize)
{
    delayInSamples = static_cast<int>(fs * delay);
    bufferSize = blockSize + delayInSamples;
    buffer.resize(bufferSize);
    std::fill(buffer.begin(), buffer.end(), 0.0f);
    rawBuffer = buffer.data();
}

void LookAhead::process(float* src, int numSamples)
{
    pushSamples(src, numSamples);
    processSamples();
    readSamples(src, numSamples);
}

void LookAhead::pushSamples(const float* src, const int numSamples)
{
    int index, b1, b2;

    // Write in delay line
    getWriteBlockSize(numSamples, index, b1, b2);

    FloatVectorOperations::copy(rawBuffer + index, src, b1);

    if (b2 > 0)
        FloatVectorOperations::copy(rawBuffer, src + b1, b2);

    writePosition += numSamples;
    writePosition = writePosition % bufferSize;

    numLastPushed = numSamples;
}

void LookAhead::readSamples(float* dst, int numSamples)
{
    int index, b1, b2;

    // Get starting index, size of block before and after wrap
    getReadBlockSize(numSamples, index, b1, b2);

    // Read from internal buffer to dst
    FloatVectorOperations::copy(dst, rawBuffer + index, b1);

    if (b2 > 0)
        FloatVectorOperations::copy(dst + b1, rawBuffer, b2);
}

void LookAhead::processSamples()
{
    int index = writePosition - 1;
    if (index < 0) index += bufferSize;

    int b1, b2;
    getProcessBlockSize(numLastPushed, index, b1, b2);

    float nextValue = 0.0f;
    float slope = 0.0f;

    // first run
    fadeIn(index, b1, slope, nextValue);

    // second run
    if (b2 > 0)
    {
        index = bufferSize - 1;
        fadeIn(index, b2, slope, nextValue);
    }

    if (index < 0) index = bufferSize - 1;

    // process delayInSamples
    getProcessBlockSize(delayInSamples, index, b1, b2);
    bool procMinimumFound = false;

    // first run
    fadeInWithBreak(index, b1, slope, nextValue, procMinimumFound);

    // second run
    if (!procMinimumFound && b2 > 0)
    {
        index = bufferSize - 1;
        fadeInWithBreak(index, b2, slope, nextValue, procMinimumFound);
    }
}

inline void LookAhead::fadeIn(int& index, int range, float& slope, float& nextValue)
{
    for (int i = 0; i < range; ++i)
    {
        const float sample = buffer[index];
        if (sample > nextValue)
        {
            buffer[index] = nextValue;
            nextValue += slope;
        }
        else
        {
            slope = -sample / static_cast<float>(delayInSamples);
            nextValue = sample + slope;
        }
        --index;
    }
}

inline void LookAhead::fadeInWithBreak(int& index, int range, float& slope, float& nextValue,
                                       bool& procMinimumFound)
{
    for (int i = 0; i < range; ++i)
    {
        const float sample = buffer[index];
        if (sample > nextValue)
        {
            buffer[index] = nextValue;
            nextValue += slope;
        }
        else
        {
            procMinimumFound = true;
            break;
        }
        --index;
    }
}

inline void LookAhead::getReadBlockSize(int numSamples, int& index, int& block1, int& block2)
{
    int pos = writePosition - numLastPushed - delayInSamples;

    if (pos < 0)
        pos = pos + bufferSize;
    pos = pos % bufferSize;

    index = pos;
    block1 = std::min(bufferSize - pos, numSamples);
    numSamples -= block1;
    block2 = numSamples > 0 ? numSamples : 0;
}

inline void LookAhead::getWriteBlockSize(int numSamples, int& index, int& block1, int& block2)
{
    int pos = writePosition;

    if (pos < 0)
        pos = pos + bufferSize;
    pos = pos % bufferSize;

    index = pos;
    block1 = std::min(bufferSize - pos, numSamples);
    numSamples -= block1;
    block2 = numSamples > 0 ? numSamples : 0;
}

inline void LookAhead::getProcessBlockSize(int numSamples, int index, int& block1, int& block2)
{
    block1 = std::min(index + 1, numSamples);
    numSamples -= block1;
    block2 = numSamples > 0 ? numSamples : 0;
}
