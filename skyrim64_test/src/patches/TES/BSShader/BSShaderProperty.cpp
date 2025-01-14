#include "../../../common.h"
#include "../BSRenderPass.h"
#include "BSShaderProperty.h"

const uint32_t BSShaderProperty::UniqueFlagIndexes[15] =
{
	0x07, 0x26, 0x0B, 0x0A, 0x15, 0x12, 0x1C,
	0x0E, 0x21, 0x38, 0x3D, 0x22, 0x29, 0x3F,
	0x2E
};

const char *BSShaderProperty::UniqueFlagNames[15] =
{
	"Envmap",
	"Glowmap",
	"Parallax",
	"Facegen",
	"Facegen RGB Tint",
	"Hair Tint",
	"Parallax Occlusion",
	"Multitexture Landscape",
	"LOD Landscape",
	"Multilayer Parallax",
	"Tree Anim",
	"LOB Objects",
	"MultiIndex",
	"LOD Objects HD",
	"Multitexture Landscape (LOD Blend)"
};

const char *BSShaderProperty::FlagNames[64] =
{
	"specular",
	"skinned",
	"temp refraction",
	"vertex alpha",
	"grayscale to palette - color",
	"grayscale to palette - alpha",
	"falloff",
	"environment mapping",
	"receive shadows",
	"cast shadows",
	"facegen",
	"parallax",
	"model-space normals",
	"non-projective shadows",
	"landscape",
	"refraction",
	"fire refraction",
	"eye environment mapping",
	"hair",
	"screendoor alpha fade",
	"localmap hide secret",
	"facegen rgb tint",
	"own emit",
	"projected uv",
	"multiple textures",
	"remappable textures",
	"decal",
	"dynamic decal",
	"parallax occlusion",
	"external emittance",
	"soft effect",
	"zbuffer test",
	"zbuffer write",
	"lod landscape",
	"lod objects",
	"no fade",
	"two-sided geometry",
	"vertex colors",
	"glow map",
	"assume shadowmask",
	"character lighting",
	"multi index snow",
	"vertex lighting",
	"uniform scale",
	"fit slope",
	"billboard",
	"no lod land blend",
	"envmap light fade",
	"wireframe",
	"weapon blood",
	"hide on local map",
	"premult alpha",
	"cloud LOD",
	"anisotropic lighting",
	"no transparency multisampling",
	"unused01",
	"multi layer parallax",
	"soft lighting",
	"rim lighting",
	"back lighting",
	"snow",
	"tree anim",
	"effect lighting",
	"hd lod objects"
};

float BSShaderProperty::GetAlpha() const
{
	return fAlpha;
}

class BSFadeNode *BSShaderProperty::QFadeNode() const
{
	return pFadeNode;
}

const BSShaderProperty::RenderPassArray *BSShaderProperty::QRenderPasses() const
{
	return &kRenderPassList;
}

const BSShaderProperty::RenderPassArray *BSShaderProperty::QDebugRenderPasses() const
{
	return &kDebugRenderPassList;
}

uint64_t BSShaderProperty::QFlags() const
{
	return ulFlags;
}

bool BSShaderProperty::GetFlag(uint32_t FlagIndex) const
{
	if (FlagIndex >= BSSP_FLAG_COUNT)
		return false;

	return QFlags() & (1ull << FlagIndex);
}

void BSShaderProperty::GetViewerStrings(void(*Callback)(const char *, ...), bool Recursive) const
{
	if (Recursive)
		__super::GetViewerStrings(Callback, Recursive);

	Callback("-- BSShaderProperty --\n");

	for (int i = 0; i < BSSP_FLAG_COUNT; i++)
	{
		if (GetFlag(i))
			Callback("Flag = %s\n", FlagNames[i]);
	}

	if (fAlpha < 1.0f)
		Callback("Alpha = %g\n", GetAlpha());

	Callback("Last Render Pass State = %d\n", iLastRenderPassState);

	int passCount = 0;
	int debugPassCount = 0;

	for (auto pass = QRenderPasses()->pPassList; pass; pass = pass->m_Previous)
		passCount++;

	for (auto pass = QDebugRenderPasses()->pPassList; pass; pass = pass->m_Previous)
		debugPassCount++;

	Callback("Pass Count = %d\n", passCount);

	for (auto pass = QRenderPasses()->pPassList; pass; pass = pass->m_Previous)
		Callback("%s Pass\n", ((const char *(*)(uint32_t))(g_ModuleBase + 0x12ABF00))(pass->m_TechniqueID));

	Callback("Debug Pass Count = %d\n", debugPassCount);

	for (auto pass = QDebugRenderPasses()->pPassList; pass; pass = pass->m_Previous)
		Callback("%s Debug Pass\n", ((const char *(*)(uint32_t))(g_ModuleBase + 0x12ABF00))(pass->m_TechniqueID));
}

const char *BSShaderProperty::GetFlagString(uint32_t FlagIndex)
{
	if (FlagIndex >= BSSP_FLAG_COUNT)
		return nullptr;

	return FlagNames[FlagIndex];
}
