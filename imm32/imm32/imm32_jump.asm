
;
; created by AheadLib
; github:https://github.com/strivexjun/AheadLib-x86-x64
;
; 把 .asm 文件添加到工程一次
; 右键单击文件-属性-常规-
; 项类型:自定义生成工具
; 从生成中排除:否

; 然后复制下面命令填入
; 命令行: ml64 /Fo $(IntDir)%(fileName).obj /c /Cp %(fileName).asm
; 输出: $(IntDir)%(fileName).obj;%(Outputs)
; 链接对象: 是
;


.DATA
EXTERN pfnAheadLib_CtfImmAppCompatEnableIMEonProtectedCode:dq;
EXTERN pfnAheadLib_CtfImmCoUninitialize:dq;
EXTERN pfnAheadLib_CtfImmDispatchDefImeMessage:dq;
EXTERN pfnAheadLib_CtfImmEnterCoInitCountSkipMode:dq;
EXTERN pfnAheadLib_CtfImmGenerateMessage:dq;
EXTERN pfnAheadLib_CtfImmGetCompatibleKeyboardLayout:dq;
EXTERN pfnAheadLib_CtfImmGetGlobalIMEStatus:dq;
EXTERN pfnAheadLib_CtfImmGetGuidAtom:dq;
EXTERN pfnAheadLib_CtfImmGetIMEFileName:dq;
EXTERN pfnAheadLib_CtfImmGetTMAEFlags:dq;
EXTERN pfnAheadLib_CtfImmHideToolbarWnd:dq;
EXTERN pfnAheadLib_CtfImmIsCiceroEnabled:dq;
EXTERN pfnAheadLib_CtfImmIsCiceroStartedInThread:dq;
EXTERN pfnAheadLib_CtfImmIsComStartedInThread:dq;
EXTERN pfnAheadLib_CtfImmIsGuidMapEnable:dq;
EXTERN pfnAheadLib_CtfImmIsTextFrameServiceDisabled:dq;
EXTERN pfnAheadLib_CtfImmLastEnabledWndDestroy:dq;
EXTERN pfnAheadLib_CtfImmLeaveCoInitCountSkipMode:dq;
EXTERN pfnAheadLib_CtfImmNotify:dq;
EXTERN pfnAheadLib_CtfImmRestoreToolbarWnd:dq;
EXTERN pfnAheadLib_CtfImmSetAppCompatFlags:dq;
EXTERN pfnAheadLib_CtfImmSetCiceroStartInThread:dq;
EXTERN pfnAheadLib_CtfImmSetDefaultRemoteKeyboardLayout:dq;
EXTERN pfnAheadLib_CtfImmTIMActivate:dq;
EXTERN pfnAheadLib_GetKeyboardLayoutCP:dq;
EXTERN pfnAheadLib_ImmActivateLayout:dq;
EXTERN pfnAheadLib_ImmAssociateContext:dq;
EXTERN pfnAheadLib_ImmAssociateContextEx:dq;
EXTERN pfnAheadLib_ImmCallImeConsoleIME:dq;
EXTERN pfnAheadLib_ImmConfigureIMEA:dq;
EXTERN pfnAheadLib_ImmConfigureIMEW:dq;
EXTERN pfnAheadLib_ImmCreateContext:dq;
EXTERN pfnAheadLib_ImmCreateIMCC:dq;
EXTERN pfnAheadLib_ImmCreateSoftKeyboard:dq;
EXTERN pfnAheadLib_ImmDestroyContext:dq;
EXTERN pfnAheadLib_ImmDestroyIMCC:dq;
EXTERN pfnAheadLib_ImmDestroySoftKeyboard:dq;
EXTERN pfnAheadLib_ImmDisableIME:dq;
EXTERN pfnAheadLib_ImmDisableIme:dq;
EXTERN pfnAheadLib_ImmDisableLegacyIME:dq;
EXTERN pfnAheadLib_ImmDisableTextFrameService:dq;
EXTERN pfnAheadLib_ImmEnumInputContext:dq;
EXTERN pfnAheadLib_ImmEnumRegisterWordA:dq;
EXTERN pfnAheadLib_ImmEnumRegisterWordW:dq;
EXTERN pfnAheadLib_ImmEscapeA:dq;
EXTERN pfnAheadLib_ImmEscapeW:dq;
EXTERN pfnAheadLib_ImmFreeLayout:dq;
EXTERN pfnAheadLib_ImmGenerateMessage:dq;
EXTERN pfnAheadLib_ImmGetAppCompatFlags:dq;
EXTERN pfnAheadLib_ImmGetCandidateListA:dq;
EXTERN pfnAheadLib_ImmGetCandidateListCountA:dq;
EXTERN pfnAheadLib_ImmGetCandidateListCountW:dq;
EXTERN pfnAheadLib_ImmGetCandidateListW:dq;
EXTERN pfnAheadLib_ImmGetCandidateWindow:dq;
EXTERN pfnAheadLib_ImmGetCompositionFontA:dq;
EXTERN pfnAheadLib_ImmGetCompositionFontW:dq;
EXTERN pfnAheadLib_ImmGetCompositionStringA:dq;
EXTERN pfnAheadLib_ImmGetCompositionStringW:dq;
EXTERN pfnAheadLib_ImmGetCompositionWindow:dq;
EXTERN pfnAheadLib_ImmGetContext:dq;
EXTERN pfnAheadLib_ImmGetConversionListA:dq;
EXTERN pfnAheadLib_ImmGetConversionListW:dq;
EXTERN pfnAheadLib_ImmGetConversionStatus:dq;
EXTERN pfnAheadLib_ImmGetDefaultIMEWnd:dq;
EXTERN pfnAheadLib_ImmGetDescriptionA:dq;
EXTERN pfnAheadLib_ImmGetDescriptionW:dq;
EXTERN pfnAheadLib_ImmGetGuideLineA:dq;
EXTERN pfnAheadLib_ImmGetGuideLineW:dq;
EXTERN pfnAheadLib_ImmGetHotKey:dq;
EXTERN pfnAheadLib_ImmGetIMCCLockCount:dq;
EXTERN pfnAheadLib_ImmGetIMCCSize:dq;
EXTERN pfnAheadLib_ImmGetIMCLockCount:dq;
EXTERN pfnAheadLib_ImmGetIMEFileNameA:dq;
EXTERN pfnAheadLib_ImmGetIMEFileNameW:dq;
EXTERN pfnAheadLib_ImmGetImeInfoEx:dq;
EXTERN pfnAheadLib_ImmGetImeMenuItemsA:dq;
EXTERN pfnAheadLib_ImmGetImeMenuItemsW:dq;
EXTERN pfnAheadLib_ImmGetOpenStatus:dq;
EXTERN pfnAheadLib_ImmGetProperty:dq;
EXTERN pfnAheadLib_ImmGetRegisterWordStyleA:dq;
EXTERN pfnAheadLib_ImmGetRegisterWordStyleW:dq;
EXTERN pfnAheadLib_ImmGetStatusWindowPos:dq;
EXTERN pfnAheadLib_ImmGetVirtualKey:dq;
EXTERN pfnAheadLib_ImmIMPGetIMEA:dq;
EXTERN pfnAheadLib_ImmIMPGetIMEW:dq;
EXTERN pfnAheadLib_ImmIMPQueryIMEA:dq;
EXTERN pfnAheadLib_ImmIMPQueryIMEW:dq;
EXTERN pfnAheadLib_ImmIMPSetIMEA:dq;
EXTERN pfnAheadLib_ImmIMPSetIMEW:dq;
EXTERN pfnAheadLib_ImmInstallIMEA:dq;
EXTERN pfnAheadLib_ImmInstallIMEW:dq;
EXTERN pfnAheadLib_ImmIsIME:dq;
EXTERN pfnAheadLib_ImmIsUIMessageA:dq;
EXTERN pfnAheadLib_ImmIsUIMessageW:dq;
EXTERN pfnAheadLib_ImmLoadIME:dq;
EXTERN pfnAheadLib_ImmLoadLayout:dq;
EXTERN pfnAheadLib_ImmLockClientImc:dq;
EXTERN pfnAheadLib_ImmLockIMC:dq;
EXTERN pfnAheadLib_ImmLockIMCC:dq;
EXTERN pfnAheadLib_ImmLockImeDpi:dq;
EXTERN pfnAheadLib_ImmNotifyIME:dq;
EXTERN pfnAheadLib_ImmProcessKey:dq;
EXTERN pfnAheadLib_ImmPutImeMenuItemsIntoMappedFile:dq;
EXTERN pfnAheadLib_ImmReSizeIMCC:dq;
EXTERN pfnAheadLib_ImmRegisterClient:dq;
EXTERN pfnAheadLib_ImmRegisterWordA:dq;
EXTERN pfnAheadLib_ImmRegisterWordW:dq;
EXTERN pfnAheadLib_ImmReleaseContext:dq;
EXTERN pfnAheadLib_ImmRequestMessageA:dq;
EXTERN pfnAheadLib_ImmRequestMessageW:dq;
EXTERN pfnAheadLib_ImmSendIMEMessageExA:dq;
EXTERN pfnAheadLib_ImmSendIMEMessageExW:dq;
EXTERN pfnAheadLib_ImmSetActiveContext:dq;
EXTERN pfnAheadLib_ImmSetActiveContextConsoleIME:dq;
EXTERN pfnAheadLib_ImmSetCandidateWindow:dq;
EXTERN pfnAheadLib_ImmSetCompositionFontA:dq;
EXTERN pfnAheadLib_ImmSetCompositionFontW:dq;
EXTERN pfnAheadLib_ImmSetCompositionStringA:dq;
EXTERN pfnAheadLib_ImmSetCompositionStringW:dq;
EXTERN pfnAheadLib_ImmSetCompositionWindow:dq;
EXTERN pfnAheadLib_ImmSetConversionStatus:dq;
EXTERN pfnAheadLib_ImmSetOpenStatus:dq;
EXTERN pfnAheadLib_ImmSetStatusWindowPos:dq;
EXTERN pfnAheadLib_ImmShowSoftKeyboard:dq;
EXTERN pfnAheadLib_ImmSimulateHotKey:dq;
EXTERN pfnAheadLib_ImmSystemHandler:dq;
EXTERN pfnAheadLib_ImmTranslateMessage:dq;
EXTERN pfnAheadLib_ImmUnlockClientImc:dq;
EXTERN pfnAheadLib_ImmUnlockIMC:dq;
EXTERN pfnAheadLib_ImmUnlockIMCC:dq;
EXTERN pfnAheadLib_ImmUnlockImeDpi:dq;
EXTERN pfnAheadLib_ImmUnregisterWordA:dq;
EXTERN pfnAheadLib_ImmUnregisterWordW:dq;
EXTERN pfnAheadLib_ImmWINNLSEnableIME:dq;
EXTERN pfnAheadLib_ImmWINNLSGetEnableStatus:dq;
EXTERN pfnAheadLib_ImmWINNLSGetIMEHotkey:dq;

