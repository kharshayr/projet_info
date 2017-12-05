# allons au ru
.set noreorder
.text
   ADD $t1,$t2,$t2
    ROTR $t1, $t2, 0x55
    J boucle
    addi $t1, $t2, 3
byebye:
    JAL 1
  J 2
.data
lunchtime : .word 12,boucle,12
.asciiz "blabla "
