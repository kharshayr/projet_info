# allons au ru
.set noreorder
.text
    Lw $t0 , lunchtime
    LW $6, -0x200($7)
    ADDI $t1,$zero, 6553
boucle:
  ROTR $t1 , $t2 , 31
    BEQ $t0 , $t1 , byebye
    NOP
    addi $t1 , $t1 , 1
    J boucle
    NOP
byebye:
    JAL viteviteauru
.data
lunchtime : .word 12 , 20
.byte 0xDD , 0xAA , 0xDD
.asciiz "blabla"
