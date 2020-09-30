.name "zork"
.comment "I'M ALIIIIVE"

l2:		st r0, :1
		and r1, %0, r1    ;yep

live:	live %1    #test
		zjmp %1





