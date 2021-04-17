#include "EffectLoader.h"
#include "ArduinoJson.h"
#include "controller-configuration.h"

void EffectLoader::setupEffects()
{
	effectsConfigSetupDoc = new DynamicJsonDocument(effects_configSetup_dynamicJsonDocument_size);
	//effectsConfigSetupDoc
	for (std::size_t i = 0; i < effects.size(); ++i) {
		effects[i]->setEffectId(i);
		if (effects[i]->getEffectConfigSetupDefined()) {
			JsonObject effectConfigJson = effectsConfigSetupDoc->createNestedObject(effects[i]->getName());
			effects[i]->defineEffectConfigSetup(&effectConfigJson);
		}
	}

	xTaskCreate(&taskHandler, "effectLoader", 5000, this, 1, nullptr);

	xTaskCreate(
			effectChangeCallback,
			"effectChangeCallback",
			2000,
			(void *) &currentEffect,
			1,
			nullptr
	);
}

//A wrapper static function to allow creation of tasks inside the class
void EffectLoader::taskHandler(void *parameter)
{
	EffectLoader *effectLoader = reinterpret_cast<EffectLoader *>(parameter);
	for (;;) {
		effectLoader->taskRunner();
	}
}

//Runs the actual task code.
void EffectLoader::taskRunner()
{
	auto effect = effects[currentEffect];
	if (!effect->getEffectGetsHandledOnce()) {
		effect->effectHandler(led2->leds, led2->numberOfLeds);
	} else if (effect->getEffectGetsHandledOnce() && !effect->getEffectHandled()) {
		effect->setEffectHandled(true);
		effect->effectHandler(led2->leds, led2->numberOfLeds);
		vTaskDelay(pdMS_TO_TICKS(200));
	}
	return;
}

const char *EffectLoader::getEffectName(int effectId, bool upperCase)
{
	String *name = new String(effects[effectId]->getName());
	if (upperCase) {
		name->toUpperCase();
		return name->c_str();
	}
	return name->c_str();
}

void EffectLoader::changeEffect(int newEffectId)
{
	if (newEffectId <= 0) {
		currentEffect = 0;
	} else if (newEffectId >= effects.size() - 1) {
		currentEffect = effects.size() - 1;
	} else {
		currentEffect = newEffectId;
	}
	for (std::size_t i = 0; i < effects.size(); ++i) {
		if (effects[i]->getEffectGetsHandledOnce()) {
			effects[i]->setEffectHandled(false);
		}
	}
	xEventGroupSetBits(eg, effectChangeEvent);
}

int EffectLoader::getCurrentEffect()
{
	return currentEffect;
}

void EffectLoader::addToJson(DynamicJsonDocument *doc, bool includeCurrentEffect, bool includeEffects)
{
	JsonObject jsonObject = doc->createNestedObject("led2");
	if (includeCurrentEffect) {
		jsonObject["currentEffect"] = getCurrentEffect();
	}


	if (includeEffects) {
		JsonArray jsonArrEffects = jsonObject.createNestedArray("effects");
		DynamicJsonDocument tmpEffectsConfigSetupDoc = *effectsConfigSetupDoc;

		for (std::size_t i = 0; i < effects.size(); ++i) {
			auto effect = effects[i];
			JsonObject effectObject = jsonArrEffects.createNestedObject();
			effectObject["name"] = effect->getName();
			effectObject["id"] = effect->getEffectId();

			JsonObject configSetupObject = effectObject.createNestedObject("configSetup");

			if(effect->getEffectConfigSetupDefined()) {
				configSetupObject.set(tmpEffectsConfigSetupDoc[effect->getName()]);
			}
		}
	}

}
