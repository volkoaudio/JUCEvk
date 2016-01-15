# JUCEvk
Some classes for JUCE library

I added three AudioParameter* classes because in Juce4.1 some of theirs members are private. That means I can't use these private members with inheritance. Normally I expected some of them would be protected and some of them would be public. 

I made private members protected and public and add std::function<void(float)> member. 

Example: 

In PluginProcessor.h

<code>
AudioParameterFloatVk *_paramFloat;
</code>

In PluginProcessor.cpp

<code>
auto lambdaParamFloat = [this] (float paramValue)
{
Logger::getCurrentLogger()->writeToLog(String("ParamFloat: ") + String(paramValue));
};

addParameter( _paramFloat = new AudioParameterFloatVk( "paramfloat", "Parameter Float", -100.f, 100.f, 0.f, lambdaParamFloat ) );
</code>

Then when you call setValueNotifyingHost(), lambdaParamFloat will also be called. 
