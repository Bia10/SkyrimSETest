#pragma once

#include "../../common.h"
#include "../TES/BSTArray.h"

struct z_stream_s;
class TESForm_CK;

struct PerkRankEntry
{
	union
	{
		struct
		{
			uint32_t FormId;	// 0x0
			uint8_t Rank;		// 0x4
		};

		struct
		{
			uint64_t FormIdOrPointer;	// 0x0
			uint8_t NewRank;			// 0x8
		};
	};
};
static_assert(sizeof(PerkRankEntry) == 0x10);

HWND WINAPI hk_CreateDialogParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
INT_PTR WINAPI hk_DialogBoxParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
BOOL WINAPI hk_EndDialog(HWND hDlg, INT_PTR nResult);
LRESULT WINAPI hk_SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

int hk_inflateInit(z_stream_s *Stream, const char *Version, int Mode);
int hk_inflate(z_stream_s *Stream, int Flush);
bool OpenPluginSaveDialog(HWND ParentWindow, const char *BasePath, bool IsESM, char *Buffer, uint32_t BufferSize, const char *Directory);
bool IsBSAVersionCurrent(class BSFile *File);

void CreateLipGenProcess(__int64 a1);
bool IsLipDataPresent(void *Thisptr);
bool WriteLipData(void *Thisptr, const char *Path, int Unkown1, bool Unknown2, bool Unknown3);
int IsWavDataPresent(const char *Path, __int64 a2, __int64 a3, __int64 a4);

uint32_t GetESLMasterCount();
const char *GetESLMasterName(uint32_t Index);
bool IsESLMaster(const char *Name);

uint32_t sub_1414974E0(BSTArray<void *>& Array, const void *&Target, uint32_t StartIndex, __int64 Unused);
uint32_t sub_1414974E0_SSE41(BSTArray<void *>& Array, const void *&Target, uint32_t StartIndex, __int64 Unused);
bool sub_1415D5640(__int64 a1, uint32_t *a2);

void UpdateLoadProgressBar();
void UpdateObjectWindowTreeView(void *Thisptr, HWND ControlHandle);
void UpdateCellViewCellList(void *Thisptr, HWND ControlHandle, __int64 Unknown);
void UpdateCellViewObjectList(void *Thisptr, HWND *ControlHandle);

void InsertComboBoxItem(HWND ComboBoxHandle, const char *DisplayText, void *Value, bool AllowResize);
void InsertListViewItem(HWND ListViewHandle, void *Parameter, bool UseImage, int ItemIndex);

void PatchTemplatedFormIterator();
void SortFormArray(BSTArray<TESForm_CK *> *Array, int(*SortFunction)(const void *, const void *));
void SortDialogueInfo(__int64 TESDataHandler, uint32_t FormType, int(*SortFunction)(const void *, const void *));

void QuitHandler();

void hk_sub_141047AB2(__int64 FileHandle, __int64 *Value);
bool hk_BGSPerkRankArray_sub_14168DF70(PerkRankEntry *Entry, uint32_t *FormId, __int64 UnknownArray);
void hk_BGSPerkRankArray_sub_14168EAE0(__int64 ArrayHandle, PerkRankEntry *&Entry);

void FaceGenOverflowWarning(__int64 Texture);
void ExportFaceGenForSelectedNPCs(__int64 a1, __int64 a2);

void hk_call_141C68FA6(TESForm_CK *DialogForm, __int64 Unused);
void *hk_call_141C26F3A(void *a1);
void hk_sub_141032ED7(__int64 a1, __int64 a2, __int64 a3);
void *hk_call_1417E42BF(void *a1);
HRESULT LoadTextureDataFromFile(__int64 a1, __int64 a2, __int64 a3, __int64 a4, unsigned int a5, int a6);
void hk_call_141C410A1(__int64 a1, class BSShaderProperty *Property);
void hk_sub_141B08540(__int64 DiskCRDT, __int64 SourceCRDT);
void hk_call_141B037B2(__int64 TESFile, __int64 SourceCRDT);
const char *hk_call_1417F4A04(int ActorValueIndex);
uint32_t sub_142647AC0(__int64 a1, bool *IterationFinished);
bool sub_142676020(const char *File, uint32_t *FileSize);
void hk_call_1412DD706(HWND WindowHandle, uint32_t *ControlId);
int sub_141BBF320(__int64 a1, __int64 a2);
void hk_call_141CF03C9(__int64 a1, bool Enable);
void hk_call_141CE8269(__int64 a1);
const char *hk_call_1416B849E(__int64 a1);
void hk_call_14135CDD3(__int64 RenderWindowInstance, uint32_t *UntypedPointerHandle, bool Select);
int hk_call_1412D1541(__int64 ObjectListInsertData, TESForm_CK *Form);
void hk_call_14147FB57(HWND ListViewHandle, TESForm_CK *Form, bool UseImage, int ItemIndex);
float hk_call_14202E0E8(float Delta);