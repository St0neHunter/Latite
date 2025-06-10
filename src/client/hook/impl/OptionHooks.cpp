#include "pch.h"
#include "OptionHooks.h"
#include "client/event/Eventing.h"
#include "client/event/impl/GammaEvent.h"
#include "client/event/impl/PerspectiveEvent.h"
#include "client/event/impl/HideHandEvent.h"
#include "client/event/impl/SensitivityEvent.h"
#include "util/ErrorHandler.h"

namespace {
	std::shared_ptr<Hook> getGammaHook;
	std::shared_ptr<Hook> getPerspectiveHook;
	std::shared_ptr<Hook> getHideHandHook;
	std::shared_ptr<Hook> getSensitivityHook;
}

float OptionHooks::Options_getGamma(void* options, void* a2) {
	BEGIN_ERROR_HANDLER
	auto o = getGammaHook->oFunc<decltype(&Options_getGamma)>()(options, a2);
	GammaEvent ev{ o };
	Eventing::get().dispatch(ev);
	return o;
	END_ERROR_HANDLER
}

int OptionHooks::Options_getPerspective(void* options) {
	BEGIN_ERROR_HANDLER
	auto o = getPerspectiveHook->oFunc<decltype(&Options_getPerspective)>()(options);
	PerspectiveEvent ev{ o };
	Eventing::get().dispatch(ev);
	return o;
	END_ERROR_HANDLER
}

bool OptionHooks::Options_getHideHand(void* options) {
	BEGIN_ERROR_HANDLER
	auto o = getHideHandHook->oFunc<decltype(&Options_getHideHand)>()(options);
	HideHandEvent ev{ o };
	Eventing::get().dispatch(ev);
	return o;
	END_ERROR_HANDLER
}

float OptionHooks::Options_getSensitivity(void* options, unsigned int a2, int a3, int a4, int a5) {
	BEGIN_ERROR_HANDLER
	auto o = getSensitivityHook->oFunc<decltype(&Options_getSensitivity)>()(options, a2, a3, a4, a5);
	SensitivityEvent ev{ o };
	Eventing::get().dispatch(ev);
	return o;
	END_ERROR_HANDLER
}

OptionHooks::OptionHooks() {
	BEGIN_ERROR_HANDLER
	getGammaHook = addHook(Signatures::Options_getGamma.result, Options_getGamma, "Options::getGamma");
	getPerspectiveHook = addHook(Signatures::Options_getPerspective.result, Options_getPerspective, "Options::getPerspective");
	getHideHandHook = addHook(Signatures::Options_getHideHand.result, Options_getHideHand, "Options::getHideHand");
	//getSensitivityHook = addHook(Signatures::Options_getSensitivity.result, Options_getSensitivity, "Options::getSensitivity");
	END_ERROR_HANDLER
}
