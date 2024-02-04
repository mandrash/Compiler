%include "io.inc"
section .data
    x dd 0
    z dd "", 0
    y dd 0
    result dd "", 0
section .text
    global main
main:
    ; Assignment to z
    mov eax, "www" ; String content
    mov ebx, z ; Get the address of the variable
    mov [ebx], eax ; Store the result in the variable
    ; Assignment to z
    mov eax, "gg" ; String content
    mov ebx, z ; Get the address of the variable
    mov [ebx], eax ; Store the result in the variable
    ; Assignment to y
    mov eax, 1 ; Load constant 1 to eax
    mov ebx, y ; Get the address of the variable
    mov [ebx], eax ; Store the result in the variable
    ; Condition Operator
    mov eax, [x] ; Load variable x to eax
    mov ebx, eax
    mov eax, 5 ; Load constant 5 to eax
    cmp ebx, eax
    jne else0
    ; Loop
while0:
    mov eax, [x] ; Load variable x to eax
    mov ebx, eax
    mov eax, 2 ; Load constant 2 to eax
    cmp ebx, eax
    jle endwhile0
    ; Assignment to x
    mov edx, [x] ; Load variable x to edx
    mov eax, edx
    mov edx, 1 ; Load constant 1 to edx
    sub eax, edx ; Subtract the result from eax
    mov ebx, x ; Get the address of the variable
    mov [ebx], eax ; Store the result in the variable
    ; Assignment to z
    mov edx, [x] ; Load variable x to edx
    mov eax, edx
    mov edx, [z] ; Load variable z to edx
    add eax, edx ; Add the result to eax
    call convert_s
    mov edx, [result] ; Load variable result to eax
    add eax, edx ; Add the result to eax
    mov ebx, z ; Get the address of the variable
    mov [ebx], eax ; Store the result in the variable
    jmp while0
endwhile0:
    jmp endif0
else0:
    ; Assignment to z
    call convert_s
    mov eax, [result] ; Load variable result to eax
    mov ebx, z ; Get the address of the variable
    mov [ebx], eax ; Store the result in the variable
endif0:
    xor eax, eax
    ret

convert_s:
    ; Condition Operator
    mov eax, [y] ; Load variable y to eax
    mov ebx, eax
    mov eax, 1 ; Load constant 1 to eax
    cmp ebx, eax
    jne else1
    ; Assignment to result
    mov eax, "true" ; String content
    mov ebx, result ; Get the address of the variable
    mov [ebx], eax ; Store the result in the variable
    jmp endif1
else1:
    ; Assignment to result
    mov eax, "fals" ; String content
    mov ebx, result ; Get the address of the variable
    mov [ebx], eax ; Store the result in the variable
endif1:
    ret
