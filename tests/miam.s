# allons au ru
.set noreorder
.text
ADD $t1,$t2, $t3
 LW $t4, lunchtime
    NEG $t2 , $t2
    ADDI $t1, $t2, 0x200
byebye :
	BLT $t1 , $t2, boucle
	NOP
	JAL boucle
  J lunchtime
.data
lunchtime : .word 12,boucle,12
.asciiz "blabla "
