# allons au ru
.set noreorder
.text
ADD $t1,$t2, $t3
 LW $t4 , -0x200($t1)
    NEG $t2 , $t2
    ADDI $t1, $t2, 0x200
byebye :
	BLT $t1 , $t2, boucle
	NOP
	JAL boucle
  J lunchtime
 LW $t4 , toucle
.data
lunchtime : .word 12,boucle,12
.asciiz "blabla "
