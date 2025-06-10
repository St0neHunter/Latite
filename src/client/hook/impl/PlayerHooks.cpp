#include "pch.h"
#include "PlayerHooks.h"
#include "util/ErrorHandler.h"

static std::shared_ptr<Hook> ActorAttackHook;

void PlayerHooks::hkActorAttack(SDK::Actor* obj, SDK::Actor* target, void* cause, bool a4) {
	BEGIN_ERROR_HANDLER
	if (obj == SDK::ClientInstance::get()->getLocalPlayer()) {
		AttackEvent ev{ target };
		Eventing::get().dispatch(ev);
	}

	ActorAttackHook->oFunc<decltype(&hkActorAttack)>()(obj, target, cause, a4);
	END_ERROR_HANDLER
}

void PlayerHooks::init(SDK::LocalPlayer* lp) {
	BEGIN_ERROR_HANDLER
	//uintptr_t* vtable = *reinterpret_cast<uintptr_t**>(lp);
	//uintptr_t* gmTable = *reinterpret_cast<uintptr_t**>(lp->gameMode);
	
	//GameModeAttackHook = this->addTableSwapHook((uintptr_t)(gmTable + 14), &hkGameModeAttack, "GameMode::attack");
	//GameModeAttackHook->enable();
	END_ERROR_HANDLER
}

PlayerHooks::PlayerHooks() {
	BEGIN_ERROR_HANDLER
	ActorAttackHook = this->addHook(Signatures::Actor_attack.result, &hkActorAttack, "Actor::attack");
	END_ERROR_HANDLER
}
