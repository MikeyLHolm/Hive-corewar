.name "ForFunGG"
.comment "Virgin"

init:
	ld	%16777216, r3
	ld	%1879113755, r2
	st	r2, 11
	st	r3, 31
	st	r1,	255
	st	r1, 14
	and r2, %5, r2
live:
	live %1
	fork %-33

