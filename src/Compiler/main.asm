
.586
.model flat, stdcall
.stack 4096
option casemap:none

includelib libcmt.lib
includelib libvcruntime.lib
includelib libucrt.lib
includelib legacy_stdio_definitions.lib

ExitProcess PROTO, dwExitCode:DWORD
extern printf:NEAR

.data
text BYTE "Text", 0

.code

main PROC C

	push offset text
    call printf
    add  esp, 4

    mov eax, 0
    ret
main ENDP
end