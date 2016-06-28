
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

#ifndef __AUDIOPARAMETERBOOLVK_HEADER_83658734_
#define __AUDIOPARAMETERBOOLVK_HEADER_83658734_

#include "../JuceLibraryCode/JuceHeader.h"
#include <atomic>

class JUCE_API AudioParameterBoolVk  : public AudioProcessorParameterWithID
{
public:
    /** Creates a AudioParameterBool with an ID and name.
     On creation, its value is set to the default value.
     */
    AudioParameterBoolVk (String parameterID, String name, bool defaultValue);
    
    /** Creates a AudioParameterBool with an ID, name and lambda.
     On creation, its value is set to the default value.
     */
    AudioParameterBoolVk (String parameterID,
                          String name,
                          bool defaultValue,
                          std::function<void(float, String)> setValueCallback);
    
    
    /** Destructor. */
    ~AudioParameterBoolVk();
    
    /** Returns the parameter's current boolean value. */
    bool get() const noexcept          { return value >= 0.5f; }
    /** Returns the parameter's current boolean value. */
    operator bool() const noexcept     { return get(); }
    
    /** Changes the parameter's current value to a new boolean. */
    AudioParameterBoolVk& operator= (bool newValue);
    
    inline void resetToDefaultValue();
    inline bool isDefaultValue();
    
protected:
    //==============================================================================
    std::atomic<float> value, defaultValue;
    
    float getValue() const override;
    void setValue (float newValue) override;
    float getDefaultValue() const override;
    int getNumSteps() const override;
    String getText (float, int) const override;
    float getValueForText (const String&) const override;
    
protected:
    void setSetValueCallback(std::function<void(float, String)> setValueCallback)
    {
        _setValueCallback = setValueCallback;
    }
    
protected:
    std::function<void(float, String)> _setValueCallback;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioParameterBoolVk)
};

inline AudioParameterBoolVk::AudioParameterBoolVk (String pid, String nm, bool def)
: AudioProcessorParameterWithID (pid, nm)
, value (def ? 1.0f : 0.0f)
, defaultValue (float(value))
, _setValueCallback(nullptr)
{
}

inline AudioParameterBoolVk::AudioParameterBoolVk (String pid, String nm,
                                                   bool def, std::function<void(float, String)> setValueCallback)
: AudioProcessorParameterWithID (pid, nm)
, value (def ? 1.0f : 0.0f)
, defaultValue (float(value))
, _setValueCallback(setValueCallback)
{
}

inline AudioParameterBoolVk::~AudioParameterBoolVk() {}

inline float AudioParameterBoolVk::getValue() const
{
    return value;
}

inline void AudioParameterBoolVk::setValue (float newValue)
{
    value = newValue;
    if (_setValueCallback)
        _setValueCallback(value, paramID);
}

inline float AudioParameterBoolVk::getDefaultValue() const
{
    return defaultValue;
}

inline int AudioParameterBoolVk::getNumSteps() const
{
    return 2;
}

inline float AudioParameterBoolVk::getValueForText (const String& text) const
{
    return text.getIntValue() != 0 ? 1.0f : 0.0f;
}

inline String AudioParameterBoolVk::getText (float v, int /*length*/) const
{
    return String ((int) (v > 0.5f ? 1 : 0));
}

inline AudioParameterBoolVk& AudioParameterBoolVk::operator= (bool newValue)
{
    const float normalisedValue = newValue ? 1.0f : 0.0f;
    
    if (value != normalisedValue)
        setValueNotifyingHost (normalisedValue);
    
    return *this;
}

inline void AudioParameterBoolVk::resetToDefaultValue()
{
    *this = defaultValue >= 0.5f;
}

inline bool AudioParameterBoolVk::isDefaultValue()
{
    return fabs(getDefaultValue() - getValue()) < std::numeric_limits<float>::epsilon();
}

#endif // __AUDIOPARAMETERBOOLVK_HEADER_83658734_