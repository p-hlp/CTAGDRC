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

class ModSlider : public Slider
{
public:


    ModSlider(Label* l) : label(l), isModifiable(false)
    {
    }

    ~ModSlider() { label = nullptr; }

    void mouseDown(const MouseEvent& e) override
    {
        const ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
        if (isModifiable && modifiers.isPopupMenu())
        {
            if (isEnabled() && label->isEnabled())
            {
                setEnabled(false);
                label->setEnabled(false);
            }
            else
            {
                setEnabled(true);
                label->setEnabled(true);
            }
        }
        else
            Slider::mouseDown(e);
    }

    void setHasModifiers(bool mod)
    {
        isModifiable = mod;
    }

    bool hasModifier() const
    {
        return isModifiable;
    }


private:
    Label* label;
    bool isModifiable;
};
