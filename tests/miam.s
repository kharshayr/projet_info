# TEST_RETURN_CODE=PASS
# allons au ru


.set noreorder
.text
ADDI $t1,$t2, -3
 SW $t4 , boucle
    NEG $t2 , $t2
    ADDI $t1, $t2, 0x200
byebye :
	BLT $t1 , $t2 , boucle
	NOP
	JAL boucle
lunchtime:
 LW $t4 , toucle
.data
.byte 0xFF
.word 12,boucle,12
.asciiz "bl       a::          bla "
.word 12
