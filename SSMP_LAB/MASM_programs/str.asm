; To replace a character in a string with new one

assume cs:code,ds:data

data segment
	m1 db 10,13,"Enter a string: $"
	m2 db 10,13,"Enter the old char: $"
	m3 db 10,13,"Enter the new char: $"
	
	found_msg db 10,13,"Found n replcaed $"
	not_found db 10,13,"Not found$"

	str db 20,?,20 dup("$")
	old db 2,?,2 dup('$')
	new db 2,?,2 dup('$')
	nl db 10,13,"$"
data ends


display macro msg
	lea dx,msg
	mov ah,09h
	int 21h
endm
display_str macro str
	lea dx,str+2
	mov ah,09h
	int 21h
endm
read macro str
	lea dx,str
	mov ah,0ah
	int 21h
endm
exit macro 
 	mov ah,4ch
	int 21h
endm



code segment
start:
	mov ax,data
	mov ds,ax

	display m1
	read str
	display nl

	display m2
	read old

	display m3
	read new
	display nl

	mov al,old+2
	mov cl,str+1
	mov ch,0
	lea si,str+2
search:
	cmp cx,0
	je not_f

	mov bl,[si]
	cmp bl,al
	je replace

	inc si
	loop search

replace:
	mov dl,new + 2
	mov [si],dl
	display found_msg

	display_str str
	
	exit
not_f:
	display not_found
	exit

code ends
end start