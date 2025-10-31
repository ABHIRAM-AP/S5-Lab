;To read an array from user and print


ASSUME CS:CODE, DS:DATA

DATA SEGMENT
    m1 db 10,13,"Enter number: $"
    m2 db 10,13,"Array Elements are: $"
    num1 dw 0
    nl db 10,13,'$'
    arr dw,5 ,5 dup(?)       
DATA ENDS

display macro msg
	lea dx,msg
	mov ah,09h
	int 21h
endm

print_char macro
	mov ah,02h
	int 21h
endm

read_num macro var
	local read_var,done_var
	mov var,0
read_var:
	mov ah,01h
	int 21h

	cmp al,13
	je done_var

	sub al,'0'
	mov ah,0

	mov bx,ax
	mov dx,10

	mov ax,var
	mul dx
	add ax,bx
	mov var,ax
	jmp read_var
done_var:
endm


print_num macro 
	cmp ax,0
	jne print_convert
	mov dl,'0'
	print_char
	jmp print_done

print_convert:
	mov bx,0
convert_loop:
	xor dx,dx
	mov di,10
	div di
	push dx
	inc bx
	cmp ax,0
	jne convert_loop

print_loop:
	pop dx
	add dl,'0'
	print_char
	dec bx
	jnz print_loop

print_done:
 	
endm
	

exit macro
	mov ah,4ch
	int 21h
endm


CODE SEGMENT
START:
   mov ax,data
   mov ds,ax
    mov cx,5
    mov si,0
input_loop:
	display m1
	read_num num1
	mov ax,num1
    mov arr[si],ax
    add si,2
    loop input_loop
  display m2
    mov cx,5
    mov si,0
out_loop:
    mov ax,arr[si]
    PRINT_NUM
    display nl
    add si,2
    loop out_loop

	exit

CODE ENDS
END START
