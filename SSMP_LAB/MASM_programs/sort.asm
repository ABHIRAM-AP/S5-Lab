// Sorting


ASSUME CS:CODE, DS:DATA

DATA SEGMENT
    arr db 20,30,50,10,40,60
    count db 6                  ; total elements
    msg db 10,13,"Sorted Array: $"
    newline db 10,13,'$'
DATA ENDS

DISPLAY MACRO msg
    lea dx,msg
    mov ah,09h
    int 21h
ENDM

PRINT_NUM MACRO
    LOCAL PN_CONV, PN_CONVLOOP, PN_PRINT, PN_DONE
    cmp ax,0
    jne PN_CONV
    mov dl,'0'
    mov ah,02h
    int 21h
    jmp PN_DONE

PN_CONV:
    mov bx,0
PN_CONVLOOP:
    xor dx,dx
    mov di,10
    div di
    push dx
    inc bx
    cmp ax,0
    jne PN_CONVLOOP

PN_PRINT:
    pop dx
    add dl,'0'
    mov ah,02h
    int 21h
    dec bx
    jnz PN_PRINT

PN_DONE:
ENDM

CODE SEGMENT
START:
    mov ax,DATA
    mov ds,ax

    mov cl,count
    dec cl                    
outer_loop:
    cmp cl,0
    je print_array

    mov ch,cl
    xor si,si

inner_loop:
    mov al,arr[si]
    mov bl,arr[si+1]
    cmp al,bl
    jbe no_swap

    ; swap
    mov arr[si],bl
    mov arr[si+1],al

no_swap:
    inc si
    dec ch
    jnz inner_loop

    dec cl
    jmp outer_loop

; ---------------- Print Sorted Array ----------------
print_array:
    DISPLAY msg
    mov cl,count
    xor si,si

print_loop:
    cmp cl,0
    je exit

    mov al,arr[si]
    mov ah,0
    mov ax,0
    mov al,arr[si]   ; move number into AX
    ; print number
    PRINT_NUM

    ; print space
    mov dl,' '
    mov ah,02h
    int 21h

    inc si
    dec cl
    jmp print_loop

exit:
    DISPLAY newline
    mov ah,4Ch
    int 21h

CODE ENDS
END START
