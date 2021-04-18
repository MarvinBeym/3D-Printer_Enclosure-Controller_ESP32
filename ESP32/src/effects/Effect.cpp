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

void Effect::defineEffectConfig()
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

bool Effect::hasEffectConfig()
{
	return false;
}

void Effect::setupEffectConfig(DynamicJsonDocument *doc)
{
	effectConfig = doc->createNestedObject(name);
	selectsJsonArr = effectConfig.createNestedArray("selects");
	switchesJsonArr = effectConfig.createNestedArray("switches");
	defineEffectConfig();
}

JsonArray Effect::addSelect(const char *_name, const char *label, int value)
{
	JsonObject selectObject = selectsJsonArr.createNestedObject();
	selectObject["value"] = value;
	selectObject["label"] = label;
	selectObject["name"] = _name;
	JsonArray options = selectObject.createNestedArray("options");
	return options;
}

JsonArray Effect::addSelect(const char *_name, const char *label, char *value)
{
	JsonObject selectObject = selectsJsonArr.createNestedObject();
	selectObject["name"] = _name;
	selectObject["label"] = label;
	selectObject["value"] = value;
	JsonArray options = selectObject.createNestedArray("options");
	return options;
}

void Effect::addSwitch(const char *_name, const char *label, bool state)
{
	JsonObject switchObject = switchesJsonArr.createNestedObject();
	switchObject["name"] = _name;
	switchObject["label"] = label;
	switchObject["state"] = state;
}
