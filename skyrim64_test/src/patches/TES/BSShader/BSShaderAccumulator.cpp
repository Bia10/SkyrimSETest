#include "../../rendering/common.h"
#include "../../../common.h"
#include "../BSGraphicsRenderer.h"
#include "../BSGraphicsState.h"
#include "../BSGraphicsRenderTargetManager.h"
#include "../BSBatchRenderer.h"
#include "BSShaderManager.h"
#include "BSShaderAccumulator.h"
#include "../BSReadWriteLock.h"
#include "../MTRenderer.h"
#include "../MOC.h"

AutoPtr(BSShaderAccumulator *, ZPrePassAccumulator, 0x3257A68);
AutoPtr(BSShaderAccumulator *, MainPassAccumulator, 0x3257A70);

void BSShaderAccumulator::InitCallbackTable()
{
	// If the pointer is null, it defaults to the function at index 0
	RegisterObjectArray[0] = RegisterObject_Default;							// 00 RegisterObject_Default
	RegisterObjectArray[1] = RegisterObject_Default;							// 01 ^
	RegisterObjectArray[2] = RegisterObject_Default;							// 02 ^
	RegisterObjectArray[3] = RegisterObject_Default;							// 03 ^
	RegisterObjectArray[4] = RegisterObject_Default;							// 04 ^
	RegisterObjectArray[5] = RegisterObject_Default;							// 05 ^
	RegisterObjectArray[6] = RegisterObject_Default;							// 06 ^
	RegisterObjectArray[7] = RegisterObject_Default;							// 07 ^
	RegisterObjectArray[8] = RegisterObject_Default;							// 08 ^
	RegisterObjectArray[9] = RegisterObject_Default;							// 09 ^
	RegisterObjectArray[10] = RegisterObject_Default;							// 10 ^
	RegisterObjectArray[11] = RegisterObject_Default;							// 11 ^
	RegisterObjectArray[12] = RegisterObject_ShadowMapOrMask;					// 12 RegisterObject_ShadowMapOrMask
	RegisterObjectArray[13] = RegisterObject_ShadowMapOrMask;					// 13 ^
	RegisterObjectArray[14] = RegisterObject_ShadowMapOrMask;					// 14 ^
	RegisterObjectArray[15] = RegisterObject_ShadowMapOrMask;					// 15 ^
	RegisterObjectArray[16] = RegisterObject_ShadowMapOrMask;					// 16 ^
	RegisterObjectArray[17] = RegisterObject_ShadowMapOrMask;					// 17 ^
	RegisterObjectArray[18] = (RegisterObjFunc)(g_ModuleBase + 0x12E1400);		// 18 ?
	RegisterObjectArray[19] = (RegisterObjFunc)(g_ModuleBase + 0x12E1400);		// 19 ?
	RegisterObjectArray[20] = (RegisterObjFunc)(g_ModuleBase + 0x12E1340);		// 20 RegisterObject_Interface
	RegisterObjectArray[21] = (RegisterObjFunc)(g_ModuleBase + 0x12E1370);		// 21 ?
	RegisterObjectArray[22] = RegisterObject_Default;							// 22 RegisterObject_Default
	RegisterObjectArray[23] = (RegisterObjFunc)(g_ModuleBase + 0x12E1380);		// 23 ?
	RegisterObjectArray[24] = (RegisterObjFunc)(g_ModuleBase + 0x12E15E0);		// 24 ?
	RegisterObjectArray[25] = (RegisterObjFunc)(g_ModuleBase + 0x12E1780);		// 25 RegisterObject_LOD
	RegisterObjectArray[26] = (RegisterObjFunc)(g_ModuleBase + 0x12E1780);		// 26 RegisterObject_LOD
	RegisterObjectArray[27] = RegisterObject_Default;							// 27 RegisterObject_Default
	RegisterObjectArray[28] = (RegisterObjFunc)(g_ModuleBase + 0x12E1800);		// 28 ?
	RegisterObjectArray[29] = RegisterObject_Default;							// 29 RegisterObject_Default

	// If the pointer is null, it defaults to the function at index 0
	FinishAccumulatingArray[0] = FinishAccumulating_Default;					// 00 FinishAccumulating_Default
	FinishAccumulatingArray[1] = nullptr;										// 01 ^
	FinishAccumulatingArray[2] = nullptr;										// 02 ^
	FinishAccumulatingArray[3] = nullptr;										// 03 ^
	FinishAccumulatingArray[4] = nullptr;										// 04 ^
	FinishAccumulatingArray[5] = nullptr;										// 05 ^
	FinishAccumulatingArray[6] = nullptr;										// 06 ^
	FinishAccumulatingArray[7] = nullptr;										// 07 ^
	FinishAccumulatingArray[8] = nullptr;										// 08 ^
	FinishAccumulatingArray[9] = nullptr;										// 09 ^
	FinishAccumulatingArray[10] = nullptr;										// 10 ^
	FinishAccumulatingArray[11] = nullptr;										// 11 ^
	FinishAccumulatingArray[12] = FinishAccumulating_ShadowMapOrMask;			// 12 FinishAccumulating_ShadowMapOrMask
	FinishAccumulatingArray[13] = FinishAccumulating_ShadowMapOrMask;			// 13 ^
	FinishAccumulatingArray[14] = FinishAccumulating_ShadowMapOrMask;			// 14 ^
	FinishAccumulatingArray[15] = FinishAccumulating_ShadowMapOrMask;			// 15 ^
	FinishAccumulatingArray[16] = FinishAccumulating_ShadowMapOrMask;			// 16 ^
	FinishAccumulatingArray[17] = FinishAccumulating_ShadowMapOrMask;			// 17 ^
	FinishAccumulatingArray[18] = nullptr;										// 18 FinishAccumulating_Default
	FinishAccumulatingArray[19] = nullptr;										// 19 ^
	FinishAccumulatingArray[20] = FinishAccumulating_InterfaceElements;			// 20 FinishAccumulating_InterfaceElements
	FinishAccumulatingArray[21] = nullptr;										// 21 FinishAccumulating_Default
	FinishAccumulatingArray[22] = FinishAccumulating_FirstPerson;				// 22 FinishAccumulating_FirstPerson
	FinishAccumulatingArray[23] = (FinishAccumFunc)(g_ModuleBase + 0x12E3020);	// 23 ?
	FinishAccumulatingArray[24] = (FinishAccumFunc)(g_ModuleBase + 0x12E3030);	// 24 ?
	FinishAccumulatingArray[25] = FinishAccumulating_LODOnly;					// 25 FinishAccumulating_LODOnly
	FinishAccumulatingArray[26] = FinishAccumulating_LODOnly;					// 26 FinishAccumulating_LODOnly
	FinishAccumulatingArray[27] = nullptr;										// 27 FinishAccumulating_Default
	FinishAccumulatingArray[28] = FinishAccumulating_Unknown1;					// 28 Never used? BSSM_RENDER_PRECIPITATION_OCCLUSION_MAP?
	FinishAccumulatingArray[29] = nullptr;										// 29 FinishAccumulating_Default

	SetRenderMode(0);
}

