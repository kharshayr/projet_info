# allons au ru
.set noreorder
.text
    ADD $t1,$t2,2
    ROTR $t1, $t2, 2
    J boucle
    addi $t1, $t2, 3
byebye:
    JAL 1
  J byebye
.data
lunchtime : .word 12,boucle
.asciiz "blabla "
