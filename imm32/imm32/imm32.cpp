
//
// created by AheadLib
// github:https://github.com/strivexjun/AheadLib-x86-x64
//
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <Shlwapi.h>
#include <fstream>
#include <string>

#include <imm.h>
#pragma comment (lib ,"imm32.lib")

#pragma comment( lib, "Shlwapi.lib")

#pragma comment(linker, "/EXPORT:CtfImmAppCompatEnableIMEonProtectedCode=AheadLib_CtfImmAppCompatEnableIMEonProtectedCode,@1")
#pragma comment(linker, "/EXPORT:CtfImmCoUninitialize=AheadLib_CtfImmCoUninitialize,@2")
#pragma comment(linker, "/EXPORT:CtfImmDispatchDefImeMessage=AheadLib_CtfImmDispatchDefImeMessage,@3")
#pragma comment(linker, "/EXPORT:CtfImmEnterCoInitCountSkipMode=AheadLib_CtfImmEnterCoInitCountSkipMode,@4")
#pragma comment(linker, "/EXPORT:CtfImmGenerateMessage=AheadLib_CtfImmGenerateMessage,@5")
#pragma comment(linker, "/EXPORT:CtfImmGetCompatibleKeyboardLayout=AheadLib_CtfImmGetCompatibleKeyboardLayout,@6")
#pragma comment(linker, "/EXPORT:CtfImmGetGlobalIMEStatus=AheadLib_CtfImmGetGlobalIMEStatus,@7")
#pragma comment(linker, "/EXPORT:CtfImmGetGuidAtom=AheadLib_CtfImmGetGuidAtom,@8")
#pragma comment(linker, "/EXPORT:CtfImmGetIMEFileName=AheadLib_CtfImmGetIMEFileName,@9")
#pragma comment(linker, "/EXPORT:CtfImmGetTMAEFlags=AheadLib_CtfImmGetTMAEFlags,@10")
#pragma comment(linker, "/EXPORT:CtfImmHideToolbarWnd=AheadLib_CtfImmHideToolbarWnd,@11")
#pragma comment(linker, "/EXPORT:CtfImmIsCiceroEnabled=AheadLib_CtfImmIsCiceroEnabled,@12")
#pragma comment(linker, "/EXPORT:CtfImmIsCiceroStartedInThread=AheadLib_CtfImmIsCiceroStartedInThread,@13")
#pragma comment(linker, "/EXPORT:CtfImmIsComStartedInThread=AheadLib_CtfImmIsComStartedInThread,@14")
#pragma comment(linker, "/EXPORT:CtfImmIsGuidMapEnable=AheadLib_CtfImmIsGuidMapEnable,@15")
#pragma comment(linker, "/EXPORT:CtfImmIsTextFrameServiceDisabled=AheadLib_CtfImmIsTextFrameServiceDisabled,@16")
#pragma comment(linker, "/EXPORT:CtfImmLastEnabledWndDestroy=AheadLib_CtfImmLastEnabledWndDestroy,@17")
#pragma comment(linker, "/EXPORT:CtfImmLeaveCoInitCountSkipMode=AheadLib_CtfImmLeaveCoInitCountSkipMode,@18")
#pragma comment(linker, "/EXPORT:CtfImmNotify=AheadLib_CtfImmNotify,@19")
#pragma comment(linker, "/EXPORT:CtfImmRestoreToolbarWnd=AheadLib_CtfImmRestoreToolbarWnd,@20")
#pragma comment(linker, "/EXPORT:CtfImmSetAppCompatFlags=AheadLib_CtfImmSetAppCompatFlags,@21")
#pragma comment(linker, "/EXPORT:CtfImmSetCiceroStartInThread=AheadLib_CtfImmSetCiceroStartInThread,@22")
#pragma comment(linker, "/EXPORT:CtfImmSetDefaultRemoteKeyboardLayout=AheadLib_CtfImmSetDefaultRemoteKeyboardLayout,@23")
#pragma comment(linker, "/EXPORT:CtfImmTIMActivate=AheadLib_CtfImmTIMActivate,@24")
#pragma comment(linker, "/EXPORT:GetKeyboardLayoutCP=AheadLib_GetKeyboardLayoutCP,@25")
#pragma comment(linker, "/EXPORT:ImmActivateLayout=AheadLib_ImmActivateLayout,@26")
#pragma comment(linker, "/EXPORT:ImmAssociateContext=AheadLib_ImmAssociateContext,@27")
#pragma comment(linker, "/EXPORT:ImmAssociateContextEx=AheadLib_ImmAssociateContextEx,@28")
#pragma comment(linker, "/EXPORT:ImmCallImeConsoleIME=AheadLib_ImmCallImeConsoleIME,@29")
#pragma comment(linker, "/EXPORT:ImmConfigureIMEA=AheadLib_ImmConfigureIMEA,@30")
#pragma comment(linker, "/EXPORT:ImmConfigureIMEW=AheadLib_ImmConfigureIMEW,@31")
#pragma comment(linker, "/EXPORT:ImmCreateContext=AheadLib_ImmCreateContext,@32")
#pragma comment(linker, "/EXPORT:ImmCreateIMCC=AheadLib_ImmCreateIMCC,@33")
#pragma comment(linker, "/EXPORT:ImmCreateSoftKeyboard=AheadLib_ImmCreateSoftKeyboard,@34")
#pragma comment(linker, "/EXPORT:ImmDestroyContext=AheadLib_ImmDestroyContext,@35")
#pragma comment(linker, "/EXPORT:ImmDestroyIMCC=AheadLib_ImmDestroyIMCC,@36")
#pragma comment(linker, "/EXPORT:ImmDestroySoftKeyboard=AheadLib_ImmDestroySoftKeyboard,@37")
#pragma comment(linker, "/EXPORT:ImmDisableIME=AheadLib_ImmDisableIME,@38")
#pragma comment(linker, "/EXPORT:ImmDisableIme=AheadLib_ImmDisableIme,@39")
#pragma comment(linker, "/EXPORT:ImmDisableLegacyIME=AheadLib_ImmDisableLegacyIME,@40")
#pragma comment(linker, "/EXPORT:ImmDisableTextFrameService=AheadLib_ImmDisableTextFrameService,@41")
#pragma comment(linker, "/EXPORT:ImmEnumInputContext=AheadLib_ImmEnumInputContext,@42")
#pragma comment(linker, "/EXPORT:ImmEnumRegisterWordA=AheadLib_ImmEnumRegisterWordA,@43")
#pragma comment(linker, "/EXPORT:ImmEnumRegisterWordW=AheadLib_ImmEnumRegisterWordW,@44")
#pragma comment(linker, "/EXPORT:ImmEscapeA=AheadLib_ImmEscapeA,@45")
#pragma comment(linker, "/EXPORT:ImmEscapeW=AheadLib_ImmEscapeW,@46")
#pragma comment(linker, "/EXPORT:ImmFreeLayout=AheadLib_ImmFreeLayout,@47")
#pragma comment(linker, "/EXPORT:ImmGenerateMessage=AheadLib_ImmGenerateMessage,@48")
#pragma comment(linker, "/EXPORT:ImmGetAppCompatFlags=AheadLib_ImmGetAppCompatFlags,@49")
#pragma comment(linker, "/EXPORT:ImmGetCandidateListA=AheadLib_ImmGetCandidateListA,@50")
#pragma comment(linker, "/EXPORT:ImmGetCandidateListCountA=AheadLib_ImmGetCandidateListCountA,@51")
#pragma comment(linker, "/EXPORT:ImmGetCandidateListCountW=AheadLib_ImmGetCandidateListCountW,@52")
#pragma comment(linker, "/EXPORT:ImmGetCandidateListW=AheadLib_ImmGetCandidateListW,@53")
#pragma comment(linker, "/EXPORT:ImmGetCandidateWindow=AheadLib_ImmGetCandidateWindow,@54")
#pragma comment(linker, "/EXPORT:ImmGetCompositionFontA=AheadLib_ImmGetCompositionFontA,@55")
#pragma comment(linker, "/EXPORT:ImmGetCompositionFontW=AheadLib_ImmGetCompositionFontW,@56")
#pragma comment(linker, "/EXPORT:ImmGetCompositionStringA=AheadLib_ImmGetCompositionStringA,@57")
#pragma comment(linker, "/EXPORT:ImmGetCompositionStringW=AheadLib_ImmGetCompositionStringW,@58")
#pragma comment(linker, "/EXPORT:ImmGetCompositionWindow=AheadLib_ImmGetCompositionWindow,@59")
#pragma comment(linker, "/EXPORT:ImmGetContext=AheadLib_ImmGetContext,@60")
#pragma comment(linker, "/EXPORT:ImmGetConversionListA=AheadLib_ImmGetConversionListA,@61")
#pragma comment(linker, "/EXPORT:ImmGetConversionListW=AheadLib_ImmGetConversionListW,@62")
#pragma comment(linker, "/EXPORT:ImmGetConversionStatus=AheadLib_ImmGetConversionStatus,@63")
#pragma comment(linker, "/EXPORT:ImmGetDefaultIMEWnd=AheadLib_ImmGetDefaultIMEWnd,@64")
#pragma comment(linker, "/EXPORT:ImmGetDescriptionA=AheadLib_ImmGetDescriptionA,@65")
#pragma comment(linker, "/EXPORT:ImmGetDescriptionW=AheadLib_ImmGetDescriptionW,@66")
#pragma comment(linker, "/EXPORT:ImmGetGuideLineA=AheadLib_ImmGetGuideLineA,@67")
#pragma comment(linker, "/EXPORT:ImmGetGuideLineW=AheadLib_ImmGetGuideLineW,@68")
#pragma comment(linker, "/EXPORT:ImmGetHotKey=AheadLib_ImmGetHotKey,@69")
#pragma comment(linker, "/EXPORT:ImmGetIMCCLockCount=AheadLib_ImmGetIMCCLockCount,@70")
#pragma comment(linker, "/EXPORT:ImmGetIMCCSize=AheadLib_ImmGetIMCCSize,@71")
#pragma comment(linker, "/EXPORT:ImmGetIMCLockCount=AheadLib_ImmGetIMCLockCount,@72")
#pragma comment(linker, "/EXPORT:ImmGetIMEFileNameA=AheadLib_ImmGetIMEFileNameA,@73")
#pragma comment(linker, "/EXPORT:ImmGetIMEFileNameW=AheadLib_ImmGetIMEFileNameW,@74")
#pragma comment(linker, "/EXPORT:ImmGetImeInfoEx=AheadLib_ImmGetImeInfoEx,@75")
#pragma comment(linker, "/EXPORT:ImmGetImeMenuItemsA=AheadLib_ImmGetImeMenuItemsA,@76")
#pragma comment(linker, "/EXPORT:ImmGetImeMenuItemsW=AheadLib_ImmGetImeMenuItemsW,@77")
#pragma comment(linker, "/EXPORT:ImmGetOpenStatus=AheadLib_ImmGetOpenStatus,@78")
#pragma comment(linker, "/EXPORT:ImmGetProperty=AheadLib_ImmGetProperty,@79")
#pragma comment(linker, "/EXPORT:ImmGetRegisterWordStyleA=AheadLib_ImmGetRegisterWordStyleA,@80")
#pragma comment(linker, "/EXPORT:ImmGetRegisterWordStyleW=AheadLib_ImmGetRegisterWordStyleW,@81")
#pragma comment(linker, "/EXPORT:ImmGetStatusWindowPos=AheadLib_ImmGetStatusWindowPos,@82")
#pragma comment(linker, "/EXPORT:ImmGetVirtualKey=AheadLib_ImmGetVirtualKey,@83")
#pragma comment(linker, "/EXPORT:ImmIMPGetIMEA=AheadLib_ImmIMPGetIMEA,@84")
#pragma comment(linker, "/EXPORT:ImmIMPGetIMEW=AheadLib_ImmIMPGetIMEW,@85")
#pragma comment(linker, "/EXPORT:ImmIMPQueryIMEA=AheadLib_ImmIMPQueryIMEA,@86")
#pragma comment(linker, "/EXPORT:ImmIMPQueryIMEW=AheadLib_ImmIMPQueryIMEW,@87")
#pragma comment(linker, "/EXPORT:ImmIMPSetIMEA=AheadLib_ImmIMPSetIMEA,@88")
#pragma comment(linker, "/EXPORT:ImmIMPSetIMEW=AheadLib_ImmIMPSetIMEW,@89")
#pragma comment(linker, "/EXPORT:ImmInstallIMEA=AheadLib_ImmInstallIMEA,@90")
#pragma comment(linker, "/EXPORT:ImmInstallIMEW=AheadLib_ImmInstallIMEW,@91")
#pragma comment(linker, "/EXPORT:ImmIsIME=AheadLib_ImmIsIME,@92")
#pragma comment(linker, "/EXPORT:ImmIsUIMessageA=AheadLib_ImmIsUIMessageA,@93")
#pragma comment(linker, "/EXPORT:ImmIsUIMessageW=AheadLib_ImmIsUIMessageW,@94")
#pragma comment(linker, "/EXPORT:ImmLoadIME=AheadLib_ImmLoadIME,@95")
#pragma comment(linker, "/EXPORT:ImmLoadLayout=AheadLib_ImmLoadLayout,@96")
#pragma comment(linker, "/EXPORT:ImmLockClientImc=AheadLib_ImmLockClientImc,@97")
#pragma comment(linker, "/EXPORT:ImmLockIMC=AheadLib_ImmLockIMC,@98")
#pragma comment(linker, "/EXPORT:ImmLockIMCC=AheadLib_ImmLockIMCC,@99")
#pragma comment(linker, "/EXPORT:ImmLockImeDpi=AheadLib_ImmLockImeDpi,@100")
#pragma comment(linker, "/EXPORT:ImmNotifyIME=AheadLib_ImmNotifyIME,@101")
#pragma comment(linker, "/EXPORT:ImmProcessKey=AheadLib_ImmProcessKey,@102")
#pragma comment(linker, "/EXPORT:ImmPutImeMenuItemsIntoMappedFile=AheadLib_ImmPutImeMenuItemsIntoMappedFile,@103")
#pragma comment(linker, "/EXPORT:ImmReSizeIMCC=AheadLib_ImmReSizeIMCC,@104")
#pragma comment(linker, "/EXPORT:ImmRegisterClient=AheadLib_ImmRegisterClient,@105")
#pragma comment(linker, "/EXPORT:ImmRegisterWordA=AheadLib_ImmRegisterWordA,@106")
#pragma comment(linker, "/EXPORT:ImmRegisterWordW=AheadLib_ImmRegisterWordW,@107")
#pragma comment(linker, "/EXPORT:ImmReleaseContext=AheadLib_ImmReleaseContext,@108")
#pragma comment(linker, "/EXPORT:ImmRequestMessageA=AheadLib_ImmRequestMessageA,@109")
#pragma comment(linker, "/EXPORT:ImmRequestMessageW=AheadLib_ImmRequestMessageW,@110")
#pragma comment(linker, "/EXPORT:ImmSendIMEMessageExA=AheadLib_ImmSendIMEMessageExA,@111")
#pragma comment(linker, "/EXPORT:ImmSendIMEMessageExW=AheadLib_ImmSendIMEMessageExW,@112")
#pragma comment(linker, "/EXPORT:ImmSetActiveContext=AheadLib_ImmSetActiveContext,@113")
#pragma comment(linker, "/EXPORT:ImmSetActiveContextConsoleIME=AheadLib_ImmSetActiveContextConsoleIME,@114")
#pragma comment(linker, "/EXPORT:ImmSetCandidateWindow=AheadLib_ImmSetCandidateWindow,@115")
#pragma comment(linker, "/EXPORT:ImmSetCompositionFontA=AheadLib_ImmSetCompositionFontA,@116")
#pragma comment(linker, "/EXPORT:ImmSetCompositionFontW=AheadLib_ImmSetCompositionFontW,@117")
#pragma comment(linker, "/EXPORT:ImmSetCompositionStringA=AheadLib_ImmSetCompositionStringA,@118")
#pragma comment(linker, "/EXPORT:ImmSetCompositionStringW=AheadLib_ImmSetCompositionStringW,@119")
#pragma comment(linker, "/EXPORT:ImmSetCompositionWindow=AheadLib_ImmSetCompositionWindow,@120")
#pragma comment(linker, "/EXPORT:ImmSetConversionStatus=AheadLib_ImmSetConversionStatus,@121")
#pragma comment(linker, "/EXPORT:ImmSetHotKey=USER32.CliImmSetHotKey,@122")
#pragma comment(linker, "/EXPORT:ImmSetOpenStatus=AheadLib_ImmSetOpenStatus,@123")
#pragma comment(linker, "/EXPORT:ImmSetStatusWindowPos=AheadLib_ImmSetStatusWindowPos,@124")
#pragma comment(linker, "/EXPORT:ImmShowSoftKeyboard=AheadLib_ImmShowSoftKeyboard,@125")
#pragma comment(linker, "/EXPORT:ImmSimulateHotKey=AheadLib_ImmSimulateHotKey,@126")
#pragma comment(linker, "/EXPORT:ImmSystemHandler=AheadLib_ImmSystemHandler,@127")
#pragma comment(linker, "/EXPORT:ImmTranslateMessage=AheadLib_ImmTranslateMessage,@128")
#pragma comment(linker, "/EXPORT:ImmUnlockClientImc=AheadLib_ImmUnlockClientImc,@129")
#pragma comment(linker, "/EXPORT:ImmUnlockIMC=AheadLib_ImmUnlockIMC,@130")
#pragma comment(linker, "/EXPORT:ImmUnlockIMCC=AheadLib_ImmUnlockIMCC,@131")
#pragma comment(linker, "/EXPORT:ImmUnlockImeDpi=AheadLib_ImmUnlockImeDpi,@132")
#pragma comment(linker, "/EXPORT:ImmUnregisterWordA=AheadLib_ImmUnregisterWordA,@133")
#pragma comment(linker, "/EXPORT:ImmUnregisterWordW=AheadLib_ImmUnregisterWordW,@134")
#pragma comment(linker, "/EXPORT:ImmWINNLSEnableIME=AheadLib_ImmWINNLSEnableIME,@135")
#pragma comment(linker, "/EXPORT:ImmWINNLSGetEnableStatus=AheadLib_ImmWINNLSGetEnableStatus,@136")
#pragma comment(linker, "/EXPORT:ImmWINNLSGetIMEHotkey=AheadLib_ImmWINNLSGetIMEHotkey,@137")