void BSShaderAccumulator::SetRenderMode(uint32_t RenderMode)
{
	RegisterObjectCurrent = RegisterObjectArray[RenderMode];
	FinishAccumulatingCurrent = FinishAccumulatingArray[RenderMode];

	if (!RegisterObjectCurrent)
		RegisterObjectCurrent = RegisterObjectArray[0];

	if (!FinishAccumulatingCurrent)
		FinishAccumulatingCurrent = FinishAccumulatingArray[0];
}

bool BSShaderAccumulator::hk_RegisterObjectDispatch(BSGeometry *Geometry, void *Unknown)
{
	NiSkinInstance *skinInstance = Geometry->QSkinInstance();
	BSShaderProperty *shaderProperty = Geometry->QShaderProperty();

	if (!shaderProperty)
		return true;

	// BSDismemberSkinInstance::bVisible
	if (skinInstance && skinInstance->IsExactKindOf(NiRTTI::ms_BSDismemberSkinInstance) && !*(BYTE *)((__int64)skinInstance + 0x98))
		return true;

	if (!Geometry->QRendererData() && !skinInstance && !Geometry->IsParticlesGeom() && Geometry->QType() != GEOMETRY_TYPE_PARTICLE_SHADER_DYNAMIC_TRISHAPE)
		return true;
	
	bool result = RegisterObjectArray[m_RenderMode](this, Geometry, shaderProperty, Unknown);

	uint32_t v9 = *(uint32_t *)((__int64)this + 0x160);

	if (v9 != 0)
	{
		__int64 v10 = (__int64)shaderProperty->pLightData;

		if (v10)
		{
			if (v9 == 0xFFFF)
				*(uint32_t *)(v10 + 0x1C) = 0;// BSShaderPropertyLightData::uiShadowAccumFlags
			else
				*(uint32_t *)(v10 + 0x1C) |= *(uint32_t *)((__int64)this + 0x164);
		}
	}

	return result;
}

