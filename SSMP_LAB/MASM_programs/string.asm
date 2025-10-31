// Simple String

assume cs:code,ds:data

data segment
	m1 db 10,13,"Enter a string: $"
	m2 db 10,13,"You entered: $"

	buffer db 20
	       db ?
	       db 20 dup('$')
	
data ends

code segment

start:
	mov ax,data
	mov ds,ax

	lea dx,m1
	mov ah,09h 
	int 21h	

	lea dx, buffer
    	mov ah, 0Ah
    	int 21h

	lea dx,m2
	mov ah,09h
	int 21h

	lea  dx, buffer+2

	mov ah,09h
	int 21h

	mov ah,4ch	
	int 21h


code ends
end start