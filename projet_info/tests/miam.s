# allons au ru
.set noreorder
.text
    ADD $t1,$t2,$t3
    ROTR $t1, $t2, 3
    J boucle
    addi $t1, $t2, byebye
byebye:
    JAL 1
  J byebye
.data
lunchtime : .word 12,boucle
.asciiz "blabla "