.CODE
AheadLib_CtfImmAppCompatEnableIMEonProtectedCode PROC
	jmp pfnAheadLib_CtfImmAppCompatEnableIMEonProtectedCode
AheadLib_CtfImmAppCompatEnableIMEonProtectedCode ENDP

AheadLib_CtfImmCoUninitialize PROC
	jmp pfnAheadLib_CtfImmCoUninitialize
AheadLib_CtfImmCoUninitialize ENDP

AheadLib_CtfImmDispatchDefImeMessage PROC
	jmp pfnAheadLib_CtfImmDispatchDefImeMessage
AheadLib_CtfImmDispatchDefImeMessage ENDP

AheadLib_CtfImmEnterCoInitCountSkipMode PROC
	jmp pfnAheadLib_CtfImmEnterCoInitCountSkipMode
AheadLib_CtfImmEnterCoInitCountSkipMode ENDP

AheadLib_CtfImmGenerateMessage PROC
	jmp pfnAheadLib_CtfImmGenerateMessage
AheadLib_CtfImmGenerateMessage ENDP

AheadLib_CtfImmGetCompatibleKeyboardLayout PROC
	jmp pfnAheadLib_CtfImmGetCompatibleKeyboardLayout
AheadLib_CtfImmGetCompatibleKeyboardLayout ENDP

