# allons au ru
.set noreorder
.text
ADD $t1,$t2, $t1
ROTR $t1, $t2, 1 
	NEG $t2 , $t2
   addi $t1, $t2, 3
byebye:
BLT $t1 , $t2 , boucle
NOP
JAL boucle
  J lunchtime
.data
lunchtime : .word 12,boucle,12
.asciiz "blabla "
