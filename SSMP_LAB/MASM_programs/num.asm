; To get an user input number 

ASSUME CS:CODE, DS:DATA

DATA SEGMENT
    m1 db 10,13,"Enter a number: $"
    m2 db 10,13,"You have entered: $"
    num1 dw 0
    nl db 10,13,'$'
DATA ENDS

CODE SEGMENT

PRINT_STR MACRO msg
    lea dx, msg
    mov ah, 09h
    int 21h
ENDM

READ_NUM MACRO var
    mov var,0
READ_&var:
    mov ah,01h
    int 21h
    cmp al,13
    je DONE_&var
    sub al,'0'      ; convert ASCII to digit
    mov ah,0        ; make AX 16-bit
    mov bx,ax       ; BX = digit
    mov ax,var      ; AX = current number
    mov cx,10
    mul cx          ; AX = AX * 10
    add ax,bx       ; AX = AX + digit
    mov var,ax
    jmp READ_&var
DONE_&var:
ENDM


PRINT_NUM MACRO
    cmp ax,0
    jne PRINT_CONVERT
    ; print 0
    mov dl,'0'
    mov ah,02h
    int 21h
    jmp PRINT_DONE

PRINT_CONVERT:
    mov cx,0
CONVERT_LOOP:
    xor dx,dx
    mov bx,10
    div bx
    push dx
    inc cx
    cmp ax,0
    jne CONVERT_LOOP

PRINT_LOOP:
    pop dx
    add dl,'0'
    mov ah,02h
    int 21h
    loop PRINT_LOOP

PRINT_DONE:
ENDM

START:
    mov ax, DATA
    mov ds, ax

    PRINT_STR m1
    READ_NUM num1

    PRINT_STR m2
    mov ax,num1
    PRINT_NUM

    PRINT_STR nl

    mov ah,4Ch
    int 21h

CODE ENDS
END START