AheadLib_CtfImmGetGlobalIMEStatus PROC
	jmp pfnAheadLib_CtfImmGetGlobalIMEStatus
AheadLib_CtfImmGetGlobalIMEStatus ENDP

AheadLib_CtfImmGetGuidAtom PROC
	jmp pfnAheadLib_CtfImmGetGuidAtom
AheadLib_CtfImmGetGuidAtom ENDP

AheadLib_CtfImmGetIMEFileName PROC
	jmp pfnAheadLib_CtfImmGetIMEFileName
AheadLib_CtfImmGetIMEFileName ENDP

AheadLib_CtfImmGetTMAEFlags PROC
	jmp pfnAheadLib_CtfImmGetTMAEFlags
AheadLib_CtfImmGetTMAEFlags ENDP

AheadLib_CtfImmHideToolbarWnd PROC
	jmp pfnAheadLib_CtfImmHideToolbarWnd
AheadLib_CtfImmHideToolbarWnd ENDP

AheadLib_CtfImmIsCiceroEnabled PROC
	jmp pfnAheadLib_CtfImmIsCiceroEnabled
AheadLib_CtfImmIsCiceroEnabled ENDP

AheadLib_CtfImmIsCiceroStartedInThread PROC
	jmp pfnAheadLib_CtfImmIsCiceroStartedInThread
