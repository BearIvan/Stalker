#include "tools/xrCore.h"
using namespace BearCore;
#include "BearGraphics.hpp"
#include "BearUI.hpp"

#include "api/StalkerAPI.h"

#include "engine/XRayExports.h"
#include "engine/render.h"
#include "engine/Properties.h"

#include "engine/vis_common.h"
#include "Engine/XRayRenderTarget.h"
#include "Engine/XRayRenderInterface.h"
#include "RenderFactory.h"

#include "xrRender/UISequenceVideoItem.h"
#include "Engine/XRayUISequenceVideoItem.h"

#include "xrRender/ConsoleRender.h"
#include "Render/XRayConsoleRender.h"

#include "xrRender/StatGraphRender.h"
#include "Render/XRayStatGraphRender.h"

#include "xrRender/EnvironmentRender.h"
#include "Render/XRayEnvironmentRender.h"
#include "Render/XRayEnvDescriptorRender.h"
#include "Render/XRayEnvDescriptorMixerRender.h"

#include "xrRender/LensFlareRender.h"
#include "Render/XRayLensFlareRender.h"
#include "Render/XRayFlareRender.h"

#include "xrRender/RainRender.h"
#include "Render/XRayRainRender.h"

#include "xrRender/ThunderboltDescRender.h"
#include "Render/XRayThunderboltDescRender.h"

#include "xrRender/ThunderboltRender.h"
#include "Render/XRayThunderboltRender.h"

#include  "xrRender/RenderDeviceRender.h"
#include "Render/XRayRenderDeviceRender.h"

#include "xrRender/StatsRender.h"
#include "Render/XRayStatsRender.h"

#include "xrRender/ApplicationRender.h"
#include "Render/XRayApplicationRender.h"


#include "xrRender/WallMarkArray.h"
#include "Engine/XRayWallMarkArray.h"

#include "xrRender/ObjectSpaceRender.h"
#include "Render/XRayObjectSpaceRender.h"

#include "Engine/XRayRenderFactory.h"

#include "xrRender/DrawUtils.h"
#include "Engine/XRayDUInterface.h"

#include "Resources/XRayMatrix.h"
#include "Resources/XRayConstant.h"
#include "Resources/XRayTexture.h"
#include "Resources/XRayShader.h"

#include "Blenders/XRayBlender.h"


#include "xrRender/FontRender.h"
#include "Render/XRayFontRender.h"

#include "Blenders/XRayBlenderDefault.h"
#include "Blenders/XRayBlenderDefaultAref.h"
#include "Blenders/XRayBlenderVertex.h"
#include "Blenders/XRayBlenderVertexAref.h"
#include "Blenders/XRayBlenderScreenSet.h"
#include "Blenders/XRayBlenderScreenGray.h"
#include "Blenders/XRayBlenderEditorSelection.h"
#include "Blenders/XRayBlenderEditorWire.h"
#include "Blenders/XRayBlenderLaEmB.h"
#include "Blenders/XRayBlenderLmEbB.h"
#include "Blenders/XRayBlenderBmmD.h"
#include "Blenders/XRayBlenderShWorld.h"
#include "Blenders/XRayBlenderBlur.h"
#include "Blenders/XRayBlenderModel.h"
#include "Blenders/XRayBlenderModelEbB.h"
#include "Blenders/XRayBlenderDetailStill.h"
#include "Blenders/XRayBlenderTree.h"
#include "Blenders/XRayBlenderParticle.h"
#include "Blenders/XRayBlenderLTX.h"
 


#include "Resources/XRayRenderResources.h"


#include "xrRender/UIShader.h"
#include "Resources/XRayUIShader.h"


#include "xrRender/UIRender.h"
#include "Render/XRayUIRender.h"

#include "xrRender/DebugRender.h"
#include "Render/XRayDebugRender.h"

#include "Blenders/XRayIDofBlenders.h"

#include "Engine/stdafx.h"
#include "engine/igame_level.h"
