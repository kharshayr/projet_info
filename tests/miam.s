# TEST_RETURN_CODE=PASS
# allons au ru

.set noreorder
.text
    Lw $t0 , $t1
    LW $6, -0x200($7)
    ADDI $t1,$zero,8

boucle:
    BEQ $t0 , $t1 , byebye
    NOP
    addi $t1 , $t1 , 1
    J boucle 
    NOP
byebye:
    JAL viteviteauru

.data
lunchtime: .word 12
.space 2
.bss
.space 24
