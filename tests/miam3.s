# allons au ru
.set noreorder
.text
    Lw $t0 , lunchtime
    LW $6, -0x200($7)
    ADDI $t1,$at, 0x15
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
lunchtime : .word 12 , 2
    .byte 13 , 13 , 13
.asciiz "blabla"
.word menu

.bss
menu:
.space 24
