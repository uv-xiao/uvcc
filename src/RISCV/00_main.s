	.text
	.align	2
	.global	main
	.type	main, @function
main:
	addi	sp, sp, -16
	sw	ra, 12(sp)
	li	a0, 0
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
	.size	main, .-main
