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
#include "../JuceLibraryCode/JuceHeader.h"

namespace SIMDMath
{
    inline float sum(const float* src, int num)
    {
        if (SystemStats::hasSSE3())
        {
            __m128 mmSum = _mm_setzero_ps();

            int i = 0;
            int rounddown = num - (num % 16);

            for (; i < rounddown; i += 16)
            {
                __m128 v0 = _mm_loadu_ps(src + i + 0);
                __m128 v1 = _mm_loadu_ps(src + i + 4);
                __m128 s01 = _mm_add_ps(v0, v1);

                __m128 v2 = _mm_loadu_ps(src + i + 8);
                __m128 v3 = _mm_loadu_ps(src + i + 12);
                __m128 s23 = _mm_add_ps(v2, v3);

                mmSum = _mm_add_ps(mmSum, s01);
                mmSum = _mm_add_ps(mmSum, s23);
            }

            for (; i < num; i++)
                mmSum = _mm_add_ss(mmSum, _mm_load_ss(src + i));

            mmSum = _mm_hadd_ps(mmSum, mmSum);
            mmSum = _mm_hadd_ps(mmSum, mmSum);
            return _mm_cvtss_f32(mmSum);
        }

        // Fallback
        float s = 0;
        const float* e = src + num;

        while (src != e)
        {
            s += *src;
            src++;
        }
        return s;
    }
}