AheadLib_CtfImmIsCiceroStartedInThread ENDP

AheadLib_CtfImmIsComStartedInThread PROC
	jmp pfnAheadLib_CtfImmIsComStartedInThread
AheadLib_CtfImmIsComStartedInThread ENDP

AheadLib_CtfImmIsGuidMapEnable PROC
	jmp pfnAheadLib_CtfImmIsGuidMapEnable
AheadLib_CtfImmIsGuidMapEnable ENDP

AheadLib_CtfImmIsTextFrameServiceDisabled PROC
	jmp pfnAheadLib_CtfImmIsTextFrameServiceDisabled
AheadLib_CtfImmIsTextFrameServiceDisabled ENDP

AheadLib_CtfImmLastEnabledWndDestroy PROC
	jmp pfnAheadLib_CtfImmLastEnabledWndDestroy
AheadLib_CtfImmLastEnabledWndDestroy ENDP

AheadLib_CtfImmLeaveCoInitCountSkipMode PROC
	jmp pfnAheadLib_CtfImmLeaveCoInitCountSkipMode
AheadLib_CtfImmLeaveCoInitCountSkipMode ENDP

AheadLib_CtfImmNotify PROC
	jmp pfnAheadLib_CtfImmNotify
AheadLib_CtfImmNotify ENDP

AheadLib_CtfImmRestoreToolbarWnd PROC
	jmp pfnAheadLib_CtfImmRestoreToolbarWnd
AheadLib_CtfImmRestoreToolbarWnd ENDP

AheadLib_CtfImmSetAppCompatFlags PROC
	jmp pfnAheadLib_CtfImmSetAppCompatFlags
AheadLib_CtfImmSetAppCompatFlags ENDP

AheadLib_CtfImmSetCiceroStartInThread PROC
	jmp pfnAheadLib_CtfImmSetCiceroStartInThread
AheadLib_CtfImmSetCiceroStartInThread ENDP

AheadLib_CtfImmSetDefaultRemoteKeyboardLayout PROC
	jmp pfnAheadLib_CtfImmSetDefaultRemoteKeyboardLayout
AheadLib_CtfImmSetDefaultRemoteKeyboardLayout ENDP

AheadLib_CtfImmTIMActivate PROC
	jmp pfnAheadLib_CtfImmTIMActivate
AheadLib_CtfImmTIMActivate ENDP

AheadLib_GetKeyboardLayoutCP PROC
	jmp pfnAheadLib_GetKeyboardLayoutCP
AheadLib_GetKeyboardLayoutCP ENDP

AheadLib_ImmActivateLayout PROC
	jmp pfnAheadLib_ImmActivateLayout
AheadLib_ImmActivateLayout ENDP

AheadLib_ImmAssociateContext PROC
	jmp pfnAheadLib_ImmAssociateContext
AheadLib_ImmAssociateContext ENDP

AheadLib_ImmAssociateContextEx PROC
	jmp pfnAheadLib_ImmAssociateContextEx
AheadLib_ImmAssociateContextEx ENDP

AheadLib_ImmCallImeConsoleIME PROC
	jmp pfnAheadLib_ImmCallImeConsoleIME
AheadLib_ImmCallImeConsoleIME ENDP

AheadLib_ImmConfigureIMEA PROC
	jmp pfnAheadLib_ImmConfigureIMEA
AheadLib_ImmConfigureIMEA ENDP

AheadLib_ImmConfigureIMEW PROC
	jmp pfnAheadLib_ImmConfigureIMEW
AheadLib_ImmConfigureIMEW ENDP

AheadLib_ImmCreateContext PROC
	jmp pfnAheadLib_ImmCreateContext
AheadLib_ImmCreateContext ENDP

AheadLib_ImmCreateIMCC PROC
	jmp pfnAheadLib_ImmCreateIMCC
AheadLib_ImmCreateIMCC ENDP

AheadLib_ImmCreateSoftKeyboard PROC
	jmp pfnAheadLib_ImmCreateSoftKeyboard
