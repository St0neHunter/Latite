#include "pch.h"
#include "RenderControllerHooks.h"
#include "client/event/Eventing.h"
#include "client/event/impl/OverlayColorEvent.h"
#include "sdk/common/world/actor/Actor.h"
#include "sdk/Util.h"
#include "util/ErrorHandler.h"

namespace {
	std::shared_ptr<Hook> GetOverlayColorHook;
}

Color* RenderControllerHooks::getOverlayColor(void* thisptr, Color* out, void* ent) {
	BEGIN_ERROR_HANDLER
	GetOverlayColorHook->oFunc<decltype(&getOverlayColor)>()(thisptr, out, ent);
	OverlayColorEvent ev{ *out, util::directAccess<SDK::Actor*>(ent, 0x38) }; // xref: getOverlayColor itself
	{
		Eventing::get().dispatch(ev);
		*out = ev.getColor();
	}
	return out;
	END_ERROR_HANDLER
}

RenderControllerHooks::RenderControllerHooks() {
	BEGIN_ERROR_HANDLER
	GetOverlayColorHook = addHook(Signatures::RenderController_getOverlayColor.result, getOverlayColor, "RenderController::getOverlayColor");
	END_ERROR_HANDLER
}
