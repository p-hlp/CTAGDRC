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

class MeterNeedle : public Component
{
public:

    MeterNeedle();
    void prepare(const float& s, const float& e);
    void paint(Graphics& g) override;
    void resized() override;
    void update(const float& val);
    void redrawNeedle(Graphics& g, float centreX, float centreY, float length);
    void setMode(int m);
    void setNeedleEnabled(bool state);
private:
    Rectangle<int> area;
    Colour needleColour;
    float valueInDecibel;
    int mode;
    int minValue, maxValue;
    float sAngle, eAngle;
};