extern "C" 
{
PVOID pfnAheadLib_CtfImmAppCompatEnableIMEonProtectedCode;
PVOID pfnAheadLib_CtfImmCoUninitialize;
PVOID pfnAheadLib_CtfImmDispatchDefImeMessage;
PVOID pfnAheadLib_CtfImmEnterCoInitCountSkipMode;
PVOID pfnAheadLib_CtfImmGenerateMessage;
PVOID pfnAheadLib_CtfImmGetCompatibleKeyboardLayout;
PVOID pfnAheadLib_CtfImmGetGlobalIMEStatus;
PVOID pfnAheadLib_CtfImmGetGuidAtom;
PVOID pfnAheadLib_CtfImmGetIMEFileName;
PVOID pfnAheadLib_CtfImmGetTMAEFlags;
PVOID pfnAheadLib_CtfImmHideToolbarWnd;
PVOID pfnAheadLib_CtfImmIsCiceroEnabled;
PVOID pfnAheadLib_CtfImmIsCiceroStartedInThread;
PVOID pfnAheadLib_CtfImmIsComStartedInThread;
PVOID pfnAheadLib_CtfImmIsGuidMapEnable;
PVOID pfnAheadLib_CtfImmIsTextFrameServiceDisabled;
PVOID pfnAheadLib_CtfImmLastEnabledWndDestroy;
PVOID pfnAheadLib_CtfImmLeaveCoInitCountSkipMode;
PVOID pfnAheadLib_CtfImmNotify;
PVOID pfnAheadLib_CtfImmRestoreToolbarWnd;
PVOID pfnAheadLib_CtfImmSetAppCompatFlags;
PVOID pfnAheadLib_CtfImmSetCiceroStartInThread;
PVOID pfnAheadLib_CtfImmSetDefaultRemoteKeyboardLayout;
PVOID pfnAheadLib_CtfImmTIMActivate;
PVOID pfnAheadLib_GetKeyboardLayoutCP;
PVOID pfnAheadLib_ImmActivateLayout;
PVOID pfnAheadLib_ImmAssociateContext;
PVOID pfnAheadLib_ImmAssociateContextEx;
PVOID pfnAheadLib_ImmCallImeConsoleIME;
PVOID pfnAheadLib_ImmConfigureIMEA;
PVOID pfnAheadLib_ImmConfigureIMEW;
PVOID pfnAheadLib_ImmCreateContext;
PVOID pfnAheadLib_ImmCreateIMCC;
PVOID pfnAheadLib_ImmCreateSoftKeyboard;
PVOID pfnAheadLib_ImmDestroyContext;
PVOID pfnAheadLib_ImmDestroyIMCC;
PVOID pfnAheadLib_ImmDestroySoftKeyboard;
PVOID pfnAheadLib_ImmDisableIME;
PVOID pfnAheadLib_ImmDisableIme;
PVOID pfnAheadLib_ImmDisableLegacyIME;
PVOID pfnAheadLib_ImmDisableTextFrameService;
PVOID pfnAheadLib_ImmEnumInputContext;
PVOID pfnAheadLib_ImmEnumRegisterWordA;
PVOID pfnAheadLib_ImmEnumRegisterWordW;
PVOID pfnAheadLib_ImmEscapeA;
PVOID pfnAheadLib_ImmEscapeW;
PVOID pfnAheadLib_ImmFreeLayout;
PVOID pfnAheadLib_ImmGenerateMessage;
PVOID pfnAheadLib_ImmGetAppCompatFlags;
PVOID pfnAheadLib_ImmGetCandidateListA;
PVOID pfnAheadLib_ImmGetCandidateListCountA;
PVOID pfnAheadLib_ImmGetCandidateListCountW;
PVOID pfnAheadLib_ImmGetCandidateListW;
PVOID pfnAheadLib_ImmGetCandidateWindow;
PVOID pfnAheadLib_ImmGetCompositionFontA;
PVOID pfnAheadLib_ImmGetCompositionFontW;
PVOID pfnAheadLib_ImmGetCompositionStringA;
PVOID pfnAheadLib_ImmGetCompositionStringW;
PVOID pfnAheadLib_ImmGetCompositionWindow;
PVOID pfnAheadLib_ImmGetContext;
PVOID pfnAheadLib_ImmGetConversionListA;
PVOID pfnAheadLib_ImmGetConversionListW;
PVOID pfnAheadLib_ImmGetConversionStatus;
PVOID pfnAheadLib_ImmGetDefaultIMEWnd;
PVOID pfnAheadLib_ImmGetDescriptionA;
PVOID pfnAheadLib_ImmGetDescriptionW;
PVOID pfnAheadLib_ImmGetGuideLineA;
PVOID pfnAheadLib_ImmGetGuideLineW;
PVOID pfnAheadLib_ImmGetHotKey;
PVOID pfnAheadLib_ImmGetIMCCLockCount;
PVOID pfnAheadLib_ImmGetIMCCSize;
PVOID pfnAheadLib_ImmGetIMCLockCount;
PVOID pfnAheadLib_ImmGetIMEFileNameA;
PVOID pfnAheadLib_ImmGetIMEFileNameW;
PVOID pfnAheadLib_ImmGetImeInfoEx;
PVOID pfnAheadLib_ImmGetImeMenuItemsA;
PVOID pfnAheadLib_ImmGetImeMenuItemsW;
PVOID pfnAheadLib_ImmGetOpenStatus;
PVOID pfnAheadLib_ImmGetProperty;
PVOID pfnAheadLib_ImmGetRegisterWordStyleA;
PVOID pfnAheadLib_ImmGetRegisterWordStyleW;
PVOID pfnAheadLib_ImmGetStatusWindowPos;
PVOID pfnAheadLib_ImmGetVirtualKey;
PVOID pfnAheadLib_ImmIMPGetIMEA;
PVOID pfnAheadLib_ImmIMPGetIMEW;
PVOID pfnAheadLib_ImmIMPQueryIMEA;
PVOID pfnAheadLib_ImmIMPQueryIMEW;
PVOID pfnAheadLib_ImmIMPSetIMEA;
PVOID pfnAheadLib_ImmIMPSetIMEW;
PVOID pfnAheadLib_ImmInstallIMEA;
PVOID pfnAheadLib_ImmInstallIMEW;
PVOID pfnAheadLib_ImmIsIME;
PVOID pfnAheadLib_ImmIsUIMessageA;
PVOID pfnAheadLib_ImmIsUIMessageW;
PVOID pfnAheadLib_ImmLoadIME;
PVOID pfnAheadLib_ImmLoadLayout;
PVOID pfnAheadLib_ImmLockClientImc;
PVOID pfnAheadLib_ImmLockIMC;
PVOID pfnAheadLib_ImmLockIMCC;
PVOID pfnAheadLib_ImmLockImeDpi;
PVOID pfnAheadLib_ImmNotifyIME;
PVOID pfnAheadLib_ImmProcessKey;
PVOID pfnAheadLib_ImmPutImeMenuItemsIntoMappedFile;
PVOID pfnAheadLib_ImmReSizeIMCC;
PVOID pfnAheadLib_ImmRegisterClient;
PVOID pfnAheadLib_ImmRegisterWordA;
PVOID pfnAheadLib_ImmRegisterWordW;
PVOID pfnAheadLib_ImmReleaseContext;
PVOID pfnAheadLib_ImmRequestMessageA;
PVOID pfnAheadLib_ImmRequestMessageW;
PVOID pfnAheadLib_ImmSendIMEMessageExA;
PVOID pfnAheadLib_ImmSendIMEMessageExW;
PVOID pfnAheadLib_ImmSetActiveContext;
PVOID pfnAheadLib_ImmSetActiveContextConsoleIME;
PVOID pfnAheadLib_ImmSetCandidateWindow;
PVOID pfnAheadLib_ImmSetCompositionFontA;
PVOID pfnAheadLib_ImmSetCompositionFontW;
PVOID pfnAheadLib_ImmSetCompositionStringA;
PVOID pfnAheadLib_ImmSetCompositionStringW;
PVOID pfnAheadLib_ImmSetCompositionWindow;
PVOID pfnAheadLib_ImmSetConversionStatus;
PVOID pfnAheadLib_ImmSetOpenStatus;
PVOID pfnAheadLib_ImmSetStatusWindowPos;
PVOID pfnAheadLib_ImmShowSoftKeyboard;
PVOID pfnAheadLib_ImmSimulateHotKey;
PVOID pfnAheadLib_ImmSystemHandler;
PVOID pfnAheadLib_ImmTranslateMessage;
PVOID pfnAheadLib_ImmUnlockClientImc;
PVOID pfnAheadLib_ImmUnlockIMC;
PVOID pfnAheadLib_ImmUnlockIMCC;
PVOID pfnAheadLib_ImmUnlockImeDpi;
PVOID pfnAheadLib_ImmUnregisterWordA;
PVOID pfnAheadLib_ImmUnregisterWordW;
PVOID pfnAheadLib_ImmWINNLSEnableIME;
PVOID pfnAheadLib_ImmWINNLSGetEnableStatus;
PVOID pfnAheadLib_ImmWINNLSGetIMEHotkey;
}


