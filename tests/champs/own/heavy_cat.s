.name "Heavy_cat.exe"
.comment "Heavy and pur-ready"

	sti r1, %7, %1
	live %1337
	ld %33554431, r2
	ld %38, r3
	ld %15, r5
	ld %655354, r4
	fork %:loop
		sti r1, %7, %1
	live %1337
	ld %33554431, r2
	ld %38, r3
	ld %15, r5
	ld %655354, r4
	fork %:loop
		sti r1, %7, %1
	live %1337
	ld %33554431, r2
	ld %38, r3
	ld %15, r5
	ld %655354, r4
	fork %:loop
		sti r1, %7, %1
	live %1337
	ld %33554431, r2
	ld %38, r3
	ld %15, r5
	ld %655354, r4
	fork %:loop
		sti r1, %7, %1
	live %1337
	ld %33554431, r2
	ld %38, r3
	ld %15, r5
	ld %655354, r4
	fork %:loop
		sti r1, %7, %1
	live %1337
	ld %33554431, r2
	ld %38, r3
	ld %15, r5
	ld %655354, r4
	fork %:loop
	live %1337
	ld %33554431, r2
	ld %38, r3
	ld %15, r5
	ld %655354, r4
	fork %:loop
		sti r1, %7, %1
	live %1337
	ld %33554431, r2
	ld %38, r3
	ld %15, r5
	ld %655354, r4
	fork %:loop
		sti r1, %7, %1
	live %1337
	ld %33554431, r2
	ld %38, r3
	ld %15, r5
	ld %655354, r4
	fork %:loop
		sti r1, %7, %1
	live %1337
	ld %33554431, r2
	ld %38, r3
	ld %15, r5
	ld %655354, r4
	fork %:loop
	live %1337
	ld %33554431, r2
	ld %38, r3
	ld %15, r5
	ld %655354, r4
	fork %:loop
		sti r1, %7, %1
	live %1337
	ld %33554431, r2
	ld %38, r3
	ld %15, r5
	ld %655354, r4
	fork %:loop
		sti r1, %7, %1
	live %1337
	ld %33554431, r2
	ld %38, r3
	ld %15, r5
	ld %655354, r4
	fork %:loop
			sti r1, %7, %1
	live %1337
	ld %33554431, r2
	ld %38, r3
	ld %15, r5
	ld %655354, r4
	fork %:loop


loop:
	sti r1, r3, %0
	sti r2, r3, %-7
	sti r4, r3, %-8
	add r3, r5, r3
	ld %0, r16
	fork %:loop