void BSShaderAccumulator::hk_FinishAccumulatingDispatch(uint32_t RenderFlags)
{
	SetRenderMode(m_RenderMode);

	if (m_RenderMode != 0)
		FinishAccumulatingCurrent(this, RenderFlags);
	else
		RenderSceneNormal(this, RenderFlags);
}

bool BSShaderAccumulator::IsGrassShadowBlacklist(uint32_t Technique)
{
	return Technique == BSSM_GRASS_SHADOW_L ||
		Technique == BSSM_GRASS_SHADOW_LS ||
		Technique == BSSM_GRASS_SHADOW_LB ||
		Technique == BSSM_GRASS_SHADOW_LSB;
}

void ApplySceneDepthShift()
{
	((void(__fastcall *)())(g_ModuleBase + 0x12F8B10))();
}

void ResetSceneDepthShift()
{
	((void(__fastcall *)())(g_ModuleBase + 0x12F8C70))();
}

bool BSShaderAccumulator::RegisterObject_Default(BSShaderAccumulator *Accumulator, BSGeometry *Geometry, BSShaderProperty *Property, void *Unknown)
{
	return ((RegisterObjFunc)(g_ModuleBase + 0x12E0F90))(Accumulator, Geometry, Property, Unknown);
}

bool BSShaderAccumulator::RegisterObject_ShadowMapOrMask(BSShaderAccumulator *Accumulator, BSGeometry *Geometry, BSShaderProperty *Property, void *Unknown)
{
	return ((RegisterObjFunc)(g_ModuleBase + 0x12E1650))(Accumulator, Geometry, Property, Unknown);
}

void BSShaderAccumulator::FinishAccumulating_Default(BSShaderAccumulator *Accumulator, uint32_t RenderFlags)
{
	ZoneScopedN("FinishAccumulating_Default");
	BSGraphics::BeginEvent(L"FinishAccumulating_Default");
	RenderSceneNormal(Accumulator, RenderFlags);
	RenderSceneNormalAlphaZ(Accumulator, RenderFlags);
	BSGraphics::EndEvent();
}

