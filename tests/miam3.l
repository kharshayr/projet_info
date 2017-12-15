<<<<<<< HEAD
  1                   # TEST_RETURN_CODE=PASS
  2                   # allons au ru
  3                   
  4                   
  5                   .set noreorder
  6                   .text
  7 00000000 3C010000     Lw $t0 , lunchtime
  7 00000004 8C280000 
  8 00000008 8CE6FE00     LW $6, -0x200($7)
  9 0000000C 2009199A     ADDI $t1,$zero, 6554
 10                   boucle:
 11 00000010 002A4FC2   ROTR $t1 , $t2 , 31
 12 00000014 11090335     BEQ $t0 , $t1 , 821
 13 00000018 00000000     NOP
 14 0000001C 21290001     addi $t1 , $t1 , 1
 15 00000020 08000004     J boucle
 16 00000024 00000000     NOP
 17                   byebye:
 18 00000028 0C000000     JAL viteviteauru
 19                   .data
 20 00000000 0000000C lunchtime : .word 12 , 3 , 24
 20 00000004 00000003 
 20 00000008 00000018 
 21 0000000C AABBCCDD .byte 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
 21 00000010 FFFFEEFF 
 22 00000012 FFFFFFAA .byte 0xAA
 23 00000013 0000     .space 2
 24                   .asciiz "blabla"
 25 0000001B 0000...  .space 12
 26                   .bss
 27 00000000 0000...  .space 12
 28                   #bla
=======
00000000 3C010000     Lw $t0 , lunchtime
00000004 8C280000 
00000008 8CE6FE00     LW $6, -0x200($7)
0000000C 20290015     ADDI $t1,$at, 0x15
00000010 002A4FC2   ROTR $t1 , $t2 , 31
00000014 11090335     BEQ $t0 , $t1 , 821
00000018 00000000     NOP
0000001C 21290001     addi $t1 , $t1 , 1
00000020 08000004     J boucle
00000024 00000000     NOP
00000028 0C000000     JAL viteviteauru
00000000 0000000C lunchtime : .word 12 , 2
00000000 00000002 
00000000 00000000 .word menu
>>>>>>> 6906d822d7ac745351f669e7c49e945346fa13e0
