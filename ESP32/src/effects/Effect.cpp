#include "Effect.h"
#include "ArduinoJson.h"

const char *Effect::getName()
{
	return name;
}

void Effect::setEffectId(int id)
{
	effectId = id;
}

void Effect::effectHandler(CRGB *leds, int numberOfLeds)
{
}

void Effect::defineEffectConfigSetup(JsonObject *_effectConfigSetupJson)
{
}

bool Effect::getEffectHandled()
{
	return effectHandled;
}

bool Effect::getEffectGetsHandledOnce()
{
	return effectGetsHandledOnce;
}

void Effect::setEffectHandled(bool handled)
{
	effectHandled = handled;
}

int Effect::getEffectId()
{
	return effectId;
}

bool Effect::getEffectConfigSetupDefined()
{
	return false;
}