void BSShaderAccumulator::RenderSceneNormal(BSShaderAccumulator *Accumulator, uint32_t RenderFlags)
{
	static bool once = false;

	if (!once)
	{
		once = true;
		MOC::Init();
	}

	if (!Accumulator->m_pkCamera)
		return;

	__int64 a1 = (__int64)Accumulator;
	auto renderer = BSGraphics::Renderer::GetGlobals();

	renderer->BeginEvent(L"BSShaderAccumulator: Draw1");
	ZoneScopedN("RenderSceneNormal");

	if (*(BYTE *)(a1 + 92) && !BSGraphics::gState.bUseEarlyZ)
		renderer->DepthStencilStateSetDepthMode(BSGraphics::DEPTH_STENCIL_DEPTH_MODE_TESTEQUAL);

	// v7 = RenderDepthOnly()? RenderAlphaOnly()? BSShaderManager::BSS_SHADOWS?
	bool v7 = (RenderFlags & 0xA) != 0;

	if (!v7)
		ApplySceneDepthShift();

	if (Accumulator == MainPassAccumulator)
	{
		if (ui::opt::RealtimeOcclusionView)
			MOC::UpdateDepthViewTexture();

		//MOC::SendTraverseCommand((NiCamera *)Accumulator->m_pkCamera);
	}

	//
	// Original draw order:
	//
	// RenderBatches
	// LowAniso
	// RenderGrass
	// RenderNoShadowGroup
	// RenderLODObjects
	// RenderLODLand
	// RenderSky
	// RenderSkyClouds
	// ???
	// BlendedDecals
	// RenderWaterStencil
	//
	{
		ProfileTimer("RenderBatches");

		// RenderBatches
		renderer->BeginEvent(L"RenderBatches");
		{
			Accumulator->RenderFromMainGroup(1, BSSM_DISTANTTREE_DEPTH, RenderFlags, -1);
		}
		renderer->EndEvent();

		// LowAniso
		renderer->BeginEvent(L"LowAniso");
		{
			Accumulator->RenderFromMainGroup(1, BSSM_BLOOD_SPLATTER, RenderFlags, 9);
		}
		renderer->EndEvent();

		// RenderGrass
		renderer->BeginEvent(L"RenderGrass");
		{
			Accumulator->RenderFromMainGroup(BSSM_GRASS_DIRONLY_LF, 0x5C00005C, RenderFlags, -1);
		}
		renderer->EndEvent();

		// RenderNoShadowGroup
		renderer->BeginEvent(L"RenderNoShadowGroup");
		{
			Accumulator->RenderFromMainGroup(1, BSSM_BLOOD_SPLATTER, RenderFlags, 8);
		}
		renderer->EndEvent();

		// RenderLODObjects
		renderer->BeginEvent(L"RenderLODObjects");
		{
			Accumulator->RenderFromMainGroup(1, BSSM_BLOOD_SPLATTER, RenderFlags, 1);

			if (*(BYTE *)(a1 + 92) && !BSGraphics::gState.bUseEarlyZ)
				renderer->DepthStencilStateSetDepthMode(BSGraphics::DEPTH_STENCIL_DEPTH_MODE_TEST_WRITE);
		}
		renderer->EndEvent();

		// RenderLODLand
		renderer->BeginEvent(L"RenderLODLand");
		{
			Accumulator->RenderFromMainGroup(1, BSSM_BLOOD_SPLATTER, RenderFlags, 0);

			if (!v7)
				ResetSceneDepthShift();
		}
		renderer->EndEvent();
	}

	// RenderSky
	renderer->BeginEvent(L"RenderSky");
	{
		renderer->SetUseAlphaTestRef(true);
		renderer->SetAlphaTestRef(128.0f / 255.0f);

		Accumulator->RenderFromMainGroup(BSSM_SKYBASEPRE, BSSM_SKY_CLOUDSFADE, RenderFlags, -1);
	}
	renderer->EndEvent();

	// RenderSkyClouds
	renderer->BeginEvent(L"RenderSkyClouds");
	{
		renderer->AlphaBlendStateSetUnknown2(11);

		Accumulator->RenderFromMainGroup(1, BSSM_BLOOD_SPLATTER, RenderFlags, 13);

		renderer->AlphaBlendStateSetUnknown2(1);
	}
	renderer->EndEvent();

	if (!v7)
		ApplySceneDepthShift();

	// NormalDecals?...CK doesn't have a specific name for this
	{
		renderer->AlphaBlendStateSetUnknown2(10);
		((void(__fastcall *)(BSShaderAccumulator *, unsigned int))(g_ModuleBase + 0x12E27B0))(Accumulator, RenderFlags);
	}

	// BlendedDecals
	renderer->BeginEvent(L"BlendedDecals");
	{
		renderer->AlphaBlendStateSetUnknown2(11);
		((void(__fastcall *)(BSShaderAccumulator *, unsigned int))(g_ModuleBase + 0x12E2950))(Accumulator, RenderFlags);
	}
	renderer->EndEvent();

	renderer->AlphaBlendStateSetMode(0);
	renderer->AlphaBlendStateSetUnknown2(1);

	AutoFunc(int(__fastcall *)(), sub_140D744B0, 0xD744E0);
	AutoFunc(__int64(__fastcall *)(__int64 a1, unsigned int a2), sub_140D69E70, 0xD69EA0);
	AutoFunc(__int64(__fastcall *)(float *a1, float a2, float a3, float a4, int a5), sub_140D69D30, 0xD69D60);
	AutoFunc(int(__fastcall *)(), sub_1412FD120, 0x12FD480);
	AutoFunc(__int64(__fastcall *)(__int64 a1, unsigned int a2, int a3, int a4, char a5), sub_140D74350, 0xD74380);
	AutoFunc(void(__fastcall *)(__int64 a1, uint32_t a2, int a3, uint32_t a4), sub_140D74370, 0xD743A0);
	AutoFunc(void(__fastcall *)(__int64 a1, char a2), sub_140D69990, 0xD699C0);
	AutoFunc(__int64(__fastcall *)(), sub_1412FADA0, 0x12FB100);
	AutoFunc(void(__fastcall *)(DWORD *a1), sub_140D69DA0, 0xD69DD0);

	DWORD *flt_14304E490 = (DWORD *)(g_ModuleBase + 0x304E490);

	if ((RenderFlags & 0x80) != 0 && Accumulator->m_MainBatch->HasTechniquePasses(0x5C000071, 0x5C006071))
	{
		int aiSource = sub_140D744B0();

		//	bAssert(aiSource != aiTarget &&
		//		aiSource < DEPTH_STENCIL_COUNT &&
		//		aiTarget < DEPTH_STENCIL_COUNT &&
		//		aiSource != DEPTH_STENCIL_TARGET_NONE &&
		//		aiTarget != DEPTH_STENCIL_TARGET_NONE);

		renderer->m_DeviceContext->CopyResource(*(ID3D11Resource **)(g_ModuleBase + 0x3050870), *((ID3D11Resource **)flt_14304E490 + 19 * aiSource + 1015));
	}

	// RenderWaterStencil
	renderer->BeginEvent(L"RenderWaterStencil");
	{
		if (Accumulator->m_MainBatch->HasTechniquePasses(BSSM_WATER_STENCIL, BSSM_WATER_DISPLACEMENT_STENCIL_Vc))
		{
			sub_140D69E70((__int64)flt_14304E490, 2u);// BSGraphics::Renderer::ClearDepthStencil(CLEAR_DEPTH_STENCIL_TARGET_STENCIL)
			sub_140D69D30((float *)flt_14304E490, 0.0, 0.0, 0.0, 0);
			int v20 = sub_1412FD120();
			sub_140D74350((__int64)(g_ModuleBase + 0x3051B20), 0, v20, 3, 1);// RENDER_TARGET_NORMAL_TAAMASK_SSRMASK (This can change) SRTM_NO_CLEAR
			sub_140D74350((__int64)(g_ModuleBase + 0x3051B20), 1u, 7, 3, 1);// RENDER_TARGET_MOTION_VECTOR SRTM_NO_CLEAR
			sub_140D74350((__int64)(g_ModuleBase + 0x3051B20), 2u, -1, 3, 1);// RENDER_TARGET_NONE SRTM_NO_CLEAR
			sub_140D74350((__int64)(g_ModuleBase + 0x3051B20), 3u, -1, 3, 1);// RENDER_TARGET_NONE SRTM_NO_CLEAR
			sub_140D74350((__int64)(g_ModuleBase + 0x3051B20), 4u, -1, 3, 1);// RENDER_TARGET_NONE SRTM_NO_CLEAR
			Accumulator->RenderFromMainGroup(BSSM_WATER_STENCIL, BSSM_WATER_DISPLACEMENT_STENCIL_Vc, RenderFlags, -1);
			sub_140D69DA0((DWORD *)flt_14304E490);
			*(DWORD *)(*(uint64_t *)((*(uint64_t*)(g_ModuleBase + 0x31F5810)) + 496) + 44i64) = 1;
		}
	}
	renderer->EndEvent();

	if (RenderFlags & 0x40)
	{
		sub_140D74370((__int64)(g_ModuleBase + 0x3051B20), 0xFFFFFFFF, 3, 0);
		sub_140D74350((__int64)(g_ModuleBase + 0x3051B20), 1u, -1, 3, 1);// RENDER_TARGET_NONE SRTM_NO_CLEAR
		sub_140D74350((__int64)(g_ModuleBase + 0x3051B20), 2u, -1, 3, 1);// RENDER_TARGET_NONE SRTM_NO_CLEAR
		sub_140D74350((__int64)(g_ModuleBase + 0x3051B20), 3u, -1, 3, 1);// RENDER_TARGET_NONE SRTM_NO_CLEAR
		sub_140D74350((__int64)(g_ModuleBase + 0x3051B20), 4u, -1, 3, 1);// RENDER_TARGET_NONE SRTM_NO_CLEAR
		sub_140D69990((__int64)flt_14304E490, 1);
		sub_1412FADA0();// SSR + downsample imagespace shaders
		sub_140D74350((__int64)(g_ModuleBase + 0x3051B20), 0, 1, 3, 1);
		int v21 = sub_140D744B0();
		sub_140D74370((__int64)(g_ModuleBase + 0x3051B20), v21, 3, 0);

		renderer->DepthStencilStateSetDepthMode(BSGraphics::DEPTH_STENCIL_DEPTH_MODE_TEST);
	}

	if (!v7)
		ResetSceneDepthShift();

	renderer->EndEvent();
}

