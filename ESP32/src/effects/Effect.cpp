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

void Effect::setupEffectConfig(DynamicJsonDocument *doc)
{
	effectConfig = doc->createNestedObject(name);
	selectFields = effectConfig.createNestedArray("selects");
	switchFields = effectConfig.createNestedArray("switches");
	numberFields = effectConfig.createNestedArray("numbers");
	defineEffectConfig();
}

JsonArray Effect::addSelectField(const char *_name, const char *label, int value)
{
	JsonObject selectField = selectFields.createNestedObject();
	selectField["value"] = value;
	selectField["label"] = label;
	selectField["name"] = _name;
	JsonArray options = selectField.createNestedArray("options");
	return options;
}

JsonArray Effect::addSelectField(const char *_name, const char *label, char *value)
{
	JsonObject selectField = selectFields.createNestedObject();
	selectField["name"] = _name;
	selectField["label"] = label;
	selectField["value"] = value;
	JsonArray options = selectField.createNestedArray("options");
	return options;
}

void Effect::addSwitchField(const char *_name, const char *label, bool state)
{
	JsonObject switchField = switchFields.createNestedObject();
	switchField["name"] = _name;
	switchField["label"] = label;
	switchField["value"] = state;
}

void Effect::addNumberField(const char *_name, const char *label, int value, int min, int max)
{
	if (value <= min) {
		value = (float) min;
	}
	if (value >= max) {
		value = (float) max;
	}

	JsonObject numberField = numberFields.createNestedObject();
	numberField["name"] = _name;
	numberField["label"] = label;
	numberField["value"] = value;
	numberField["min"] = min;
	numberField["max"] = max;
}

void Effect::changeEffectFieldValue(DynamicJsonDocument doc)
{
	JsonArray _selectFields = doc["selects"];
	JsonArray _switchFields = doc["switches"];
	JsonArray _numberFields = doc["numbers"];

	//Select fields
	updateEffectFieldValue(_selectFields, selectFields);

	//Switch fields
	updateEffectFieldValue(_switchFields, switchFields);

	//Number fields
	updateEffectFieldValue(_numberFields, numberFields);

}

void Effect::updateEffectFieldValue(JsonArray arrayWithNewValues, JsonArray currentArray)
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

bool Effect::getSwitchFieldState(const char *_name)
{
	return (bool) getConfigFieldValue(_name, switchFields);
}

JsonVariant Effect::getSelectFieldValue(const char *_name)
{
	return getConfigFieldValue(_name, selectFields);
}

JsonVariant Effect::getNumberFieldValue(const char *_name)
{
	return getConfigFieldValue(_name, numberFields);
}

JsonVariant Effect::getConfigFieldValue(const char *_name, JsonArray fields)
{
	for (JsonObject obj : fields) {
		const char *fieldName = obj["name"];
		if (strcmp(fieldName, _name) == 0) {
			return obj["value"];
		}
	}
	return ArduinoJson::JsonVariant();
}

bool Effect::getHasEffectConfig()
{
	return hasEffectConfig;
}
