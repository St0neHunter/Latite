#include "pch.h"
#include "LevelRendererHooks.h"
#include "client/event/Eventing.h"
#include "client/event/impl/RenderLevelEvent.h"
#include "client/render/Renderer.h"
#include "client/Latite.h"
#include "util/ErrorHandler.h"

namespace {
	std::shared_ptr<Hook> renderLevelHook;
}

void LevelRendererHooks::LevelRenderer_renderLevel(SDK::LevelRenderer* lvl, SDK::ScreenContext* scn, void* unk) {
	BEGIN_ERROR_HANDLER
	SDK::ScreenContext::instance3d = scn;

	{
		PluginManager::Event ev{ L"render3d", {}, true };
		if (Latite::getPluginManager().dispatchEvent(ev)) {
			return;
		}
	}

	renderLevelHook->oFunc<decltype(&LevelRenderer_renderLevel)>()(lvl, scn, unk);
	{
		RenderLevelEvent ev{ lvl, scn };
		Eventing::get().dispatch(ev);
	}
	END_ERROR_HANDLER
}

LevelRendererHooks::LevelRendererHooks() {
	BEGIN_ERROR_HANDLER
	renderLevelHook = addHook(Signatures::LevelRenderer_renderLevel.result, LevelRenderer_renderLevel,
		"LevelRenderer::renderLevel");
	END_ERROR_HANDLER
}
