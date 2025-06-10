#pragma once
#include "api/feature/module/ModuleManager.h"
#include "api/eventing/Event.h"
#include "api/eventing/Listenable.h"
#include "Module.h"
#include "script/JsModule.h"

class ModuleManager : public Listener, public IModuleManager {
public:
	ModuleManager();
	~ModuleManager();

	bool registerScriptModule(JsModule* mod) {
		BEGIN_ERROR_HANDLER
		for (auto& mod_ : items) {
			if (mod_->name() == mod->name()) {
				return false;
			}
		}

		mod->onInit();
		this->items.push_back(std::shared_ptr<JsModule>(mod));
		JS::JsAddRef(mod->object, nullptr);
		return true;
		END_ERROR_HANDLER
	}

	bool deregisterScriptModule(JsModule* mod) {
		BEGIN_ERROR_HANDLER
		for (auto it = items.begin(); it != items.end(); it++) {
			if (it->get() == mod) {
				items.erase(it);
				return true;
			}
		}
		return false;
		END_ERROR_HANDLER
	}

	void onKey(Event& ev);
};