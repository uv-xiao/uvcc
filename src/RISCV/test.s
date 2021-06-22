	.global	v0
	.section	.sdata
	.align	2
	.type	v0, @object
	.size	v0, 4
v0:
	.word	0
	.comm	v1, 40, 4
	.text
	.align	2
	.global	main
	.type	main, @function
main:
	addi	sp, sp, -16
	sw	ra, 12(sp)
	call	getint
	la	t0, v0
	sw	a0, 0(t0)
	lui	t0, %hi(v0)
	lw	t0, %lo(v0)(t0)
	li	t1, 10
	ble	t0, t1, .l0
	li	a0, 1
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.l0:
	li	s0, 0
	mv	s1, s0
.l1:
	lui	t0, %hi(v0)
	lw	t0, %lo(v0)(t0)
	bge	s0, t0, .l2
	call	getint
	mv	t0, a0
	li	t1, 4
	mul	t2, s0, t1
	la	t3, v1
	add	t3, t3, t2
	sw	t0, 0(t3)
	lw	t4, 0(t3)
	add	s1, s1, t4
	li	s0, 1
	add	s0, s0, s0
	j	.l1
.l2:
	mv	a0, s1
	call	putint
	li	a0, 0
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
	.size	main, .-main
