
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
EXTERN pfnAheadLib_WinVerifyTrust:dq;

.CODE
AheadLib_WinVerifyTrust PROC
	jmp pfnAheadLib_WinVerifyTrust
AheadLib_WinVerifyTrust ENDP

END
