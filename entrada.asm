.text
.globl main
main:
label_0:	or $t1, $s3, $s4
beq $s5, $zero, label_0
lw $t1, 8($t2)
sw $t3, 0($t3)
and $t1, $t2, $s4
sub $s1, $s2, $t2
sltiu $t1, $t2, 10
j label_0
addi $t1, $t2, 10
andi $t1, $t2, 10
ori $t1, $t2, 10
slti $t1, $t2, 10
xori $t1, $t2, 10
