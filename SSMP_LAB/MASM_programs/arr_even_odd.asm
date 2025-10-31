; To get even and odd count in an array

ASSUME CS:CODE, DS:DATA

DATA SEGMENT
    arr         DB 5, 19, 13, 24, 7    ; array (5 elements)
    ; n is optional - we'll set loop count with an immediate
    even_count  DB 0
    odd_count   DB 0
    msg1        DB 10,13, "Even count: $"
    msg2        DB 10,13, "Odd count : $"
DATA ENDS

CODE SEGMENT
START:
    ; initialize data segment
    MOV AX, DATA
    MOV DS, AX

    ; initialize loop and pointers
    MOV CX, 5           ; number of elements
    LEA SI, arr         ; SI -> first element
    MOV BL, 0           ; BL = even count
    MOV BH, 0           ; BH = odd count

NEXT_ELEM:
    MOV AL, [SI]        ; load array element (byte)
    AND AL, 01H         ; isolate LSB
    JZ IS_EVEN          ; if zero → even
    INC BH   
	INC SI
               ; odd++
    LOOP NEXT_ELEM

IS_EVEN:
    INC BL
    JMP NEXT_ELEM              ; even++

AFTER_CHECK:
     ; decrement CX and loop if not zero

    ; store counts to memory (optional)
    MOV even_count, BL
    MOV odd_count, BH

    ; --- Print Even count ---
    MOV AH, 09H
    LEA DX, msg1
    INT 21H

    MOV AL, even_count  ; AL = even count
    CALL DISP_NUM

    ; print newline for safety
    MOV AH, 02H
    MOV DL, 10
    INT 21H
    MOV DL, 13
    INT 21H

    ; --- Print Odd count ---
    MOV AH, 09H
    LEA DX, msg2
    INT 21H

    MOV AL, odd_count   ; AL = odd count
    CALL DISP_NUM

    ; final newline and exit
    MOV AH, 02H
    MOV DL, 10
    INT 21H
    MOV DL, 13
    INT 21H

    MOV AH, 4CH
    INT 21H

; ----------------------------------------------------------------
; DISP_NUM: display number in AL (0..255 but we only expect small counts)
; - prints decimal representation without leading zero for values <10
; - preserves registers used by caller except AL is consumed
; ----------------------------------------------------------------
DISP_NUM PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX

    MOV AH, 0
    MOV BL, 10
    ; prepare AX = 0:AL for DIV
    MOV AH, 0
    MOV AL, AL          ; AL already holds the number
    DIV BL              ; AX / 10 -> AL = quotient (tens), AH = remainder (ones)

    CMP AL, 0
    JE .print_one_digit ; if quotient == 0 → number < 10

    ; print tens digit
    ADD AL, '0'
    MOV DL, AL
    MOV AH, 02H
    INT 21H

    ; print ones digit (remainder in AH)
    MOV AL, AH
    ADD AL, '0'
    MOV DL, AL
    MOV AH, 02H
    INT 21H
    JMP .done

.print_one_digit:
    ; remainder in AH is the number itself
    MOV AL, AH
    ADD AL, '0'
    MOV DL, AL
    MOV AH, 02H
    INT 21H

.done:
    POP DX
    POP CX
    POP BX
    POP AX
    RET
DISP_NUM ENDP

CODE ENDS
END START
