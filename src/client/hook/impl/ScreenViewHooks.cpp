#include "pch.h"
#include "ScreenViewHooks.h"
#include "PacketHooks.h"
#include "client/event/Eventing.h"
#include "client/event/impl/RenderLayerEvent.h"
#include "client/event/impl/RenderGameEvent.h"
#include "sdk/common/client/gui/controls/VisualTree.h"
#include "sdk/common/client/gui/controls/UIControl.h"
#include "../Hooks.h"
#include <client/script/PluginManager.h>
#include "util/ErrorHandler.h"

namespace {
	std::shared_ptr<Hook> setupAndRenderHook;
}

void __fastcall ScreenViewHooks::setupAndRender(SDK::ScreenView* view, void* ctx) {
	BEGIN_ERROR_HANDLER
	setupAndRenderHook->oFunc<decltype(&setupAndRender)>()(view, ctx);
	RenderLayerEvent ev{ view, reinterpret_cast<SDK::MinecraftUIRenderContext*>(ctx) };
	Eventing::get().dispatch(ev);

	if (view->visualTree->rootControl->name == "debug_screen") {
		PluginManager::Event ev{L"render2d", {}, false};
		Latite::getPluginManager().dispatchEvent(ev);
	}

	static bool hasInitPacketSender = false;
	if (!hasInitPacketSender) {
		if (SDK::ClientInstance::get()->getLocalPlayer()) {
			Latite::getHooks().get<PacketHooks>().initPacketSender(SDK::ClientInstance::get()->getLocalPlayer()->packetSender);
			hasInitPacketSender = true;
		}
	}

	RenderGameEvent evt{ };
	Eventing::get().dispatch(evt);
	END_ERROR_HANDLER
}

ScreenViewHooks::ScreenViewHooks() : HookGroup("ScreenView") {
	BEGIN_ERROR_HANDLER
	setupAndRenderHook = addHook(Signatures::ScreenView_setupAndRender.result, setupAndRender,
		"ScreenView::setupAndRender");
	END_ERROR_HANDLER
}
