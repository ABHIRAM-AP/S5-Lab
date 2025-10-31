// Linear Search

assume cs:code,ds:data

data segment
	msg1 db 10,13,"Item found $"
	msg2 db 10,13,"Item Not found $"
	arr db 10,20,30,40,50
	arr_length db 5
data ends

DISPLAY MACRO msg
	lea dx,msg
	mov ah,09h
	int 21h
ENDM


code segment
start:
	mov ax,data
	mov ds,ax

	mov al,20
	mov cl,arr_length
	lea si,arr

search:
	cmp cl,0	
	je not_found

	mov bl,[si]
	cmp bl,al
	je found

	inc si
	dec cl
	jmp search


found:	
	display msg1
	jmp exit

not_found:
	display msg2
	jmp exit

exit:
	mov ah,4ch
	int 21h



code ends
end start