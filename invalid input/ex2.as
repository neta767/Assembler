      Thisssssss lliiiinnneeeee iss wayyyyyyyy tooooo longgggggggggg      TTTTThissssss lliiiinnneeeeeeee isssss wayyyyyyyy tooooo lllllongggggggggggg!!!!!

;Invalid mcro declaration, no name was defined
mcro
mcroend

;Invalid mcro declaration, "mcro" command with extraneous text
e mcro mcro_name
mcroend

;Invalid mcro declaration, an opcode name cannot be used as a mcro name
mcro mov
mcroend

;Invalid mcro declaration, a register name cannot be used as a mcro name
mcro r0
mcroend

;Invalid mcro declaration, an instruction name cannot be used as a mcro name
mcro .data
mcroend

;Invalid mcro declaration, mcro should be assigned with one name
mcro mcro1 mcro2
mcroend

;Invalid mcro declaration, this mcro name is already in use
mcro mymcro
    mov r1, r2
mcroend

mcro mymcro
    add r1, r3
mcroend

;Invalid mcro declaration, "mcroend" command with extraneous text
mcro anothermcro
    sub r2, r4
mcroend extra_text_here

;Invalid mcro declaration, mcro name is too long, 31 characters max
mcro thismcroNameIsWayTooLongAndInvalid
    clr r5
mcroend

;Invalid mcro declaration, empty mcro content
mcro emptymcro

mcroend

;Invalid mcro declaration, "mcro" is a reserved word
mcro mcro
    inc r1
mcroend

;Invalid mcro declaration, "mcroend" is a reserved word
mcro mcroend
    dec r2
mcroend

