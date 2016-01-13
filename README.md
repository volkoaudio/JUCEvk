# JUCEvk
Some classes for JUCE library

I added three AudioParameter* classes because in Juce4.1 some of theirs members are private. That means I can't use these private members with inheritance. Normally I expected they are protected. 

I made private members protected and add std::function<void(float)> member. 

Example: 

In PluginProcessor.h

AudioParameterFloatVk *_paramFloat;

In PluginProcessor.cpp

auto lambdaParamFloat = [this] (float paramValue)
{
Logger::getCurrentLogger()->writeToLog(String("ParamFloat: ") + String(paramValue));
};

addParameter( _paramFloat = new AudioParameterFloatVk( "paramfloat", "Parameter Float", -100.f, 100.f, 0.f, lambdaParamFloat ) );

Then when you call setValueNotifyingHost(), lambdaParamFloat will also call. 
