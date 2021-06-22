	.global	v0
	.section	.sdata
	.align	2
	.type	v0, @object
	.size	v0, 4
v0:
	.word	0
	.text
	.align	2
	.global	bubblesort
	.type	bubblesort, @function
bubblesort:
	addi	sp, sp, -112
	sw	ra, 108(sp)
	lw	t0, 4(sp)
	li	t0, 0
	sw	t0, 4(sp)
	sw	a0, 0(sp)
.l0:
	lw	t0, 12(sp)
	lui	t1, %hi(v0)
	lw	t1, %lo(v0)(t1)
	li	s0, -1
	add	t0, t1, s0
	lw	t2, 16(sp)
	lw	t3, 4(sp)
	slt	t2, t3, t0
	sw	t2, 16(sp)
	beq	t2, x0, .l4
	lw	t0, 8(sp)
	li	t0, 0
	sw	t0, 8(sp)
.l1:
	lw	t0, 20(sp)
	lui	t1, %hi(v0)
	lw	t1, %lo(v0)(t1)
	lw	t2, 4(sp)
	sub	t0, t1, t2
	lw	t3, 24(sp)
	li	s0, -1
	add	t3, t0, s0
	lw	t4, 28(sp)
	lw	t5, 8(sp)
	slt	t4, t5, t3
	sw	t4, 28(sp)
	beq	t4, x0, .l3
	lw	t0, 32(sp)
	lw	t1, 8(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 36(sp)
	lw	t4, 0(sp)
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 40(sp)
	li	s0, 1
	add	t6, t1, s0
	lw	s1, 44(sp)
	li	s2, 4
	mul	s1, t6, s2
	lw	s3, 48(sp)
	add	s4, t4, s1
	lw	s3, 0(s4)
	lw	s5, 52(sp)
	sgt	s5, t3, s3
	sw	s5, 52(sp)
	beq	s5, x0, .l2
	lw	t0, 60(sp)
	lw	t1, 8(sp)
	li	s0, 1
	add	t0, t1, s0
	lw	t2, 64(sp)
	li	t3, 4
	mul	t2, t0, t3
	lw	t4, 68(sp)
	lw	t5, 0(sp)
	add	t6, t5, t2
	lw	t4, 0(t6)
	lw	s1, 56(sp)
	mv	s1, t4
	lw	s2, 72(sp)
	li	s0, 1
	add	s2, t1, s0
	lw	s3, 76(sp)
	li	s4, 4
	mul	s3, s2, s4
	lw	s5, 80(sp)
	li	s6, 4
	mul	s5, t1, s6
	lw	s7, 84(sp)
	add	s8, t5, s5
	lw	s7, 0(s8)
	add	s9, t5, s3
	sw	s7, 0(s9)
	lw	s10, 88(sp)
	li	s11, 4
	mul	s10, t1, s11
	add	a0, t5, s10
	sw	s1, 0(a0)
	sw	s1, 56(sp)
	sw	s10, 88(sp)
.l2:
	lw	t0, 92(sp)
	lw	t1, 8(sp)
	li	s0, 1
	add	t0, t1, s0
	mv	t1, t0
	sw	t1, 8(sp)
	j	.l1
.l3:
	lw	t0, 96(sp)
	lw	t1, 4(sp)
	li	s0, 1
	add	t0, t1, s0
	mv	t1, t0
	sw	t1, 4(sp)
	j	.l0
.l4:
	li	a0, 0
	lw	ra, 108(sp)
	addi	sp, sp, 112
	ret
	.size	bubblesort, .-bubblesort
	.text
	.align	2
	.global	insertsort
	.type	insertsort, @function
insertsort:
	addi	sp, sp, -96
	sw	ra, 92(sp)
	lw	t0, 4(sp)
	li	t0, 1
	sw	t0, 4(sp)
	sw	a0, 0(sp)
.l5:
	lw	t0, 8(sp)
	lw	t1, 4(sp)
	lui	t2, %hi(v0)
	lw	t2, %lo(v0)(t2)
	slt	t0, t1, t2
	sw	t0, 8(sp)
	beq	t0, x0, .l10
	lw	t0, 16(sp)
	lw	t1, 4(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 20(sp)
	lw	t4, 0(sp)
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 12(sp)
	mv	t6, t3
	lw	s1, 28(sp)
	li	s0, -1
	add	s1, t1, s0
	lw	s2, 24(sp)
	mv	s2, s1
	sw	s1, 28(sp)
	sw	s2, 24(sp)
	sw	t6, 12(sp)
.l6:
	lw	t0, 32(sp)
	lw	t1, 24(sp)
	li	t2, -1
	sgt	t0, t1, t2
	sw	t0, 32(sp)
	beq	t0, x0, .l7
	lw	t0, 40(sp)
	lw	t1, 24(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 44(sp)
	lw	t4, 0(sp)
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 48(sp)
	lw	s1, 12(sp)
	slt	t6, s1, t3
	sw	t6, 48(sp)
	beq	t6, x0, .l7
	lw	t0, 36(sp)
	li	t0, 1
	sw	t0, 36(sp)
	j	.l8
.l7:
	lw	t0, 36(sp)
	li	t0, 0
	sw	t0, 36(sp)
.l8:
	lw	t0, 36(sp)
	beq	t0, x0, .l9
	lw	t0, 52(sp)
	lw	t1, 24(sp)
	li	s0, 1
	add	t0, t1, s0
	lw	t2, 56(sp)
	li	t3, 4
	mul	t2, t0, t3
	lw	t4, 60(sp)
	li	t5, 4
	mul	t4, t1, t5
	lw	t6, 64(sp)
	lw	s1, 0(sp)
	add	s2, s1, t4
	lw	t6, 0(s2)
	add	s3, s1, t2
	sw	t6, 0(s3)
	lw	s4, 68(sp)
	li	s0, -1
	add	s4, t1, s0
	mv	t1, s4
	sw	t1, 24(sp)
	j	.l6
.l9:
	lw	t0, 72(sp)
	lw	t1, 24(sp)
	li	s0, 1
	add	t0, t1, s0
	lw	t2, 76(sp)
	li	t3, 4
	mul	t2, t0, t3
	lw	t4, 0(sp)
	lw	t5, 12(sp)
	add	t6, t4, t2
	sw	t5, 0(t6)
	lw	s1, 80(sp)
	lw	s2, 4(sp)
	li	s0, 1
	add	s1, s2, s0
	mv	s2, s1
	sw	s2, 4(sp)
	j	.l5
.l10:
	li	a0, 0
	lw	ra, 92(sp)
	addi	sp, sp, 96
	ret
	.size	insertsort, .-insertsort
	.text
	.align	2
	.global	QuickSort
	.type	QuickSort, @function
QuickSort:
	addi	sp, sp, -192
	sw	ra, 188(sp)
	lw	t0, 12(sp)
	slt	t0, a1, a2
	sw	t0, 12(sp)
	sw	a0, 0(sp)
	sw	a1, 4(sp)
	sw	a2, 8(sp)
	beq	t0, x0, .l23
	lw	t0, 16(sp)
	lw	t1, 4(sp)
	mv	t0, t1
	lw	t2, 20(sp)
	lw	t3, 8(sp)
	mv	t2, t3
	lw	t4, 28(sp)
	li	t5, 4
	mul	t4, t1, t5
	lw	t6, 32(sp)
	lw	s1, 0(sp)
	add	s2, s1, t4
	lw	t6, 0(s2)
	lw	s3, 24(sp)
	mv	s3, t6
	sw	s3, 24(sp)
	sw	t0, 16(sp)
	sw	t2, 20(sp)
	sw	t6, 32(sp)
.l11:
	lw	t0, 36(sp)
	lw	t1, 16(sp)
	lw	t2, 20(sp)
	slt	t0, t1, t2
	sw	t0, 36(sp)
	beq	t0, x0, .l22
.l12:
	lw	t0, 40(sp)
	lw	t1, 16(sp)
	lw	t2, 20(sp)
	slt	t0, t1, t2
	sw	t0, 40(sp)
	beq	t0, x0, .l13
	lw	t0, 48(sp)
	lw	t1, 20(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 52(sp)
	lw	t4, 0(sp)
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 56(sp)
	lw	s1, 24(sp)
	li	s0, -1
	add	t6, s1, s0
	lw	s2, 60(sp)
	sgt	s2, t3, t6
	sw	s2, 60(sp)
	beq	s2, x0, .l13
	lw	t0, 44(sp)
	li	t0, 1
	sw	t0, 44(sp)
	j	.l14
.l13:
	lw	t0, 44(sp)
	li	t0, 0
	sw	t0, 44(sp)
.l14:
	lw	t0, 44(sp)
	beq	t0, x0, .l15
	lw	t0, 64(sp)
	lw	t1, 20(sp)
	li	s0, -1
	add	t0, t1, s0
	mv	t1, t0
	sw	t1, 20(sp)
	j	.l12
.l15:
	lw	t0, 68(sp)
	lw	t1, 16(sp)
	lw	t2, 20(sp)
	slt	t0, t1, t2
	sw	t0, 68(sp)
	beq	t0, x0, .l16
	lw	t0, 72(sp)
	lw	t1, 16(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 76(sp)
	lw	t4, 20(sp)
	li	t5, 4
	mul	t3, t4, t5
	lw	t6, 80(sp)
	lw	s1, 0(sp)
	add	s2, s1, t3
	lw	t6, 0(s2)
	add	s3, s1, t0
	sw	t6, 0(s3)
	lw	s4, 84(sp)
	li	s0, 1
	add	s4, t1, s0
	mv	t1, s4
	sw	s4, 84(sp)
	sw	t1, 16(sp)
	sw	t1, 16(sp)
.l16:
.l17:
	lw	t0, 88(sp)
	lw	t1, 16(sp)
	lw	t2, 20(sp)
	slt	t0, t1, t2
	sw	t0, 88(sp)
	beq	t0, x0, .l18
	lw	t0, 96(sp)
	lw	t1, 16(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 100(sp)
	lw	t4, 0(sp)
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 104(sp)
	lw	s1, 24(sp)
	slt	t6, t3, s1
	sw	t6, 104(sp)
	beq	t6, x0, .l18
	lw	t0, 92(sp)
	li	t0, 1
	sw	t0, 92(sp)
	j	.l19
.l18:
	lw	t0, 92(sp)
	li	t0, 0
	sw	t0, 92(sp)
.l19:
	lw	t0, 92(sp)
	beq	t0, x0, .l20
	lw	t0, 108(sp)
	lw	t1, 16(sp)
	li	s0, 1
	add	t0, t1, s0
	mv	t1, t0
	sw	t1, 16(sp)
	j	.l17
.l20:
	lw	t0, 112(sp)
	lw	t1, 16(sp)
	lw	t2, 20(sp)
	slt	t0, t1, t2
	sw	t0, 112(sp)
	beq	t0, x0, .l21
	lw	t0, 116(sp)
	lw	t1, 20(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 120(sp)
	lw	t4, 16(sp)
	li	t5, 4
	mul	t3, t4, t5
	lw	t6, 124(sp)
	lw	s1, 0(sp)
	add	s2, s1, t3
	lw	t6, 0(s2)
	add	s3, s1, t0
	sw	t6, 0(s3)
	lw	s4, 128(sp)
	li	s0, -1
	add	s4, t1, s0
	mv	t1, s4
	sw	s4, 128(sp)
	sw	t1, 20(sp)
.l21:
	j	.l11
.l22:
	lw	t0, 132(sp)
	lw	t1, 16(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 0(sp)
	lw	t4, 24(sp)
	add	t5, t3, t0
	sw	t4, 0(t5)
	lw	t6, 140(sp)
	li	s0, -1
	add	t6, t1, s0
	lw	s1, 136(sp)
	mv	s1, t6
	lw	s2, 144(sp)
	li	s0, 0
	add	s2, t3, s0
	lw	s3, 148(sp)
	mv	s3, s2
	lw	s4, 152(sp)
	lw	s5, 4(sp)
	mv	s4, s5
	lw	s6, 156(sp)
	mv	s6, s1
	mv	a0, s3
	mv	a1, s4
	mv	a2, s6
	sw	s1, 136(sp)
	call	QuickSort
	lw	t0, 136(sp)
	mv	t0, a0
	lw	t1, 164(sp)
	lw	t3, 16(sp)
	li	s0, 1
	add	t1, t3, s0
	mv	t0, t1
	lw	t4, 168(sp)
	lw	t6, 0(sp)
	li	s0, 0
	add	t4, t6, s0
	lw	s1, 172(sp)
	mv	s1, t4
	lw	s2, 176(sp)
	mv	s2, t0
	lw	s3, 180(sp)
	lw	s4, 8(sp)
	mv	s3, s4
	mv	a0, s1
	mv	a1, s2
	mv	a2, s3
	call	QuickSort
	mv	t0, a0
	sw	a0, 184(sp)
.l23:
	li	a0, 0
	lw	ra, 188(sp)
	addi	sp, sp, 192
	ret
	.size	QuickSort, .-QuickSort
	.text
	.align	2
	.global	getMid
	.type	getMid, @function
getMid:
	addi	sp, sp, -64
	sw	ra, 60(sp)
	lw	t0, 8(sp)
	lui	t1, %hi(v0)
	lw	t1, %lo(v0)(t1)
	li	t2, 2
	rem	t0, t1, t2
	lw	t3, 12(sp)
	xor	t3, t0, x0
	seqz	t3, t3
	sw	t3, 12(sp)
	sw	a0, 0(sp)
	beq	t3, x0, .l24
	lw	t0, 16(sp)
	lui	t1, %hi(v0)
	lw	t1, %lo(v0)(t1)
	li	t2, 2
	div	t0, t1, t2
	lw	t3, 4(sp)
	mv	t3, t0
	lw	t4, 20(sp)
	li	t5, 4
	mul	t4, t3, t5
	lw	t6, 24(sp)
	lw	s1, 0(sp)
	add	s2, s1, t4
	lw	t6, 0(s2)
	lw	s3, 28(sp)
	li	s0, -1
	add	s3, t3, s0
	lw	s4, 32(sp)
	li	s5, 4
	mul	s4, s3, s5
	lw	s6, 36(sp)
	add	s7, s1, s4
	lw	s6, 0(s7)
	lw	s8, 40(sp)
	add	s8, t6, s6
	lw	s9, 44(sp)
	li	s10, 2
	div	s9, s8, s10
	mv	a0, s9
	sw	s9, 44(sp)
	sw	t3, 4(sp)
	lw	ra, 60(sp)
	addi	sp, sp, 64
	ret
	j	.l25
.l24:
	lw	t0, 48(sp)
	lui	t1, %hi(v0)
	lw	t1, %lo(v0)(t1)
	li	t2, 2
	div	t0, t1, t2
	lw	t3, 4(sp)
	mv	t3, t0
	lw	t4, 52(sp)
	li	t5, 4
	mul	t4, t3, t5
	lw	t6, 56(sp)
	lw	s1, 0(sp)
	add	s2, s1, t4
	lw	t6, 0(s2)
	mv	a0, t6
	sw	t6, 56(sp)
	lw	ra, 60(sp)
	addi	sp, sp, 64
	ret
.l25:
	li	a0, 0
	lw	ra, 60(sp)
	addi	sp, sp, 64
	ret
	.size	getMid, .-getMid
	.text
	.align	2
	.global	getMost
	.type	getMost, @function
getMost:
	li	s0, -4096
	add	sp, sp, s0
	li	s0, 4092
	add	s0, s0, sp
	sw	ra, 0(s0)
	li	s0, 4004
	add	s0, s0, sp
	lw	t0, 0(s0)
	li	t0, 0
	li	s0, 4004
	add	s0, s0, sp
	sw	t0, 0(s0)
	sw	a0, 0(sp)
.l26:
	li	s0, 4008
	add	s0, s0, sp
	lw	t0, 0(s0)
	li	s0, 4004
	add	s0, s0, sp
	lw	t1, 0(s0)
	li	s0, 1000
	slt	t0, t1, s0
	li	s0, 4008
	add	s0, s0, sp
	sw	t0, 0(s0)
	beq	t0, x0, .l27
	li	s0, 4012
	add	s0, s0, sp
	lw	t0, 0(s0)
	li	s0, 4004
	add	s0, s0, sp
	lw	t1, 0(s0)
	li	t2, 4
	mul	t0, t1, t2
	addi	t3, sp, 4
	add	t4, t3, t0
	sw	x0, 0(t4)
	li	s0, 4016
	add	s0, s0, sp
	lw	t5, 0(s0)
	li	s0, 1
	add	t5, t1, s0
	mv	t1, t5
	li	s0, 4004
	add	s0, s0, sp
	sw	t1, 0(s0)
	j	.l26
.l27:
	li	s0, 4004
	add	s0, s0, sp
	lw	t0, 0(s0)
	li	t0, 0
	li	s0, 4020
	add	s0, s0, sp
	lw	t1, 0(s0)
	li	t1, 0
	li	s0, 4004
	add	s0, s0, sp
	sw	t0, 0(s0)
	li	s0, 4020
	add	s0, s0, sp
	sw	t1, 0(s0)
.l28:
	li	s0, 4028
	add	s0, s0, sp
	lw	t0, 0(s0)
	li	s0, 4004
	add	s0, s0, sp
	lw	t1, 0(s0)
	lui	t2, %hi(v0)
	lw	t2, %lo(v0)(t2)
	slt	t0, t1, t2
	li	s0, 4028
	add	s0, s0, sp
	sw	t0, 0(s0)
	beq	t0, x0, .l30
	li	s0, 4036
	add	s0, s0, sp
	lw	t0, 0(s0)
	li	s0, 4004
	add	s0, s0, sp
	lw	t1, 0(s0)
	li	t2, 4
	mul	t0, t1, t2
	li	s0, 4040
	add	s0, s0, sp
	lw	t3, 0(s0)
	lw	t4, 0(sp)
	add	t5, t4, t0
	lw	t3, 0(t5)
	li	s0, 4032
	add	s0, s0, sp
	lw	t6, 0(s0)
	mv	t6, t3
	li	s0, 4044
	add	s0, s0, sp
	lw	s1, 0(s0)
	li	s2, 4
	mul	s1, t6, s2
	li	s0, 4048
	add	s0, s0, sp
	lw	s3, 0(s0)
	li	s4, 4
	mul	s3, t6, s4
	li	s0, 4052
	add	s0, s0, sp
	lw	s5, 0(s0)
	addi	s6, sp, 4
	add	s7, s6, s3
	lw	s5, 0(s7)
	li	s0, 4056
	add	s0, s0, sp
	lw	s8, 0(s0)
	li	s0, 1
	add	s8, s5, s0
	add	s9, s6, s1
	sw	s8, 0(s9)
	li	s0, 4060
	add	s0, s0, sp
	lw	s10, 0(s0)
	li	s11, 4
	mul	s10, t6, s11
	li	s0, 4064
	add	s0, s0, sp
	lw	a0, 0(s0)
	add	a1, s6, s10
	lw	a0, 0(a1)
	li	s0, 4068
	add	s0, s0, sp
	lw	a2, 0(s0)
	li	s0, 4020
	add	s0, s0, sp
	lw	a3, 0(s0)
	sgt	a2, a0, a3
	li	s0, 4032
	add	s0, s0, sp
	sw	t6, 0(s0)
	li	s0, 4068
	add	s0, s0, sp
	sw	a2, 0(s0)
	beq	a2, x0, .l29
	li	s0, 4072
	add	s0, s0, sp
	lw	t0, 0(s0)
	li	s0, 4032
	add	s0, s0, sp
	lw	t1, 0(s0)
	li	t2, 4
	mul	t0, t1, t2
	li	s0, 4076
	add	s0, s0, sp
	lw	t3, 0(s0)
	addi	t4, sp, 4
	add	t5, t4, t0
	lw	t3, 0(t5)
	li	s0, 4020
	add	s0, s0, sp
	lw	t6, 0(s0)
	mv	t6, t3
	li	s0, 4024
	add	s0, s0, sp
	lw	s1, 0(s0)
	mv	s1, t1
	li	s0, 4024
	add	s0, s0, sp
	sw	s1, 0(s0)
	li	s0, 4020
	add	s0, s0, sp
	sw	t6, 0(s0)
.l29:
	li	s0, 4080
	add	s0, s0, sp
	lw	t0, 0(s0)
	li	s0, 4004
	add	s0, s0, sp
	lw	t1, 0(s0)
	li	s0, 1
	add	t0, t1, s0
	mv	t1, t0
	li	s0, 4004
	add	s0, s0, sp
	sw	t1, 0(s0)
	j	.l28
.l30:
	li	s0, 4024
	add	s0, s0, sp
	lw	a0, 0(s0)
	li	s0, 4092
	add	s0, s0, sp
	lw	ra, 0(s0)
	li	s0, 4096
	add	sp, sp, s0
	ret
	.size	getMost, .-getMost
	.text
	.align	2
	.global	revert
	.type	revert, @function
revert:
	addi	sp, sp, -64
	sw	ra, 60(sp)
	lw	t0, 8(sp)
	li	t0, 0
	lw	t1, 12(sp)
	li	t1, 0
	sw	t0, 8(sp)
	sw	t1, 12(sp)
	sw	a0, 0(sp)
.l31:
	lw	t0, 16(sp)
	lw	t1, 8(sp)
	lw	t2, 12(sp)
	slt	t0, t1, t2
	sw	t0, 16(sp)
	beq	t0, x0, .l32
	lw	t0, 20(sp)
	lw	t1, 8(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 24(sp)
	lw	t4, 0(sp)
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 4(sp)
	mv	t6, t3
	lw	s1, 28(sp)
	li	s2, 4
	mul	s1, t1, s2
	lw	s3, 32(sp)
	lw	s4, 12(sp)
	li	s5, 4
	mul	s3, s4, s5
	lw	s6, 36(sp)
	add	s7, t4, s3
	lw	s6, 0(s7)
	add	s8, t4, s1
	sw	s6, 0(s8)
	lw	s9, 40(sp)
	li	s10, 4
	mul	s9, s4, s10
	add	s11, t4, s9
	sw	t6, 0(s11)
	lw	a0, 44(sp)
	li	s0, 1
	add	a0, t1, s0
	mv	t1, a0
	lw	a1, 48(sp)
	li	s0, -1
	add	a1, s4, s0
	mv	s4, a1
	sw	s4, 12(sp)
	sw	t1, 8(sp)
	j	.l31
.l32:
	li	a0, 0
	lw	ra, 60(sp)
	addi	sp, sp, 64
	ret
	.size	revert, .-revert
	.text
	.align	2
	.global	arrCopy
	.type	arrCopy, @function
arrCopy:
	addi	sp, sp, -48
	sw	ra, 44(sp)
	lw	t0, 8(sp)
	li	t0, 0
	sw	t0, 8(sp)
	sw	a0, 0(sp)
	sw	a1, 4(sp)
.l33:
	lw	t0, 12(sp)
	lw	t1, 8(sp)
	lui	t2, %hi(v0)
	lw	t2, %lo(v0)(t2)
	slt	t0, t1, t2
	sw	t0, 12(sp)
	beq	t0, x0, .l34
	lw	t0, 16(sp)
	lw	t1, 8(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 20(sp)
	li	t4, 4
	mul	t3, t1, t4
	lw	t5, 24(sp)
	lw	t6, 0(sp)
	add	s1, t6, t3
	lw	t5, 0(s1)
	lw	s2, 4(sp)
	add	s3, s2, t0
	sw	t5, 0(s3)
	lw	s4, 28(sp)
	li	s0, 1
	add	s4, t1, s0
	mv	t1, s4
	sw	t1, 8(sp)
	j	.l33
.l34:
	li	a0, 0
	lw	ra, 44(sp)
	addi	sp, sp, 48
	ret
	.size	arrCopy, .-arrCopy
	.text
	.align	2
	.global	calSum
	.type	calSum, @function
calSum:
	addi	sp, sp, -64
	sw	ra, 60(sp)
	lw	t0, 8(sp)
	li	t0, 0
	lw	t1, 12(sp)
	li	t1, 0
	sw	t0, 8(sp)
	sw	t1, 12(sp)
	sw	a0, 0(sp)
	sw	a1, 4(sp)
.l35:
	lw	t0, 16(sp)
	lw	t1, 12(sp)
	lui	t2, %hi(v0)
	lw	t2, %lo(v0)(t2)
	slt	t0, t1, t2
	sw	t0, 16(sp)
	beq	t0, x0, .l38
	lw	t0, 20(sp)
	lw	t1, 12(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 24(sp)
	lw	t4, 0(sp)
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 28(sp)
	lw	s1, 8(sp)
	add	t6, s1, t3
	mv	s1, t6
	lw	s2, 32(sp)
	lw	s3, 4(sp)
	rem	s2, t1, s3
	lw	s4, 36(sp)
	li	s0, -1
	add	s4, s3, s0
	lw	s5, 40(sp)
	xor	s5, s2, s4
	snez	s5, s5
	sw	s1, 8(sp)
	sw	s5, 40(sp)
	beq	s5, x0, .l36
	lw	t0, 44(sp)
	lw	t1, 12(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 0(sp)
	add	t4, t3, t0
	sw	x0, 0(t4)
	j	.l37
.l36:
	lw	t0, 48(sp)
	lw	t1, 12(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 0(sp)
	lw	t4, 8(sp)
	add	t5, t3, t0
	sw	t4, 0(t5)
	li	t4, 0
	sw	t4, 8(sp)
.l37:
	lw	t0, 52(sp)
	lw	t1, 12(sp)
	li	s0, 1
	add	t0, t1, s0
	mv	t1, t0
	sw	t1, 12(sp)
	j	.l35
.l38:
	li	a0, 0
	lw	ra, 60(sp)
	addi	sp, sp, 64
	ret
	.size	calSum, .-calSum
	.text
	.align	2
	.global	avgPooling
	.type	avgPooling, @function
avgPooling:
	addi	sp, sp, -144
	sw	ra, 140(sp)
	lw	t0, 12(sp)
	li	t0, 0
	lw	t1, 8(sp)
	li	t1, 0
	sw	t0, 12(sp)
	sw	t1, 8(sp)
	sw	a0, 0(sp)
	sw	a1, 4(sp)
.l39:
	lw	t0, 20(sp)
	lw	t1, 12(sp)
	lui	t2, %hi(v0)
	lw	t2, %lo(v0)(t2)
	slt	t0, t1, t2
	sw	t0, 20(sp)
	beq	t0, x0, .l44
	lw	t0, 24(sp)
	lw	t1, 4(sp)
	li	s0, -1
	add	t0, t1, s0
	lw	t2, 28(sp)
	lw	t3, 12(sp)
	slt	t2, t3, t0
	sw	t2, 28(sp)
	beq	t2, x0, .l40
	lw	t0, 32(sp)
	lw	t1, 12(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 36(sp)
	lw	t4, 0(sp)
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 40(sp)
	lw	s1, 8(sp)
	add	t6, s1, t3
	mv	s1, t6
	sw	s1, 8(sp)
	j	.l43
.l40:
	lw	t0, 44(sp)
	lw	t1, 4(sp)
	li	s0, -1
	add	t0, t1, s0
	lw	t2, 48(sp)
	lw	t3, 12(sp)
	xor	t2, t3, t0
	seqz	t2, t2
	sw	t2, 48(sp)
	beq	t2, x0, .l41
	lw	t0, 52(sp)
	lw	t1, 0(sp)
	lw	t0, 0(t1)
	lw	t2, 16(sp)
	mv	t2, t0
	lw	t3, 56(sp)
	lw	t4, 8(sp)
	lw	t5, 4(sp)
	div	t3, t4, t5
	sw	t3, 0(t1)
	sw	t2, 16(sp)
	j	.l42
.l41:
	lw	t0, 60(sp)
	lw	t1, 12(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 64(sp)
	lw	t4, 0(sp)
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 68(sp)
	lw	s1, 8(sp)
	add	t6, s1, t3
	lw	s2, 72(sp)
	lw	s3, 16(sp)
	sub	s2, t6, s3
	mv	s1, s2
	lw	s4, 76(sp)
	lw	s5, 4(sp)
	sub	s4, t1, s5
	lw	s6, 80(sp)
	li	s0, 1
	add	s6, s4, s0
	lw	s7, 84(sp)
	li	s8, 4
	mul	s7, s6, s8
	lw	s9, 88(sp)
	add	s10, t4, s7
	lw	s9, 0(s10)
	mv	s3, s9
	lw	s11, 92(sp)
	sub	s11, t1, s5
	lw	a0, 96(sp)
	li	s0, 1
	add	a0, s11, s0
	lw	a1, 100(sp)
	li	a2, 4
	mul	a1, a0, a2
	lw	a3, 104(sp)
	div	a3, s1, s5
	add	a4, t4, a1
	sw	a3, 0(a4)
	sw	s1, 8(sp)
	sw	s3, 16(sp)
	sw	a1, 100(sp)
	sw	a3, 104(sp)
	sw	s1, 8(sp)
	sw	s3, 16(sp)
.l42:
.l43:
	lw	t0, 108(sp)
	lw	t1, 12(sp)
	li	s0, 1
	add	t0, t1, s0
	mv	t1, t0
	sw	t1, 12(sp)
	j	.l39
.l44:
	lw	t0, 112(sp)
	lui	t1, %hi(v0)
	lw	t1, %lo(v0)(t1)
	lw	t2, 4(sp)
	sub	t0, t1, t2
	lw	t3, 116(sp)
	li	s0, 1
	add	t3, t0, s0
	lw	t4, 12(sp)
	mv	t4, t3
	sw	t3, 116(sp)
	sw	t4, 12(sp)
.l45:
	lw	t0, 120(sp)
	lw	t1, 12(sp)
	lui	t2, %hi(v0)
	lw	t2, %lo(v0)(t2)
	slt	t0, t1, t2
	sw	t0, 120(sp)
	beq	t0, x0, .l46
	lw	t0, 124(sp)
	lw	t1, 12(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 0(sp)
	add	t4, t3, t0
	sw	x0, 0(t4)
	lw	t5, 128(sp)
	li	s0, 1
	add	t5, t1, s0
	mv	t1, t5
	sw	t1, 12(sp)
	j	.l45
.l46:
	li	a0, 0
	lw	ra, 140(sp)
	addi	sp, sp, 144
	ret
	.size	avgPooling, .-avgPooling
	.text
	.align	2
	.global	main
	.type	main, @function
main:
	addi	sp, sp, -656
	sw	ra, 652(sp)
	lui	t0, %hi(v0)
	lw	t0, %lo(v0)(t0)
	li	t0, 32
	addi	t1, sp, 0
	li	t2, 7
	sw	t2, 0(t1)
	li	t3, 23
	sw	t3, 4(t1)
	li	t4, 89
	sw	t4, 8(t1)
	li	t5, 26
	sw	t5, 12(t1)
	li	t6, 282
	sw	t6, 16(t1)
	li	s1, 254
	sw	s1, 20(t1)
	li	s2, 27
	sw	s2, 24(t1)
	li	s3, 5
	sw	s3, 28(t1)
	li	s4, 83
	sw	s4, 32(t1)
	li	s5, 273
	sw	s5, 36(t1)
	li	s6, 574
	sw	s6, 40(t1)
	li	s7, 905
	sw	s7, 44(t1)
	li	s8, 354
	sw	s8, 48(t1)
	li	s9, 657
	sw	s9, 52(t1)
	li	s10, 935
	sw	s10, 56(t1)
	li	s11, 264
	sw	s11, 60(t1)
	li	a0, 639
	sw	a0, 64(t1)
	li	a1, 459
	sw	a1, 68(t1)
	li	a2, 29
	sw	a2, 72(t1)
	li	a3, 68
	sw	a3, 76(t1)
	li	a4, 929
	sw	a4, 80(t1)
	li	a5, 756
	sw	a5, 84(t1)
	li	a6, 452
	sw	a6, 88(t1)
	li	a7, 279
	sw	a7, 92(t1)
	la	s0, v0
	sw	t0, 0(s0)
	li	t0, 58
	sw	t0, 96(t1)
	li	t0, 87
	sw	t0, 100(t1)
	li	t0, 96
	sw	t0, 104(t1)
	li	t0, 36
	sw	t0, 108(t1)
	li	t0, 39
	sw	t0, 112(t1)
	li	t0, 28
	sw	t0, 116(t1)
	li	t0, 1
	sw	t0, 120(t1)
	li	t0, 290
	sw	t0, 124(t1)
	lw	t0, 260(sp)
	li	s0, 0
	add	t0, t1, s0
	lw	t2, 264(sp)
	mv	t2, t0
	lw	t0, 268(sp)
	addi	t3, sp, 128
	li	s0, 0
	add	t0, t3, s0
	lw	t4, 272(sp)
	mv	t4, t0
	mv	a0, t2
	mv	a1, t4
	call	arrCopy
	lw	t0, 256(sp)
	mv	t0, a0
	lw	t1, 280(sp)
	addi	t2, sp, 128
	li	s0, 0
	add	t1, t2, s0
	lw	t3, 284(sp)
	mv	t3, t1
	mv	a0, t3
	sw	t0, 256(sp)
	call	revert
	lw	t0, 256(sp)
	mv	t0, a0
	lw	t1, 292(sp)
	li	t1, 0
	sw	t0, 256(sp)
	sw	t1, 292(sp)
.l47:
	lw	t0, 296(sp)
	lw	t1, 292(sp)
	li	s0, 32
	slt	t0, t1, s0
	sw	t0, 296(sp)
	beq	t0, x0, .l48
	lw	t0, 300(sp)
	lw	t1, 292(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 304(sp)
	addi	t4, sp, 128
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 256(sp)
	mv	t6, t3
	lw	s1, 308(sp)
	mv	s1, t6
	mv	a0, s1
	sw	t6, 256(sp)
	call	putint
	lw	t0, 312(sp)
	lw	t1, 292(sp)
	li	s0, 1
	add	t0, t1, s0
	mv	t1, t0
	sw	t1, 292(sp)
	j	.l47
.l48:
	lw	t0, 316(sp)
	addi	t1, sp, 128
	li	s0, 0
	add	t0, t1, s0
	lw	t2, 320(sp)
	mv	t2, t0
	mv	a0, t2
	call	bubblesort
	lw	t0, 256(sp)
	mv	t0, a0
	lw	t1, 292(sp)
	li	t1, 0
	sw	t0, 256(sp)
	sw	t1, 292(sp)
.l49:
	lw	t0, 328(sp)
	lw	t1, 292(sp)
	li	s0, 32
	slt	t0, t1, s0
	sw	t0, 328(sp)
	beq	t0, x0, .l50
	lw	t0, 332(sp)
	lw	t1, 292(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 336(sp)
	addi	t4, sp, 128
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 256(sp)
	mv	t6, t3
	lw	s1, 340(sp)
	mv	s1, t6
	mv	a0, s1
	sw	t6, 256(sp)
	call	putint
	lw	t0, 344(sp)
	lw	t1, 292(sp)
	li	s0, 1
	add	t0, t1, s0
	mv	t1, t0
	sw	t1, 292(sp)
	j	.l49
.l50:
	lw	t0, 348(sp)
	addi	t1, sp, 128
	li	s0, 0
	add	t0, t1, s0
	lw	t2, 352(sp)
	mv	t2, t0
	mv	a0, t2
	call	getMid
	lw	t0, 256(sp)
	mv	t0, a0
	lw	t1, 360(sp)
	mv	t1, t0
	mv	a0, t1
	sw	t0, 256(sp)
	call	putint
	lw	t0, 364(sp)
	addi	t1, sp, 128
	li	s0, 0
	add	t0, t1, s0
	lw	t2, 368(sp)
	mv	t2, t0
	mv	a0, t2
	call	getMost
	lw	t0, 256(sp)
	mv	t0, a0
	lw	t1, 376(sp)
	mv	t1, t0
	mv	a0, t1
	sw	t0, 256(sp)
	call	putint
	lw	t0, 380(sp)
	addi	t1, sp, 0
	li	s0, 0
	add	t0, t1, s0
	lw	t2, 384(sp)
	mv	t2, t0
	lw	t3, 388(sp)
	addi	t4, sp, 128
	li	s0, 0
	add	t3, t4, s0
	lw	t5, 392(sp)
	mv	t5, t3
	mv	a0, t2
	mv	a1, t5
	call	arrCopy
	lw	t0, 256(sp)
	mv	t0, a0
	lw	t1, 400(sp)
	addi	t2, sp, 128
	li	s0, 0
	add	t1, t2, s0
	lw	t3, 404(sp)
	mv	t3, t1
	mv	a0, t3
	sw	t0, 256(sp)
	call	bubblesort
	lw	t0, 256(sp)
	mv	t0, a0
	lw	t1, 292(sp)
	li	t1, 0
	sw	t0, 256(sp)
	sw	t1, 292(sp)
.l51:
	lw	t0, 412(sp)
	lw	t1, 292(sp)
	li	s0, 32
	slt	t0, t1, s0
	sw	t0, 412(sp)
	beq	t0, x0, .l52
	lw	t0, 416(sp)
	lw	t1, 292(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 420(sp)
	addi	t4, sp, 128
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 256(sp)
	mv	t6, t3
	lw	s1, 424(sp)
	mv	s1, t6
	mv	a0, s1
	sw	t6, 256(sp)
	call	putint
	lw	t0, 428(sp)
	lw	t1, 292(sp)
	li	s0, 1
	add	t0, t1, s0
	mv	t1, t0
	sw	t1, 292(sp)
	j	.l51
.l52:
	lw	t0, 432(sp)
	addi	t1, sp, 0
	li	s0, 0
	add	t0, t1, s0
	lw	t2, 436(sp)
	mv	t2, t0
	lw	t3, 440(sp)
	addi	t4, sp, 128
	li	s0, 0
	add	t3, t4, s0
	lw	t5, 444(sp)
	mv	t5, t3
	mv	a0, t2
	mv	a1, t5
	call	arrCopy
	lw	t0, 256(sp)
	mv	t0, a0
	lw	t1, 452(sp)
	addi	t2, sp, 128
	li	s0, 0
	add	t1, t2, s0
	lw	t3, 456(sp)
	mv	t3, t1
	mv	a0, t3
	sw	t0, 256(sp)
	call	insertsort
	lw	t0, 256(sp)
	mv	t0, a0
	lw	t1, 292(sp)
	li	t1, 0
	sw	t0, 256(sp)
	sw	t1, 292(sp)
.l53:
	lw	t0, 464(sp)
	lw	t1, 292(sp)
	li	s0, 32
	slt	t0, t1, s0
	sw	t0, 464(sp)
	beq	t0, x0, .l54
	lw	t0, 468(sp)
	lw	t1, 292(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 472(sp)
	addi	t4, sp, 128
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 256(sp)
	mv	t6, t3
	lw	s1, 476(sp)
	mv	s1, t6
	mv	a0, s1
	sw	t6, 256(sp)
	call	putint
	lw	t0, 480(sp)
	lw	t1, 292(sp)
	li	s0, 1
	add	t0, t1, s0
	mv	t1, t0
	sw	t1, 292(sp)
	j	.l53
.l54:
	lw	t0, 484(sp)
	addi	t1, sp, 0
	li	s0, 0
	add	t0, t1, s0
	lw	t2, 488(sp)
	mv	t2, t0
	lw	t3, 492(sp)
	addi	t4, sp, 128
	li	s0, 0
	add	t3, t4, s0
	lw	t5, 496(sp)
	mv	t5, t3
	mv	a0, t2
	mv	a1, t5
	call	arrCopy
	lw	t0, 256(sp)
	mv	t0, a0
	lw	t1, 292(sp)
	li	t1, 0
	li	t0, 31
	lw	t2, 504(sp)
	addi	t3, sp, 128
	li	s0, 0
	add	t2, t3, s0
	lw	t4, 508(sp)
	mv	t4, t2
	lw	t5, 512(sp)
	mv	t5, t1
	lw	t6, 516(sp)
	mv	t6, t0
	mv	a0, t4
	mv	a1, t5
	mv	a2, t6
	sw	t0, 256(sp)
	sw	t1, 292(sp)
	call	QuickSort
	lw	t0, 256(sp)
	mv	t0, a0
	sw	t0, 256(sp)
	sw	a0, 520(sp)
.l55:
	lw	t0, 524(sp)
	lw	t1, 292(sp)
	li	s0, 32
	slt	t0, t1, s0
	sw	t0, 524(sp)
	beq	t0, x0, .l56
	lw	t0, 528(sp)
	lw	t1, 292(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 532(sp)
	addi	t4, sp, 128
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 256(sp)
	mv	t6, t3
	lw	s1, 536(sp)
	mv	s1, t6
	mv	a0, s1
	sw	t6, 256(sp)
	call	putint
	lw	t0, 540(sp)
	lw	t1, 292(sp)
	li	s0, 1
	add	t0, t1, s0
	mv	t1, t0
	sw	t1, 292(sp)
	j	.l55
.l56:
	lw	t0, 544(sp)
	addi	t1, sp, 0
	li	s0, 0
	add	t0, t1, s0
	lw	t2, 548(sp)
	mv	t2, t0
	lw	t3, 552(sp)
	addi	t4, sp, 128
	li	s0, 0
	add	t3, t4, s0
	lw	t5, 556(sp)
	mv	t5, t3
	mv	a0, t2
	mv	a1, t5
	call	arrCopy
	lw	t0, 256(sp)
	mv	t0, a0
	lw	t1, 564(sp)
	addi	t2, sp, 128
	li	s0, 0
	add	t1, t2, s0
	lw	t3, 568(sp)
	mv	t3, t1
	mv	a0, t3
	li	a1, 4
	sw	t0, 256(sp)
	call	calSum
	lw	t0, 256(sp)
	mv	t0, a0
	lw	t1, 292(sp)
	li	t1, 0
	sw	t0, 256(sp)
	sw	t1, 292(sp)
.l57:
	lw	t0, 576(sp)
	lw	t1, 292(sp)
	li	s0, 32
	slt	t0, t1, s0
	sw	t0, 576(sp)
	beq	t0, x0, .l58
	lw	t0, 580(sp)
	lw	t1, 292(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 584(sp)
	addi	t4, sp, 128
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 256(sp)
	mv	t6, t3
	lw	s1, 588(sp)
	mv	s1, t6
	mv	a0, s1
	sw	t6, 256(sp)
	call	putint
	lw	t0, 592(sp)
	lw	t1, 292(sp)
	li	s0, 1
	add	t0, t1, s0
	mv	t1, t0
	sw	t1, 292(sp)
	j	.l57
.l58:
	lw	t0, 596(sp)
	addi	t1, sp, 0
	li	s0, 0
	add	t0, t1, s0
	lw	t2, 600(sp)
	mv	t2, t0
	lw	t3, 604(sp)
	addi	t4, sp, 128
	li	s0, 0
	add	t3, t4, s0
	lw	t5, 608(sp)
	mv	t5, t3
	mv	a0, t2
	mv	a1, t5
	call	arrCopy
	lw	t0, 256(sp)
	mv	t0, a0
	lw	t1, 616(sp)
	addi	t2, sp, 128
	li	s0, 0
	add	t1, t2, s0
	lw	t3, 620(sp)
	mv	t3, t1
	mv	a0, t3
	li	a1, 3
	sw	t0, 256(sp)
	call	avgPooling
	lw	t0, 256(sp)
	mv	t0, a0
	lw	t1, 292(sp)
	li	t1, 0
	sw	t0, 256(sp)
	sw	t1, 292(sp)
.l59:
	lw	t0, 628(sp)
	lw	t1, 292(sp)
	li	s0, 32
	slt	t0, t1, s0
	sw	t0, 628(sp)
	beq	t0, x0, .l60
	lw	t0, 632(sp)
	lw	t1, 292(sp)
	li	t2, 4
	mul	t0, t1, t2
	lw	t3, 636(sp)
	addi	t4, sp, 128
	add	t5, t4, t0
	lw	t3, 0(t5)
	lw	t6, 256(sp)
	mv	t6, t3
	lw	s1, 640(sp)
	mv	s1, t6
	mv	a0, s1
	call	putint
	lw	t0, 644(sp)
	lw	t1, 292(sp)
	li	s0, 1
	add	t0, t1, s0
	mv	t1, t0
	sw	t1, 292(sp)
	j	.l59
.l60:
	li	a0, 0
	lw	ra, 652(sp)
	addi	sp, sp, 656
	ret
	.size	main, .-main
