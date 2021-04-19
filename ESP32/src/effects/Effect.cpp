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
	switchObject["value"] = state;
}

void Effect::changeEffectConfigValue(DynamicJsonDocument doc)
{
	JsonArray _selectsJsonArr = doc["selects"];
	JsonArray _switchesJsonArr = doc["switches"];

	//Selects
	updateEffectConfigValue(_selectsJsonArr, selectsJsonArr);

	//Switches
	updateEffectConfigValue(_switchesJsonArr, switchesJsonArr);
}

void Effect::updateEffectConfigValue(JsonArray arrayWithNewValues, JsonArray currentArray)
{
	for (JsonObject newObj : arrayWithNewValues) {
		//!!!!!Should only run once - a select can only send/have a single value so the first key is the value!!!!!
		for (JsonPair kv : newObj) {
			const char *key = kv.key().c_str();
			JsonVariant value = kv.value();

			for (JsonObject currentObj : currentArray) {
				const char *currentKey = currentObj["name"];
				if (strcmp(key, currentKey) == 0) {
					currentObj["value"] = value;
				}
			}
		}
	}
}

bool Effect::getSwitchState(const char *_name)
{
	return (bool) getConfigFieldValue(_name, switchesJsonArr);
}

JsonVariant Effect::getSelectValue(const char *_name)
{
	return getConfigFieldValue(_name, selectsJsonArr);
}

JsonVariant Effect::getConfigFieldValue(const char *_name, JsonArray jsonArr)
{
	for(JsonObject obj : jsonArr) {
		const char *fieldName = obj["name"];
		if(strcmp(fieldName, _name) == 0) {
			return obj["value"];
		}
	}
	return ArduinoJson::JsonVariant();
}
