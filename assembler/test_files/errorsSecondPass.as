prn GGG
mov #-20, val
cmp r3   , r4
.string "random"
mcr mac1
	sub r5, JERUS
	rts
endmcr
label2: .data 4,6,-19,78
label1: add r4,YUV
lea val , r6
mac1
.entry JERUS
clr MAIN
not r8
mcr mac2
	sub r5, JERUS
	rts
endmcr
.entry nonExistentEntryLabel
inc r15
.entry GGG
dec val
MAIN: jmp YUV
NY: .string "abagada"
miami: bne val[r15]
red r9
.entry val
clr MAIN
jsr	miami
dec GGG
mac1
YUV: .string "danzie"
bne val
val: rts
prn nonExistentLabel
mac2
JERUS:  add #67       ,r1
stop
red r4
.extern GGG