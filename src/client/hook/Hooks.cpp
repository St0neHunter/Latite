#include "pch.h"
#include "Hooks.h"

#include "impl/GeneralHooks.h"
#include "impl/LevelRendererHooks.h"
#include "impl/OptionHooks.h"
#include "impl/DXHooks.h"
#include "impl/MinecraftGameHooks.h"
#include "impl/RenderControllerHooks.h"
#include "impl/ScreenViewHooks.h"
#include "impl/PacketHooks.h"
#include "MinHook.h"
#include <vhook/vtable_hook.h>

#include "util/ErrorHandler.h"

using namespace std::chrono_literals;


LatiteHooks::LatiteHooks() {}

LatiteHooks::~LatiteHooks() {
	BEGIN_ERROR_HANDLER
	MH_Uninitialize();
	END_ERROR_HANDLER
}

void LatiteHooks::enable() {
	BEGIN_ERROR_HANDLER
	MH_EnableHook(MH_ALL_HOOKS);
	END_ERROR_HANDLER
}

void LatiteHooks::disable() {
	BEGIN_ERROR_HANDLER
	MH_DisableHook(MH_ALL_HOOKS);
	vh::unhook_all();
	END_ERROR_HANDLER
}
