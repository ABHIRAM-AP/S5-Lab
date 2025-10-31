; To print each characters in a string


assume cs:code,ds:data
data segment
	m1 db 10,13,"Enter a string: $"	
	m2 db 10,13,"Characters are: $"
	nl db 10,13,"$"

	str db 20,?,20 dup('$')

display macro msg
	lea dx,msg
	mov ah,09h
	int 21h
endm

read macro str
	lea dx,str	
	mov ah,0Ah
	int 21h
endm

display_char macro
	mov ah,02h
	int 21h
endm
	

exit macro
	mov ah,4ch
	int 21h
endm
data ends


code segment
start:
	mov ax,data
	mov ds,ax

	display m1
	read str

	mov cl,str+1
	lea si,str+2
	display nl

print_loop:
	mov dl,[si]
	display_char

	mov dl, 0Dh
    	display_char

	mov dl, 0Ah
    	display_char
	
	inc si

	loop print_loop

	mov ah,4ch
	int 21h
exit
code ends
end start