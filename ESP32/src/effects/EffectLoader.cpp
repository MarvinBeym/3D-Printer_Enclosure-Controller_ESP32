#include "EffectLoader.h"

void EffectLoader::setupEffects()
{
	for (std::size_t i = 0; i < effects.size(); ++i) {
		effects[i]->setEffectId(i + 1);
	}
	xTaskCreate(&EffectLoader::taskHandler, "effectLaoder", 10000, this, 1, nullptr);
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
	auto effect = effects[currentEffect - 1];
	if(!effect->getEffectGetsHandledOnce()) {
		effect->effectHandler(led2->leds, led2->numberOfLeds);
	} else if(effect->getEffectGetsHandledOnce() && !effect->getEffectHandled()){
		effect->setEffectHandled(true);
		effect->effectHandler(led2->leds, led2->numberOfLeds);
		vTaskDelay(pdMS_TO_TICKS(200));
	}
	return;
}

void EffectLoader::changeEffect(int newEffectId)
{
	currentEffect = newEffectId;
	for (std::size_t i = 0; i < effects.size(); ++i) {
		if(effects[i]->getEffectGetsHandledOnce()) {
			effects[i]->setEffectHandled(false);
		}
	}
}

void EffectLoader::addToJson(DynamicJsonDocument *doc) {
	JsonObject json = doc->createNestedObject("effectLoader");
	JsonArray effectsArray = json.createNestedArray("effects");
	for (std::size_t i = 0; i < effects.size(); ++i) {
		auto effect = effects[i];
		JsonObject effectObject = effectsArray.createNestedObject();
		effectObject["name"] = effect->getName();
		effectObject["id"] = effect->getEffectId();
	}

	json["currentEffect"] = currentEffect;
}