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
#include "MeterNeedle.h"
#include "MeterBackground.h"

/*Meter-Class:
 * A component that can display an analog-style meter
 */
class Meter : public Component
{
public:
    enum Mode { IN = 1, OUT, GR };

    Meter();
    void paint(Graphics& g) override;
    void resized() override;
    void setMode(int m);
    void modeBoxChanged();
    void update(const float& val);
    int getMode();
    float getValue();
    void setGUIEnabled(bool state);
private:
    MeterBackground meterBg;
    MeterNeedle needle;
    ComboBox modeBox;
    Colour backgroundDarkGrey;
    int meterMode;
    float valueInDecibel;
    float startAngle, endAngle;
};