AheadLib_ImmCreateSoftKeyboard ENDP

AheadLib_ImmDestroyContext PROC
	jmp pfnAheadLib_ImmDestroyContext
AheadLib_ImmDestroyContext ENDP

AheadLib_ImmDestroyIMCC PROC
	jmp pfnAheadLib_ImmDestroyIMCC
AheadLib_ImmDestroyIMCC ENDP

AheadLib_ImmDestroySoftKeyboard PROC
	jmp pfnAheadLib_ImmDestroySoftKeyboard
AheadLib_ImmDestroySoftKeyboard ENDP

AheadLib_ImmDisableIME PROC
	jmp pfnAheadLib_ImmDisableIME
AheadLib_ImmDisableIME ENDP

AheadLib_ImmDisableIme PROC
	jmp pfnAheadLib_ImmDisableIme
AheadLib_ImmDisableIme ENDP

AheadLib_ImmDisableLegacyIME PROC
	jmp pfnAheadLib_ImmDisableLegacyIME
AheadLib_ImmDisableLegacyIME ENDP

AheadLib_ImmDisableTextFrameService PROC
	jmp pfnAheadLib_ImmDisableTextFrameService
AheadLib_ImmDisableTextFrameService ENDP

AheadLib_ImmEnumInputContext PROC
	jmp pfnAheadLib_ImmEnumInputContext
AheadLib_ImmEnumInputContext ENDP

AheadLib_ImmEnumRegisterWordA PROC
	jmp pfnAheadLib_ImmEnumRegisterWordA
AheadLib_ImmEnumRegisterWordA ENDP

AheadLib_ImmEnumRegisterWordW PROC
	jmp pfnAheadLib_ImmEnumRegisterWordW
AheadLib_ImmEnumRegisterWordW ENDP

AheadLib_ImmEscapeA PROC
	jmp pfnAheadLib_ImmEscapeA
AheadLib_ImmEscapeA ENDP

AheadLib_ImmEscapeW PROC
	jmp pfnAheadLib_ImmEscapeW
AheadLib_ImmEscapeW ENDP

AheadLib_ImmFreeLayout PROC
	jmp pfnAheadLib_ImmFreeLayout
AheadLib_ImmFreeLayout ENDP

AheadLib_ImmGenerateMessage PROC
	jmp pfnAheadLib_ImmGenerateMessage
AheadLib_ImmGenerateMessage ENDP

AheadLib_ImmGetAppCompatFlags PROC
	jmp pfnAheadLib_ImmGetAppCompatFlags
AheadLib_ImmGetAppCompatFlags ENDP

AheadLib_ImmGetCandidateListA PROC
	jmp pfnAheadLib_ImmGetCandidateListA
AheadLib_ImmGetCandidateListA ENDP

AheadLib_ImmGetCandidateListCountA PROC
	jmp pfnAheadLib_ImmGetCandidateListCountA
AheadLib_ImmGetCandidateListCountA ENDP

AheadLib_ImmGetCandidateListCountW PROC
	jmp pfnAheadLib_ImmGetCandidateListCountW
AheadLib_ImmGetCandidateListCountW ENDP

AheadLib_ImmGetCandidateListW PROC
	jmp pfnAheadLib_ImmGetCandidateListW
AheadLib_ImmGetCandidateListW ENDP

AheadLib_ImmGetCandidateWindow PROC
	jmp pfnAheadLib_ImmGetCandidateWindow
AheadLib_ImmGetCandidateWindow ENDP

AheadLib_ImmGetCompositionFontA PROC
	jmp pfnAheadLib_ImmGetCompositionFontA
AheadLib_ImmGetCompositionFontA ENDP

AheadLib_ImmGetCompositionFontW PROC
	jmp pfnAheadLib_ImmGetCompositionFontW
AheadLib_ImmGetCompositionFontW ENDP

AheadLib_ImmGetCompositionStringA PROC
	jmp pfnAheadLib_ImmGetCompositionStringA
AheadLib_ImmGetCompositionStringA ENDP

AheadLib_ImmGetCompositionStringW PROC
	jmp pfnAheadLib_ImmGetCompositionStringW
AheadLib_ImmGetCompositionStringW ENDP

AheadLib_ImmGetCompositionWindow PROC
	jmp pfnAheadLib_ImmGetCompositionWindow
AheadLib_ImmGetCompositionWindow ENDP

AheadLib_ImmGetContext PROC
	jmp pfnAheadLib_ImmGetContext
