.text
.globl main
main:
label_0:	or $t1, $s3, $s4
beq $s5, $zero, label_0
lw $t1, 8($t2)
sw $t3, 0($t3)
and $t1, $t2, $s4
sub $s1, $s2, $t2
sltiu $t2, $t1, 10
j label_0
beq $t0, $zero, label_1
or $t1, $s3, $s4
or $t1, $s3, $s4
label_1:	or $t1, $s3, $s4
