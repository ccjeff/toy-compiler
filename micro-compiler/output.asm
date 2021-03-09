move $sp,$fp
addi $sp,$sp,4
sw $zero,4($fp)
li $v0,5
syscall
move $t1,$v0
sw $t1,1($fp)
addi $sp,$sp,4
sw $zero,8($fp)
lw $t1,2($fp)
addi $t2,$t1,2
lw $t3,1($fp)
add $t3,$t2,$t3
sw $t3,2($fp)
addi $sp,$sp,4
sw $zero,12($fp)
lw $t1,3($fp)
lw $t2,2($fp)
add $t2,$t1,$t2
addi $t3,$t2,1
sw $t3,3($fp)
addi $sp,$sp,4
sw $zero,16($fp)
lw $t1,4($fp)
lw $t2,3($fp)
add $t2,$t1,$t2
addi $t3,$t2,12
sw $t3,4($fp)
lw $t1, 16($fp)
sw $t1, 4($fp)
addi $sp,$sp,4
sw $zero,20($fp)
lw $t1,5($fp)
lw $t2,1($fp)
add $t2,$t1,$t2
addi $t3,$t2,1
sw $t3,5($fp)
addi $sp,$sp,4
sw $zero,24($fp)
lw $t1, 20($fp)
sw $t1, 24($fp)
addi $sp,$sp,4
sw $zero,28($fp)
li $t1,111111
sw $t1,28($fp)
lw $t1,1($fp)
li $v0,1
move $t0,$t1
syscall
lw $t1,6($fp)
li $v0,1
move $t0,$t1
syscall
addi $sp,$sp,4
sw $zero,32($fp)
lw $t1,8($fp)
lw $t2,1($fp)
add $t2,$t1,$t2
lw $t3,6($fp)
add $t3,$t2,$t3
sw $t3,8($fp)
lw $t1,8($fp)
li $v0,1
move $t0,$t1
syscall