AheadLib_ImmGetContext ENDP

AheadLib_ImmGetConversionListA PROC
	jmp pfnAheadLib_ImmGetConversionListA
AheadLib_ImmGetConversionListA ENDP

AheadLib_ImmGetConversionListW PROC
	jmp pfnAheadLib_ImmGetConversionListW
AheadLib_ImmGetConversionListW ENDP

AheadLib_ImmGetConversionStatus PROC
	jmp pfnAheadLib_ImmGetConversionStatus
AheadLib_ImmGetConversionStatus ENDP

AheadLib_ImmGetDefaultIMEWnd PROC
	jmp pfnAheadLib_ImmGetDefaultIMEWnd
AheadLib_ImmGetDefaultIMEWnd ENDP

AheadLib_ImmGetDescriptionA PROC
	jmp pfnAheadLib_ImmGetDescriptionA
AheadLib_ImmGetDescriptionA ENDP

AheadLib_ImmGetDescriptionW PROC
	jmp pfnAheadLib_ImmGetDescriptionW
AheadLib_ImmGetDescriptionW ENDP

AheadLib_ImmGetGuideLineA PROC
	jmp pfnAheadLib_ImmGetGuideLineA
AheadLib_ImmGetGuideLineA ENDP

AheadLib_ImmGetGuideLineW PROC
	jmp pfnAheadLib_ImmGetGuideLineW
AheadLib_ImmGetGuideLineW ENDP

AheadLib_ImmGetHotKey PROC
	jmp pfnAheadLib_ImmGetHotKey
AheadLib_ImmGetHotKey ENDP

AheadLib_ImmGetIMCCLockCount PROC
	jmp pfnAheadLib_ImmGetIMCCLockCount
AheadLib_ImmGetIMCCLockCount ENDP

AheadLib_ImmGetIMCCSize PROC
	jmp pfnAheadLib_ImmGetIMCCSize
AheadLib_ImmGetIMCCSize ENDP

AheadLib_ImmGetIMCLockCount PROC
	jmp pfnAheadLib_ImmGetIMCLockCount
AheadLib_ImmGetIMCLockCount ENDP

AheadLib_ImmGetIMEFileNameA PROC
	jmp pfnAheadLib_ImmGetIMEFileNameA
AheadLib_ImmGetIMEFileNameA ENDP

AheadLib_ImmGetIMEFileNameW PROC
	jmp pfnAheadLib_ImmGetIMEFileNameW
AheadLib_ImmGetIMEFileNameW ENDP

AheadLib_ImmGetImeInfoEx PROC
	jmp pfnAheadLib_ImmGetImeInfoEx
AheadLib_ImmGetImeInfoEx ENDP

AheadLib_ImmGetImeMenuItemsA PROC
	jmp pfnAheadLib_ImmGetImeMenuItemsA
AheadLib_ImmGetImeMenuItemsA ENDP

AheadLib_ImmGetImeMenuItemsW PROC
	jmp pfnAheadLib_ImmGetImeMenuItemsW
AheadLib_ImmGetImeMenuItemsW ENDP

AheadLib_ImmGetOpenStatus PROC
	jmp pfnAheadLib_ImmGetOpenStatus
AheadLib_ImmGetOpenStatus ENDP

AheadLib_ImmGetProperty PROC
	jmp pfnAheadLib_ImmGetProperty
AheadLib_ImmGetProperty ENDP

AheadLib_ImmGetRegisterWordStyleA PROC
	jmp pfnAheadLib_ImmGetRegisterWordStyleA
AheadLib_ImmGetRegisterWordStyleA ENDP

AheadLib_ImmGetRegisterWordStyleW PROC
	jmp pfnAheadLib_ImmGetRegisterWordStyleW
AheadLib_ImmGetRegisterWordStyleW ENDP

AheadLib_ImmGetStatusWindowPos PROC
	jmp pfnAheadLib_ImmGetStatusWindowPos
AheadLib_ImmGetStatusWindowPos ENDP

AheadLib_ImmGetVirtualKey PROC
	jmp pfnAheadLib_ImmGetVirtualKey
AheadLib_ImmGetVirtualKey ENDP

AheadLib_ImmIMPGetIMEA PROC
	jmp pfnAheadLib_ImmIMPGetIMEA
AheadLib_ImmIMPGetIMEA ENDP

AheadLib_ImmIMPGetIMEW PROC
	jmp pfnAheadLib_ImmIMPGetIMEW
