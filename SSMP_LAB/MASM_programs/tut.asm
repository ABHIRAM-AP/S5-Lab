; Test File

assume cs:code,ds:data


data segment
	m1 db 10,13,"Enter a number: $"
	num1 dw 0
	nl db 10,13,"$"
exit macro
	mov ah,4ch
	int 21h
endm

print_char macro
	mov ah,02h
	int 21h
endm

display macro msg
	lea dx,msg
	mov ah,09h	
	int 21h
endm

read_num macro var
	mov var,0
read_var:
	mov ah,01h
	int 21h

	cmp al,13
	je done

	sub al,'0'
	mov ah,0

	mov bx,ax
	mov dx,10

	mov ax,var
	mul dx
	add ax,bx
	mov var,ax
	jmp read_var
done:
endm

print_num macro
	cmp ax,0
	jne pn_convert
	mov dl,'0'
	print_char
	jmp pdone

pn_convert:
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
pdone:
endm

data ends

code segment
start:
	mov ax,data
	mov ds,ax

	display m1
	read_num num1
	
	mov ax,num1
	print_num
	exit
	

code ends
end start
