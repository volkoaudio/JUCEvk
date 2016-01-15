
/*
 ==============================================================================
 
 This file is part of the JUCE library.
 Copyright (c) 2013 - Raw Material Software Ltd.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses JUCE, commercial licenses are
 available: visit www.juce.com for more information.
 
 ==============================================================================
 */

#ifndef __AUDIOPARAMETERINTVK_HEADER_038746_
#define __AUDIOPARAMETERINTVK_HEADER_038746_

#include "../JuceLibraryCode/JuceHeader.h"

class JUCE_API  AudioParameterIntVk  : public AudioProcessorParameterWithID
{
public:
    /** Creates an AudioParameterInt with an ID, name, and range.
     Note that the min and max range values are inclusive.
     On creation, its value is set to the default value.
     */
    AudioParameterIntVk (String parameterID, String name,
                       int minValue, int maxValue,
                       int defaultValue);

    /** Creates an AudioParameterInt with an ID, name, range and lambda.
     Note that the min and max range values are inclusive.
     On creation, its value is set to the default value.
     */
    AudioParameterIntVk (String parameterID, String name,
                         int minValue, int maxValue,
                         int defaultValue,
                         std::function<void(float)> setValueCallback
                         );

    /** Destructor. */
    ~AudioParameterIntVk();
    
    /** Returns the parameter's current value as an integer. */
    int get() const noexcept                    { return roundToInt (value); }
    /** Returns the parameter's current value as an integer. */
    operator int() const noexcept               { return get(); }
    
    /** Changes the parameter's current value to a new integer.
     The value passed in will be snapped to the permitted range before being used.
     */
    AudioParameterIntVk& operator= (int newValue);
    
    /** Returns the parameter's range. */
    Range<int> getRange() const noexcept        { return Range<int> (minValue, maxValue); }
    
    
protected:
    //==============================================================================
    int minValue, maxValue;
    float value, defaultValue;
    
    int limitRange (int) const noexcept;
    float convertTo0to1 (int) const noexcept;
    int convertFrom0to1 (float) const noexcept;
    
    // VK
public:
    float getValue() const override;
    void setValue (float newValue) override;
    float getDefaultValue() const override;
    int getNumSteps() const override;
    String getText (float, int) const override;
    float getValueForText (const String&) const override;

    void setSetValueCallback(std::function<void(float)> setValueCallback)
    {
        _setValueCallback = setValueCallback;
    }
    
protected:
    std::function<void(float)> _setValueCallback;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioParameterIntVk)
};


inline AudioParameterIntVk::AudioParameterIntVk (String pid, String nm, int mn, int mx, int def)
: AudioProcessorParameterWithID (pid, nm)
, minValue (mn), maxValue (mx)
, value ((float) def)
, defaultValue (convertTo0to1 (def))
, _setValueCallback(nullptr)
{
    jassert (minValue < maxValue); // must have a non-zero range of values!
}

inline AudioParameterIntVk::AudioParameterIntVk (String pid, String nm, int mn, int mx, int def
                                          , std::function<void(float)> setValueCallback)
: AudioProcessorParameterWithID (pid, nm)
, minValue (mn), maxValue (mx)
, value ((float) def)
, defaultValue (convertTo0to1 (def))
, _setValueCallback(setValueCallback)
{
    jassert (minValue < maxValue); // must have a non-zero range of values!
}

inline AudioParameterIntVk::~AudioParameterIntVk() {}

inline int AudioParameterIntVk::limitRange (int v) const noexcept
{
    return jlimit (minValue, maxValue, v);
}

inline float AudioParameterIntVk::convertTo0to1 (int v) const noexcept
{
    return (limitRange (v) - minValue) / (float) (maxValue - minValue);
}

inline int AudioParameterIntVk::convertFrom0to1 (float v) const noexcept
{
    return limitRange (roundToInt ((v * (float) (maxValue - minValue)) + minValue));
}

inline float AudioParameterIntVk::getValue() const
{
    return convertTo0to1 (roundToInt (value));
}

inline void AudioParameterIntVk::setValue (float newValue)
{
    value = (float) convertFrom0to1 (newValue);
    if (_setValueCallback)
        _setValueCallback(newValue);
}

inline float AudioParameterIntVk::getDefaultValue() const
{
    return defaultValue;
}

inline int AudioParameterIntVk::getNumSteps() const
{
    return AudioProcessorParameterWithID::getNumSteps();
}

inline float AudioParameterIntVk::getValueForText (const String& text) const
{
    return convertTo0to1 (text.getIntValue());
}

inline String AudioParameterIntVk::getText (float v, int /*length*/) const
{
    return String (convertFrom0to1 (v));
}

inline AudioParameterIntVk& AudioParameterIntVk::operator= (int newValue)
{
    const float normalisedValue = convertTo0to1 (newValue);
    
    if (value != normalisedValue)
        setValueNotifyingHost (normalisedValue);
    
    return *this;
}

#endif // __AUDIOPARAMETERINTVK_HEADER_038746_