void BSShaderAccumulator::RenderSceneNormalAlphaZ(BSShaderAccumulator *Accumulator, uint32_t RenderFlags)
{
	ZoneScopedN("RenderSceneNormalAlphaZ");
	((FinishAccumFunc)(g_ModuleBase + 0x12E1F70))(Accumulator, RenderFlags);
}

void BSShaderAccumulator::FinishAccumulating_ShadowMapOrMask(BSShaderAccumulator *Accumulator, uint32_t RenderFlags)
{
	if (!Accumulator->m_pkCamera)
		return;

	ZoneScopedN("FinishAccumulating_ShadowMapOrMask");
	BSGraphics::BeginEvent(L"FinishAccumulating_ShadowMapOrMask");

	if ((RenderFlags & 0x22) == 0x20)
		ApplySceneDepthShift();

	if (RenderFlags & 0x100)
	{
		//AssertMsgDebug(iEnableShadowCastingFlag->Value == 2, "Volumetric lighting shadow map render was called whereas it should have been disabled.");
		//AssertMsgDebug(Accumulator->m_RenderMode == ?, "Volumetric lighting shadow map render was called for a non directional light.");

		BSGraphics::BeginEvent(L"Volumetric lighting specific shadow map");
		BSGraphics::Renderer::GetGlobals()->RasterStateSetCullMode(0);
		Accumulator->RenderFromMainGroup(1, BSSM_BLOOD_SPLATTER, RenderFlags, 15);
		BSGraphics::EndEvent();
	}
	else
	{
		BSGraphics::BeginEvent(L"RenderBatches");
		Accumulator->RenderFromMainGroup(0x2B, 0x4000002B, RenderFlags, -1);
		Accumulator->RenderFromMainGroup(BSSM_GRASS_DIRONLY_LF, 0x5C00005C, RenderFlags, -1);
		Accumulator->RenderFromMainGroup(1, BSSM_BLOOD_SPLATTER, RenderFlags, 1);
		BSGraphics::EndEvent();

		BSGraphics::BeginEvent(L"LowAniso");
		Accumulator->RenderFromMainGroup(1, BSSM_BLOOD_SPLATTER, RenderFlags, 9);
		BSGraphics::EndEvent();

		BSGraphics::BeginEvent(L"Decals");
		((void(__fastcall *)(BSShaderAccumulator *, uint32_t))(g_ModuleBase + 0x12E2950))(Accumulator, RenderFlags);
		BSGraphics::EndEvent();
	}

	if ((RenderFlags & 0x22) == 0x20)
		ResetSceneDepthShift();

	BSGraphics::EndEvent();
}

