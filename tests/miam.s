# allons au ru
.set noreorder
.text
   ADD $t1,$t2, $t1
    ROTR $t1, $t2, 1
    MOVE $t2, $t1
    addi $t1, $t2, 3
byebye:
    JAL 1
 NOP
  J 2
.data
lunchtime : .word 12,boucle,12
.asciiz "blabla "
