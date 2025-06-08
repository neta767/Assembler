; Semantic error example
MAIN:   mov  r3, NOT_DEFINED_LABEL
        stop
		
; Syntax error example
MAIN:   mvo r3, LENGTH  ; 'mvo' is not a valid opcode
        stop