void BSShaderAccumulator::FinishAccumulating_InterfaceElements(BSShaderAccumulator *Accumulator, uint32_t RenderFlags)
{
	ZoneScopedN("FinishAccumulating_InterfaceElements");
	BSGraphics::BeginEvent(L"FinishAccumulating_InterfaceElements");
	((FinishAccumFunc)(g_ModuleBase + 0x12E2FE0))(Accumulator, RenderFlags);
	BSGraphics::EndEvent();
}

void BSShaderAccumulator::FinishAccumulating_FirstPerson(BSShaderAccumulator *Accumulator, uint32_t RenderFlags)
{
	ZoneScopedN("FinishAccumulating_FirstPerson");
	BSGraphics::BeginEvent(L"FinishAccumulating_FirstPerson");
	((FinishAccumFunc)(g_ModuleBase + 0x12E2B20))(Accumulator, RenderFlags);
	BSGraphics::EndEvent();
}

void BSShaderAccumulator::FinishAccumulating_LODOnly(BSShaderAccumulator *Accumulator, uint32_t RenderFlags)
{
	if (!Accumulator->m_pkCamera)
		return;

	BSShaderManager::bLODLandscapeNoise = false;

	BSGraphics::BeginEvent(L"FinishAccumulating_LODOnly");
	{
		ZoneScopedN("FinishAccumulating_LODOnly");

		BSGraphics::BeginEvent(L"RenderLODLand");
		Accumulator->RenderFromMainGroup(1, BSSM_BLOOD_SPLATTER, RenderFlags, 0);
		BSGraphics::EndEvent();

		BSGraphics::BeginEvent(L"RenderLODObjects");
		Accumulator->RenderFromMainGroup(1, BSSM_BLOOD_SPLATTER, RenderFlags, 1);
		BSGraphics::EndEvent();
	}
	BSGraphics::EndEvent();

	BSShaderManager::bLODLandscapeNoise = true;
}

