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
#include "ModSlider.h"

using SliderAttachment = std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>;
using ButtonAttachment = std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>;

class LabeledSlider : public Component
{
public:

    LabeledSlider() : slider(&sliderLabel)
    {
        addAndMakeVisible(slider);
        slider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

        addAndMakeVisible(sliderLabel);
        sliderLabel.setJustificationType(Justification::centred);
    }

    LabeledSlider(Component* parent) : slider(&sliderLabel)
    {
        addAndMakeVisible(slider);
        slider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        slider.setPopupDisplayEnabled(true, true, parent, 0);
        slider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

        addAndMakeVisible(sliderLabel);
        sliderLabel.setJustificationType(Justification::centred);
    }

    void reset(AudioProcessorValueTreeState& state, const String& paramID)
    {
        sAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(state, paramID, slider));
    }

    void setLabelText(const String& text)
    {
        sliderLabel.setText(text, dontSendNotification);
    }


    void resized() override
    {
        auto bounds = getLocalBounds().reduced(2);
        const auto fontHeight = sliderLabel.getFont().getHeight();

        const auto labelBounds = bounds.removeFromBottom(fontHeight);
        slider.setBounds(bounds);
        sliderLabel.setBounds(labelBounds);
    }

    void setHasModifier(bool mod) { slider.setHasModifiers(mod); }
    bool hasModifier() const { return slider.hasModifier(); }

private:
    ModSlider slider;
    Label sliderLabel;
    SliderAttachment sAttachment;
    ButtonAttachment bAttachment;
};
