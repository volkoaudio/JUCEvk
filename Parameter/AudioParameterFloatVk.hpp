
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

#ifndef __AUDIOPARAMETERFLOATVK_HEADER_574658_
#define __AUDIOPARAMETERFLOATVK_HEADER_574658_

#include "../JuceLibraryCode/JuceHeader.h"

/**
    A subclass of AudioProcessorParameter that provides an easy way to create a
    parameter which maps onto a given NormalisableRange.

    @see AudioParameterInt, AudioParameterBool, AudioParameterChoice
*/
class JUCE_API  AudioParameterFloatVk  : public AudioProcessorParameterWithID
{
public:
    /** Creates a AudioParameterFloat with an ID, name, and range.
        On creation, its value is set to the default value.
    */
    AudioParameterFloatVk (String parameterID, String name,
                         NormalisableRange<float> normalisableRange,
                         float defaultValue);

    /** Creates a AudioParameterFloat with an ID, name, and range.
        On creation, its value is set to the default value.
        For control over skew factors, you can use the other
        constructor and provide a NormalisableRange.
    */
    AudioParameterFloatVk (String parameterID, String name,
                         float minValue,
                         float maxValue,
                         float defaultValue);
    
    /** Creates a AudioParameterFloat with an ID, name, range and lambda.
     On creation, its value is set to the default value.
     For control over skew factors, you can use the other
     constructor and provide a NormalisableRange.
     */
    AudioParameterFloatVk (String parameterID, String name,
                           float minValue,
                           float maxValue,
                           float defaultValue,
                           std::function<void(float)> setValueCallback);

    /** Destructor. */
    ~AudioParameterFloatVk();

    /** Returns the parameter's current value. */
    float get() const noexcept                  { return value; }
    /** Returns the parameter's current value. */
    operator float() const noexcept             { return value; }

    /** Changes the parameter's current value. */
    AudioParameterFloatVk& operator= (float newValue);

    /** Provides access to the parameter's range. */
    NormalisableRange<float> range;


protected:
    //==============================================================================
    float value, defaultValue;
    
    // VK
public:
    float getValue() const override;
    float getDefaultValue() const override;
    void setValue (float newValue) override;
    int getNumSteps() const override;
    String getText (float, int) const override;
    float getValueForText (const String&) const override;

    void setSetValueCallback(std::function<void(float)> setValueCallback)
    {
        _setValueCallback = setValueCallback;
    }
    
protected:
    std::function<void(float)> _setValueCallback;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioParameterFloatVk)
};

inline AudioParameterFloatVk::AudioParameterFloatVk (String pid, String nm, NormalisableRange<float> r, float def)
   : AudioProcessorParameterWithID (pid, nm), range (r), value (def), defaultValue (def), _setValueCallback(nullptr)
{
}

inline AudioParameterFloatVk::AudioParameterFloatVk (String pid, String nm, float minValue, float maxValue, float def)
   : AudioProcessorParameterWithID (pid, nm), range (minValue, maxValue), value (def), defaultValue (def), _setValueCallback(nullptr)
{
}

inline AudioParameterFloatVk::AudioParameterFloatVk (String pid, String nm, float minValue, float maxValue, float def, std::function<void(float)> setValueCallback)
: AudioProcessorParameterWithID (pid, nm)
, range (minValue, maxValue)
, value (def)
, defaultValue (def)
, _setValueCallback(setValueCallback)
{
}

inline AudioParameterFloatVk::~AudioParameterFloatVk()
{
}

inline float AudioParameterFloatVk::getValue() const                              
{ 
    return range.convertTo0to1 (value); 
}

inline void AudioParameterFloatVk::setValue (float newValue)                      
{ 
    value = range.convertFrom0to1 (newValue);
    if (_setValueCallback)
        _setValueCallback(newValue);
}

inline float AudioParameterFloatVk::getDefaultValue() const                       
{ 
    return range.convertTo0to1 (defaultValue); 
}

inline int AudioParameterFloatVk::getNumSteps() const                             
{ 
    return AudioProcessorParameterWithID::getNumSteps(); 
}

inline float AudioParameterFloatVk::getValueForText (const String& text) const    
{ 
    return range.convertTo0to1 (text.getFloatValue()); 
}

inline String AudioParameterFloatVk::getText (float v, int length) const          
{ 
    return String (range.convertFrom0to1 (v), 2).substring (0, length); 
}

inline AudioParameterFloatVk& AudioParameterFloatVk::operator= (float newValue)
{
    const float normalisedValue = range.convertTo0to1 (newValue);

    if (value != normalisedValue)
        setValueNotifyingHost (normalisedValue);

    return *this;
}

#endif // __AUDIOPARAMETERFLOATVK_HEADER_574658_
