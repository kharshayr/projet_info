# allons au ru
.set noreorder
.text
   ADD $t1,$t2, $t1
    ROTR $t1, $t2, 1
    NEG $t2 , 2
    addi $t1, $t2, 3
byebye:
  NOP
    JAL boucle
  J lunchtime
.data
lunchtime : .word 12,boucle,12
.asciiz "blabla "