static
HMODULE g_OldModule = NULL;
HKL m_Oldhkl;

VOID WINAPI Free()
{
	if (g_OldModule)
	{
		FreeLibrary(g_OldModule);
	}
}


BOOL WINAPI Load()
{
	TCHAR tzPath[MAX_PATH];
	TCHAR tzTemp[MAX_PATH * 2];

	//
	// 这里是否从系统目录或当前目录加载原始DLL
	//
	//GetModuleFileName(NULL,tzPath,MAX_PATH); //获取本目录下的
	//PathRemoveFileSpec(tzPath);

	GetSystemDirectory(tzPath, MAX_PATH); //默认获取系统目录的

	lstrcat(tzPath, TEXT("\\imm32.dll"));

	g_OldModule = LoadLibrary(tzPath);
	if (g_OldModule == NULL)
	{
		wsprintf(tzTemp, TEXT("无法找到模块 %s,程序无法正常运行"), tzPath);
		MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
	}

	return (g_OldModule != NULL);

}


FARPROC WINAPI GetAddress(PCSTR pszProcName)
{
	FARPROC fpAddress;
	CHAR szProcName[64];
	TCHAR tzTemp[MAX_PATH];

	fpAddress = GetProcAddress(g_OldModule, pszProcName);
	if (fpAddress == NULL)
	{
		if (HIWORD(pszProcName) == 0)
		{
			wsprintfA(szProcName, "#%d", pszProcName);
			pszProcName = szProcName;
		}

		wsprintf(tzTemp, TEXT("无法找到函数 %hs,程序无法正常运行"), pszProcName);
		MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
		ExitProcess(-2);
	}
	return fpAddress;
}