void BSShaderAccumulator::FinishAccumulating_Unknown1(BSShaderAccumulator *Accumulator, uint32_t RenderFlags)
{
	if (!Accumulator->m_pkCamera)
		return;

	ZoneScopedN("FinishAccumulating_Unknown1");
	BSGraphics::BeginEvent(L"FinishAccumulating_Unknown1");

	Accumulator->RenderFromMainGroup(1, BSSM_BLOOD_SPLATTER, RenderFlags, 14);

	BSGraphics::EndEvent();
}

void BSShaderAccumulator::RenderFromMainGroup(uint32_t StartTechnique, uint32_t EndTechnique, uint32_t RenderFlags, int GroupType)
{
	if (GroupType == -1)
	{
		RenderTechniques(StartTechnique, EndTechnique, RenderFlags, -1);
	}
	else
	{
		auto pass = m_MainBatch->m_Groups[GroupType];

		if (pass)
		{
			if (pass->UnkByte1 & 1)
				pass->Render(RenderFlags);
			else
				RenderTechniques(StartTechnique, EndTechnique, RenderFlags, GroupType);
		}
	}
}

void BSShaderAccumulator::RenderTechniques(uint32_t StartTechnique, uint32_t EndTechnique, uint32_t RenderFlags, int GroupType)
{
	Assert(GroupType >= -1 && GroupType < 16);
	Assert(StartTechnique <= EndTechnique);

	BSBatchRenderer::RenderGroup *group = nullptr;
	BSBatchRenderer *batch = nullptr;

	// Always run the full function because I'm not sure if the structure
	// is used somewhere important.
	MTRenderer::InsertCommand<MTRenderer::SetAccumulatorRenderCommand>(this);
	BSShaderManager::SetCurrentAccumulator(this);

	if (GroupType <= -1)
	{
		// Wildcard: render everything with the given techniques
		group = nullptr;
		batch = m_MainBatch;
	}
	else
	{
		// Render a single group with given techniques
		group = m_MainBatch->m_Groups[GroupType];
		batch = group->m_BatchRenderer;
	}

	m_CurrentTech = 0;

	if (batch)
	{
		auto *currentNode = &batch->m_UnknownList;

		batch->m_StartingTech = StartTechnique;
		batch->m_EndingTech = EndTechnique;

		m_CurrentGroupIndex = 0;
		m_HasPendingDraws = batch->sub_14131E700(m_CurrentTech, m_CurrentGroupIndex, (__int64)&currentNode);

		while (m_HasPendingDraws)
		{
			if (IsGrassShadowBlacklist(m_CurrentTech) && (m_UseUnknownCameraAdjust || *(BYTE *)((__int64)this + 297)))// if (is grass shadow) ???
				m_HasPendingDraws = batch->DiscardNextGroup(m_CurrentTech, m_CurrentGroupIndex, (__int64)&currentNode);
			else
				m_HasPendingDraws = batch->RenderNextGroup(m_CurrentTech, m_CurrentGroupIndex, (__int64)&currentNode, RenderFlags);
		}
	}
	else
	{
		// No batcher available to render these, so just drop them
		m_HasPendingDraws = false;
	}

	if (group)
		group->Unregister();

	MTRenderer::ClearShaderAndTechnique();
}