AheadLib_ImmIMPGetIMEW ENDP

AheadLib_ImmIMPQueryIMEA PROC
	jmp pfnAheadLib_ImmIMPQueryIMEA
AheadLib_ImmIMPQueryIMEA ENDP

AheadLib_ImmIMPQueryIMEW PROC
	jmp pfnAheadLib_ImmIMPQueryIMEW
AheadLib_ImmIMPQueryIMEW ENDP

AheadLib_ImmIMPSetIMEA PROC
	jmp pfnAheadLib_ImmIMPSetIMEA
AheadLib_ImmIMPSetIMEA ENDP

AheadLib_ImmIMPSetIMEW PROC
	jmp pfnAheadLib_ImmIMPSetIMEW
AheadLib_ImmIMPSetIMEW ENDP

AheadLib_ImmInstallIMEA PROC
	jmp pfnAheadLib_ImmInstallIMEA
AheadLib_ImmInstallIMEA ENDP

AheadLib_ImmInstallIMEW PROC
	jmp pfnAheadLib_ImmInstallIMEW
AheadLib_ImmInstallIMEW ENDP

AheadLib_ImmIsIME PROC
	jmp pfnAheadLib_ImmIsIME
AheadLib_ImmIsIME ENDP

AheadLib_ImmIsUIMessageA PROC
	jmp pfnAheadLib_ImmIsUIMessageA
AheadLib_ImmIsUIMessageA ENDP

AheadLib_ImmIsUIMessageW PROC
	jmp pfnAheadLib_ImmIsUIMessageW
AheadLib_ImmIsUIMessageW ENDP

AheadLib_ImmLoadIME PROC
	jmp pfnAheadLib_ImmLoadIME
AheadLib_ImmLoadIME ENDP

AheadLib_ImmLoadLayout PROC
	jmp pfnAheadLib_ImmLoadLayout
AheadLib_ImmLoadLayout ENDP

AheadLib_ImmLockClientImc PROC
	jmp pfnAheadLib_ImmLockClientImc
AheadLib_ImmLockClientImc ENDP

AheadLib_ImmLockIMC PROC
	jmp pfnAheadLib_ImmLockIMC
AheadLib_ImmLockIMC ENDP

AheadLib_ImmLockIMCC PROC
	jmp pfnAheadLib_ImmLockIMCC
AheadLib_ImmLockIMCC ENDP

AheadLib_ImmLockImeDpi PROC
	jmp pfnAheadLib_ImmLockImeDpi
AheadLib_ImmLockImeDpi ENDP

AheadLib_ImmNotifyIME PROC
	jmp pfnAheadLib_ImmNotifyIME
AheadLib_ImmNotifyIME ENDP

AheadLib_ImmProcessKey PROC
	jmp pfnAheadLib_ImmProcessKey
AheadLib_ImmProcessKey ENDP

AheadLib_ImmPutImeMenuItemsIntoMappedFile PROC
	jmp pfnAheadLib_ImmPutImeMenuItemsIntoMappedFile
AheadLib_ImmPutImeMenuItemsIntoMappedFile ENDP

AheadLib_ImmReSizeIMCC PROC
	jmp pfnAheadLib_ImmReSizeIMCC
AheadLib_ImmReSizeIMCC ENDP

AheadLib_ImmRegisterClient PROC
	jmp pfnAheadLib_ImmRegisterClient
AheadLib_ImmRegisterClient ENDP

AheadLib_ImmRegisterWordA PROC
	jmp pfnAheadLib_ImmRegisterWordA
AheadLib_ImmRegisterWordA ENDP

AheadLib_ImmRegisterWordW PROC
	jmp pfnAheadLib_ImmRegisterWordW
AheadLib_ImmRegisterWordW ENDP

AheadLib_ImmReleaseContext PROC
	jmp pfnAheadLib_ImmReleaseContext
AheadLib_ImmReleaseContext ENDP

AheadLib_ImmRequestMessageA PROC
	jmp pfnAheadLib_ImmRequestMessageA
AheadLib_ImmRequestMessageA ENDP

AheadLib_ImmRequestMessageW PROC
	jmp pfnAheadLib_ImmRequestMessageW
AheadLib_ImmRequestMessageW ENDP

AheadLib_ImmSendIMEMessageExA PROC
	jmp pfnAheadLib_ImmSendIMEMessageExA
AheadLib_ImmSendIMEMessageExA ENDP