BOOL WINAPI Init()
{
	pfnAheadLib_CtfImmAppCompatEnableIMEonProtectedCode = GetAddress("CtfImmAppCompatEnableIMEonProtectedCode");
	pfnAheadLib_CtfImmCoUninitialize = GetAddress("CtfImmCoUninitialize");
	pfnAheadLib_CtfImmDispatchDefImeMessage = GetAddress("CtfImmDispatchDefImeMessage");
	pfnAheadLib_CtfImmEnterCoInitCountSkipMode = GetAddress("CtfImmEnterCoInitCountSkipMode");
	pfnAheadLib_CtfImmGenerateMessage = GetAddress("CtfImmGenerateMessage");
	pfnAheadLib_CtfImmGetCompatibleKeyboardLayout = GetAddress("CtfImmGetCompatibleKeyboardLayout");
	pfnAheadLib_CtfImmGetGlobalIMEStatus = GetAddress("CtfImmGetGlobalIMEStatus");
	pfnAheadLib_CtfImmGetGuidAtom = GetAddress("CtfImmGetGuidAtom");
	pfnAheadLib_CtfImmGetIMEFileName = GetAddress("CtfImmGetIMEFileName");
	pfnAheadLib_CtfImmGetTMAEFlags = GetAddress("CtfImmGetTMAEFlags");
	pfnAheadLib_CtfImmHideToolbarWnd = GetAddress("CtfImmHideToolbarWnd");
	pfnAheadLib_CtfImmIsCiceroEnabled = GetAddress("CtfImmIsCiceroEnabled");
	pfnAheadLib_CtfImmIsCiceroStartedInThread = GetAddress("CtfImmIsCiceroStartedInThread");
	pfnAheadLib_CtfImmIsComStartedInThread = GetAddress("CtfImmIsComStartedInThread");
	pfnAheadLib_CtfImmIsGuidMapEnable = GetAddress("CtfImmIsGuidMapEnable");
	pfnAheadLib_CtfImmIsTextFrameServiceDisabled = GetAddress("CtfImmIsTextFrameServiceDisabled");
	pfnAheadLib_CtfImmLastEnabledWndDestroy = GetAddress("CtfImmLastEnabledWndDestroy");
	pfnAheadLib_CtfImmLeaveCoInitCountSkipMode = GetAddress("CtfImmLeaveCoInitCountSkipMode");
	pfnAheadLib_CtfImmNotify = GetAddress("CtfImmNotify");
	pfnAheadLib_CtfImmRestoreToolbarWnd = GetAddress("CtfImmRestoreToolbarWnd");
	pfnAheadLib_CtfImmSetAppCompatFlags = GetAddress("CtfImmSetAppCompatFlags");
	pfnAheadLib_CtfImmSetCiceroStartInThread = GetAddress("CtfImmSetCiceroStartInThread");
	pfnAheadLib_CtfImmSetDefaultRemoteKeyboardLayout = GetAddress("CtfImmSetDefaultRemoteKeyboardLayout");
	pfnAheadLib_CtfImmTIMActivate = GetAddress("CtfImmTIMActivate");
	pfnAheadLib_GetKeyboardLayoutCP = GetAddress("GetKeyboardLayoutCP");
	pfnAheadLib_ImmActivateLayout = GetAddress("ImmActivateLayout");
	pfnAheadLib_ImmAssociateContext = GetAddress("ImmAssociateContext");
	pfnAheadLib_ImmAssociateContextEx = GetAddress("ImmAssociateContextEx");
	pfnAheadLib_ImmCallImeConsoleIME = GetAddress("ImmCallImeConsoleIME");
	pfnAheadLib_ImmConfigureIMEA = GetAddress("ImmConfigureIMEA");
	pfnAheadLib_ImmConfigureIMEW = GetAddress("ImmConfigureIMEW");
	pfnAheadLib_ImmCreateContext = GetAddress("ImmCreateContext");
	pfnAheadLib_ImmCreateIMCC = GetAddress("ImmCreateIMCC");
	pfnAheadLib_ImmCreateSoftKeyboard = GetAddress("ImmCreateSoftKeyboard");
	pfnAheadLib_ImmDestroyContext = GetAddress("ImmDestroyContext");
	pfnAheadLib_ImmDestroyIMCC = GetAddress("ImmDestroyIMCC");
	pfnAheadLib_ImmDestroySoftKeyboard = GetAddress("ImmDestroySoftKeyboard");
	pfnAheadLib_ImmDisableIME = GetAddress("ImmDisableIME");
	pfnAheadLib_ImmDisableIme = GetAddress("ImmDisableIme");
	pfnAheadLib_ImmDisableLegacyIME = GetAddress("ImmDisableLegacyIME");
	pfnAheadLib_ImmDisableTextFrameService = GetAddress("ImmDisableTextFrameService");
	pfnAheadLib_ImmEnumInputContext = GetAddress("ImmEnumInputContext");
	pfnAheadLib_ImmEnumRegisterWordA = GetAddress("ImmEnumRegisterWordA");
	pfnAheadLib_ImmEnumRegisterWordW = GetAddress("ImmEnumRegisterWordW");
	pfnAheadLib_ImmEscapeA = GetAddress("ImmEscapeA");
	pfnAheadLib_ImmEscapeW = GetAddress("ImmEscapeW");
	pfnAheadLib_ImmFreeLayout = GetAddress("ImmFreeLayout");
	pfnAheadLib_ImmGenerateMessage = GetAddress("ImmGenerateMessage");
	pfnAheadLib_ImmGetAppCompatFlags = GetAddress("ImmGetAppCompatFlags");
	pfnAheadLib_ImmGetCandidateListA = GetAddress("ImmGetCandidateListA");
	pfnAheadLib_ImmGetCandidateListCountA = GetAddress("ImmGetCandidateListCountA");
	pfnAheadLib_ImmGetCandidateListCountW = GetAddress("ImmGetCandidateListCountW");
	pfnAheadLib_ImmGetCandidateListW = GetAddress("ImmGetCandidateListW");
	pfnAheadLib_ImmGetCandidateWindow = GetAddress("ImmGetCandidateWindow");
	pfnAheadLib_ImmGetCompositionFontA = GetAddress("ImmGetCompositionFontA");
	pfnAheadLib_ImmGetCompositionFontW = GetAddress("ImmGetCompositionFontW");
	pfnAheadLib_ImmGetCompositionStringA = GetAddress("ImmGetCompositionStringA");
	pfnAheadLib_ImmGetCompositionStringW = GetAddress("ImmGetCompositionStringW");
	pfnAheadLib_ImmGetCompositionWindow = GetAddress("ImmGetCompositionWindow");
	pfnAheadLib_ImmGetContext = GetAddress("ImmGetContext");
	pfnAheadLib_ImmGetConversionListA = GetAddress("ImmGetConversionListA");
	pfnAheadLib_ImmGetConversionListW = GetAddress("ImmGetConversionListW");
	pfnAheadLib_ImmGetConversionStatus = GetAddress("ImmGetConversionStatus");
	pfnAheadLib_ImmGetDefaultIMEWnd = GetAddress("ImmGetDefaultIMEWnd");
	pfnAheadLib_ImmGetDescriptionA = GetAddress("ImmGetDescriptionA");
	pfnAheadLib_ImmGetDescriptionW = GetAddress("ImmGetDescriptionW");
	pfnAheadLib_ImmGetGuideLineA = GetAddress("ImmGetGuideLineA");
	pfnAheadLib_ImmGetGuideLineW = GetAddress("ImmGetGuideLineW");
	pfnAheadLib_ImmGetHotKey = GetAddress("ImmGetHotKey");
	pfnAheadLib_ImmGetIMCCLockCount = GetAddress("ImmGetIMCCLockCount");
	pfnAheadLib_ImmGetIMCCSize = GetAddress("ImmGetIMCCSize");
	pfnAheadLib_ImmGetIMCLockCount = GetAddress("ImmGetIMCLockCount");
	pfnAheadLib_ImmGetIMEFileNameA = GetAddress("ImmGetIMEFileNameA");
	pfnAheadLib_ImmGetIMEFileNameW = GetAddress("ImmGetIMEFileNameW");
	pfnAheadLib_ImmGetImeInfoEx = GetAddress("ImmGetImeInfoEx");
	pfnAheadLib_ImmGetImeMenuItemsA = GetAddress("ImmGetImeMenuItemsA");
	pfnAheadLib_ImmGetImeMenuItemsW = GetAddress("ImmGetImeMenuItemsW");
	pfnAheadLib_ImmGetOpenStatus = GetAddress("ImmGetOpenStatus");
	pfnAheadLib_ImmGetProperty = GetAddress("ImmGetProperty");
	pfnAheadLib_ImmGetRegisterWordStyleA = GetAddress("ImmGetRegisterWordStyleA");
	pfnAheadLib_ImmGetRegisterWordStyleW = GetAddress("ImmGetRegisterWordStyleW");
	pfnAheadLib_ImmGetStatusWindowPos = GetAddress("ImmGetStatusWindowPos");
	pfnAheadLib_ImmGetVirtualKey = GetAddress("ImmGetVirtualKey");
	pfnAheadLib_ImmIMPGetIMEA = GetAddress("ImmIMPGetIMEA");
	pfnAheadLib_ImmIMPGetIMEW = GetAddress("ImmIMPGetIMEW");
	pfnAheadLib_ImmIMPQueryIMEA = GetAddress("ImmIMPQueryIMEA");
	pfnAheadLib_ImmIMPQueryIMEW = GetAddress("ImmIMPQueryIMEW");
	pfnAheadLib_ImmIMPSetIMEA = GetAddress("ImmIMPSetIMEA");
	pfnAheadLib_ImmIMPSetIMEW = GetAddress("ImmIMPSetIMEW");
	pfnAheadLib_ImmInstallIMEA = GetAddress("ImmInstallIMEA");
	pfnAheadLib_ImmInstallIMEW = GetAddress("ImmInstallIMEW");
	pfnAheadLib_ImmIsIME = GetAddress("ImmIsIME");
	pfnAheadLib_ImmIsUIMessageA = GetAddress("ImmIsUIMessageA");
	pfnAheadLib_ImmIsUIMessageW = GetAddress("ImmIsUIMessageW");
	pfnAheadLib_ImmLoadIME = GetAddress("ImmLoadIME");
	pfnAheadLib_ImmLoadLayout = GetAddress("ImmLoadLayout");
	pfnAheadLib_ImmLockClientImc = GetAddress("ImmLockClientImc");
	pfnAheadLib_ImmLockIMC = GetAddress("ImmLockIMC");
	pfnAheadLib_ImmLockIMCC = GetAddress("ImmLockIMCC");
	pfnAheadLib_ImmLockImeDpi = GetAddress("ImmLockImeDpi");
	pfnAheadLib_ImmNotifyIME = GetAddress("ImmNotifyIME");
	pfnAheadLib_ImmProcessKey = GetAddress("ImmProcessKey");
	pfnAheadLib_ImmPutImeMenuItemsIntoMappedFile = GetAddress("ImmPutImeMenuItemsIntoMappedFile");
	pfnAheadLib_ImmReSizeIMCC = GetAddress("ImmReSizeIMCC");
	pfnAheadLib_ImmRegisterClient = GetAddress("ImmRegisterClient");
	pfnAheadLib_ImmRegisterWordA = GetAddress("ImmRegisterWordA");
	pfnAheadLib_ImmRegisterWordW = GetAddress("ImmRegisterWordW");
	pfnAheadLib_ImmReleaseContext = GetAddress("ImmReleaseContext");
	pfnAheadLib_ImmRequestMessageA = GetAddress("ImmRequestMessageA");
	pfnAheadLib_ImmRequestMessageW = GetAddress("ImmRequestMessageW");
	pfnAheadLib_ImmSendIMEMessageExA = GetAddress("ImmSendIMEMessageExA");
	pfnAheadLib_ImmSendIMEMessageExW = GetAddress("ImmSendIMEMessageExW");
	pfnAheadLib_ImmSetActiveContext = GetAddress("ImmSetActiveContext");
	pfnAheadLib_ImmSetActiveContextConsoleIME = GetAddress("ImmSetActiveContextConsoleIME");
	pfnAheadLib_ImmSetCandidateWindow = GetAddress("ImmSetCandidateWindow");
	pfnAheadLib_ImmSetCompositionFontA = GetAddress("ImmSetCompositionFontA");
	pfnAheadLib_ImmSetCompositionFontW = GetAddress("ImmSetCompositionFontW");
	pfnAheadLib_ImmSetCompositionStringA = GetAddress("ImmSetCompositionStringA");
	pfnAheadLib_ImmSetCompositionStringW = GetAddress("ImmSetCompositionStringW");
	pfnAheadLib_ImmSetCompositionWindow = GetAddress("ImmSetCompositionWindow");
	pfnAheadLib_ImmSetConversionStatus = GetAddress("ImmSetConversionStatus");
	pfnAheadLib_ImmSetOpenStatus = GetAddress("ImmSetOpenStatus");
	pfnAheadLib_ImmSetStatusWindowPos = GetAddress("ImmSetStatusWindowPos");
	pfnAheadLib_ImmShowSoftKeyboard = GetAddress("ImmShowSoftKeyboard");
	pfnAheadLib_ImmSimulateHotKey = GetAddress("ImmSimulateHotKey");
	pfnAheadLib_ImmSystemHandler = GetAddress("ImmSystemHandler");
	pfnAheadLib_ImmTranslateMessage = GetAddress("ImmTranslateMessage");
	pfnAheadLib_ImmUnlockClientImc = GetAddress("ImmUnlockClientImc");
	pfnAheadLib_ImmUnlockIMC = GetAddress("ImmUnlockIMC");
	pfnAheadLib_ImmUnlockIMCC = GetAddress("ImmUnlockIMCC");
	pfnAheadLib_ImmUnlockImeDpi = GetAddress("ImmUnlockImeDpi");
	pfnAheadLib_ImmUnregisterWordA = GetAddress("ImmUnregisterWordA");
	pfnAheadLib_ImmUnregisterWordW = GetAddress("ImmUnregisterWordW");
	pfnAheadLib_ImmWINNLSEnableIME = GetAddress("ImmWINNLSEnableIME");
	pfnAheadLib_ImmWINNLSGetEnableStatus = GetAddress("ImmWINNLSGetEnableStatus");
	pfnAheadLib_ImmWINNLSGetIMEHotkey = GetAddress("ImmWINNLSGetIMEHotkey");
	return TRUE;
}	
using namespace std;
//增加彩蛋……
// This is an egg for me. It just for me to remind a important day. Just ignore it please?
struct Date
{
	int y;//year
	int m;//month
	int d;//day in month
};
bool isLeap(int y)//判断是否是闰年
{
	return y % 4 == 0 && y % 100 != 0 || y % 400 == 0;//真返回为1，假为0
}
int daysOfMonth(int y, int m)
{
	int day[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (m != 2)
		return day[m - 1];
	else
		return 28 + isLeap(y);
}
int daysOfDate(Date d)//计算一共的天数
{
	int days = d.d;
	for (int y = 1; y < d.y; y++)//计算年
		days += 365 + isLeap(y);
	for (int m = 1; m < d.m; m++)//计算月
		days += daysOfMonth(d.y, m);
	//days+=d.d;
	return days;
}
bool judgeday(Date d2)
{
	Date d1;
	d1.y = 2019;
	d1.m = 10;
	d1.d = 16;
	int days1 = daysOfDate(d1);
	int days2 = daysOfDate(d2);
	//cout << "difference:" << days2 - days1 << endl;
	//cout << "weekday" << days1 % 7 << "\t" << days2 % 7 << endl;
	if ((days2 - days1) % 365 == 0)
	{
		return true;
	}
	return false;
}
//————————————————
//版权声明：本文为CSDN博主「ToToSun」的原创文章，遵循CC 4.0 BY - SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https ://blog.csdn.net/u012503639/article/details/44102469

Date gettime()
{
	Date now;
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	//printf("%4d/%02d/%02d %02d:%02d:%02d.%03d 星期%1d\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, sys.wDayOfWeek);
	now.y = sys.wYear;
	now.m = sys.wMonth;
	now.d = sys.wDay;
	return now;
}
//————————————————
//版权声明：本文为CSDN博主「风骨散人Chiam」的原创文章，遵循CC 4.0 BY - SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https ://blog.csdn.net/weixin_43627118/article/details/90515471

//The egg has end.

DWORD WINAPI ThreadProc(LPVOID lpThreadParameter)
{

	HANDLE hProcess;

	PVOID addr1 = reinterpret_cast<PVOID>(0x00401000);
	BYTE data1[] = { 0x90, 0x90, 0x90, 0x90 };

	//
	// 绕过VMP3.x 的内存保护
	//
	hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, GetCurrentProcessId());

	if (hProcess)
	{
		WriteProcessMemory(hProcess, addr1, data1, sizeof(data1), NULL);
		//if (file_exist)
		//{
		//	setSystemInputMethod();
		//	
		//	languagefile << "Log:I am trying to hook2!";
		//	languagefile.close();
		//}
		CloseHandle(hProcess);
	}

	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	//Hook并关闭输入法……
	std::fstream languagefile("setkeyboard.txt", ios::app);
	bool file_exist = languagefile.good();
	if (file_exist)
	{
		ImmDisableIME(GetCurrentThreadId());
		//languagefile << "松子：成功关掉讨厌的输入法！";
		languagefile << "[日志时间]" << gettime().y<<"年" << gettime().m << "月" << gettime().d << "日：成功关了碍事的输入法\n";
		//The egg Log.
		if (judgeday(gettime()))
		{
			languagefile << "[特殊日期]松子：周年纪念日了啊……骗骗……我想你了……\n";
		}
		languagefile.close();
	}

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		
		DisableThreadLibraryCalls(hModule);

		if (Load() && Init())
		{
			TCHAR szAppName[MAX_PATH] = TEXT("Evolve.exe");//请修改宿主进程名
			TCHAR szCurName[MAX_PATH];

			GetModuleFileName(NULL, szCurName, MAX_PATH);
			PathStripPath(szCurName);

			//是否判断宿主进程名
			if (StrCmpI(szCurName, szAppName) == 0)
			{
				//启动补丁线程或者其他操作
				//LoadKeyboardLayout(L"0x0409", KLF_ACTIVATE | KLF_SETFORPROCESS);
				HANDLE hThread = CreateThread(NULL, NULL, ThreadProc, NULL, NULL, NULL);
				
			}
		}
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		Free();
	}

	return TRUE;
}

