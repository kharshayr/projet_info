# TEST_RETURN_CODE=PASS
# allons au ru


.set noreorder
.text
    Lw $t0 , lunchtime
    LW $6, -0x200($7)
    ADDI $t1,$zero, 6554
boucle:
  ROTR $t1 , $t2 , 31
    BEQ $t0 , $t1 , 821
    NOP
    addi $t1 , $t1 , 1
    J boucle
    NOP
byebye:
    JAL viteviteauru
.data
lunchtime : .word 12 , 3 , 24
.byte 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
.byte 0xAA
.space 2
.asciiz "blabla"
.word menu

.bss
menu:
.space 24