AheadLib_ImmSendIMEMessageExW PROC
	jmp pfnAheadLib_ImmSendIMEMessageExW
AheadLib_ImmSendIMEMessageExW ENDP

AheadLib_ImmSetActiveContext PROC
	jmp pfnAheadLib_ImmSetActiveContext
AheadLib_ImmSetActiveContext ENDP

AheadLib_ImmSetActiveContextConsoleIME PROC
	jmp pfnAheadLib_ImmSetActiveContextConsoleIME
AheadLib_ImmSetActiveContextConsoleIME ENDP

AheadLib_ImmSetCandidateWindow PROC
	jmp pfnAheadLib_ImmSetCandidateWindow
AheadLib_ImmSetCandidateWindow ENDP

AheadLib_ImmSetCompositionFontA PROC
	jmp pfnAheadLib_ImmSetCompositionFontA
AheadLib_ImmSetCompositionFontA ENDP

AheadLib_ImmSetCompositionFontW PROC
	jmp pfnAheadLib_ImmSetCompositionFontW
AheadLib_ImmSetCompositionFontW ENDP

AheadLib_ImmSetCompositionStringA PROC
	jmp pfnAheadLib_ImmSetCompositionStringA
AheadLib_ImmSetCompositionStringA ENDP

AheadLib_ImmSetCompositionStringW PROC
	jmp pfnAheadLib_ImmSetCompositionStringW
AheadLib_ImmSetCompositionStringW ENDP

AheadLib_ImmSetCompositionWindow PROC
	jmp pfnAheadLib_ImmSetCompositionWindow
AheadLib_ImmSetCompositionWindow ENDP

AheadLib_ImmSetConversionStatus PROC
	jmp pfnAheadLib_ImmSetConversionStatus
AheadLib_ImmSetConversionStatus ENDP

AheadLib_ImmSetOpenStatus PROC
	jmp pfnAheadLib_ImmSetOpenStatus
AheadLib_ImmSetOpenStatus ENDP

AheadLib_ImmSetStatusWindowPos PROC
	jmp pfnAheadLib_ImmSetStatusWindowPos
AheadLib_ImmSetStatusWindowPos ENDP

AheadLib_ImmShowSoftKeyboard PROC
	jmp pfnAheadLib_ImmShowSoftKeyboard
AheadLib_ImmShowSoftKeyboard ENDP

AheadLib_ImmSimulateHotKey PROC
	jmp pfnAheadLib_ImmSimulateHotKey
AheadLib_ImmSimulateHotKey ENDP

AheadLib_ImmSystemHandler PROC
	jmp pfnAheadLib_ImmSystemHandler
AheadLib_ImmSystemHandler ENDP

AheadLib_ImmTranslateMessage PROC
	jmp pfnAheadLib_ImmTranslateMessage
AheadLib_ImmTranslateMessage ENDP

AheadLib_ImmUnlockClientImc PROC
	jmp pfnAheadLib_ImmUnlockClientImc
AheadLib_ImmUnlockClientImc ENDP

AheadLib_ImmUnlockIMC PROC
	jmp pfnAheadLib_ImmUnlockIMC
AheadLib_ImmUnlockIMC ENDP

AheadLib_ImmUnlockIMCC PROC
	jmp pfnAheadLib_ImmUnlockIMCC
AheadLib_ImmUnlockIMCC ENDP

AheadLib_ImmUnlockImeDpi PROC
	jmp pfnAheadLib_ImmUnlockImeDpi
AheadLib_ImmUnlockImeDpi ENDP

AheadLib_ImmUnregisterWordA PROC
	jmp pfnAheadLib_ImmUnregisterWordA
AheadLib_ImmUnregisterWordA ENDP

AheadLib_ImmUnregisterWordW PROC
	jmp pfnAheadLib_ImmUnregisterWordW
AheadLib_ImmUnregisterWordW ENDP

AheadLib_ImmWINNLSEnableIME PROC
	jmp pfnAheadLib_ImmWINNLSEnableIME
AheadLib_ImmWINNLSEnableIME ENDP

AheadLib_ImmWINNLSGetEnableStatus PROC
	jmp pfnAheadLib_ImmWINNLSGetEnableStatus
AheadLib_ImmWINNLSGetEnableStatus ENDP

AheadLib_ImmWINNLSGetIMEHotkey PROC
	jmp pfnAheadLib_ImmWINNLSGetIMEHotkey
AheadLib_ImmWINNLSGetIMEHotkey ENDP


END
