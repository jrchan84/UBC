.pos 0x100
    ld $0x0, r0           #r0 = 0
    ld $a, r1             #r1 = address of a
    ld $tos, r2           #r2 = address of tos
    ld $tmp, r3           #r3 = address of tmp
    ld $s, r4             #r4 = address of s
    st r0, (r2)           #tos = 0;
    st r0, (r3)           #tmp = 0;
    ld (r2), r5           #r5 = value of tos
    ld (r1, r0, 4), r6    #r6 = a[0]
    st r6, (r4, r5, 4)    #s[tos] = a[0]

    ld (r2), r7           #tos++ (1)
    inc r7                #tos++ (2)
    st r7, (r2)           #tos++ (3)

    ld $0x1, r0           #r0 = 1
    ld (r2), r5           #r5 = value of tos
    ld (r1, r0, 4), r6    #r6 = a[1]
    st r6, (r4, r5, 4)    #s[tos] = a[1]

    ld (r2), r7           #tos++ (1)
    inc r7                #tos++ (2)
    st r7, (r2)           #tos++ (3)

    ld $0x2, r0           #r0 = 2
    ld (r2), r5           #r5 = value of tos
    ld (r1, r0, 4), r6    #r6 = a[2]
    st r6, (r4, r5, 4)    #s[tos] = a[2]

    ld (r2), r7           #tos++ (1)
    inc r7                #tos++ (2)
    st r7, (r2)           #tos++ (3)

    ld (r2), r7           #tos-- (1)
    dec r7                #tos-- (2)
    st r7, (r2)           #tos-- (3)

    ld (r2), r5           #r5 = value of tos
    ld (r4, r5, 4), r6    #r6 = value of s[tos]
    st r6, (r3)           #tmp = s[tos]

    ld (r2), r7           #tos-- (1)
    dec r7                #tos-- (2)
    st r7, (r2)           #tos-- (3)

    ld (r2), r5           #r5 = value of tos
    ld (r4, r5, 4), r6    #r6 = value of s[tos]
    ld (r3), r7           #r7 = value of tmp
    add r6, r7            #r7 = r7 + s[tos]
    st r7, (r3)           #tmp = tmp + s[tos]

    ld (r2), r7           #tos-- (1)
    dec r7                #tos-- (2)
    st r7, (r2)           #tos-- (3)

    ld (r2), r5           #r5 = value of tos
    ld (r4, r5, 4), r6    #r6 = value of s[tos]
    ld (r3), r7           #r7 = value of tmp
    add r6, r7            #r7 = r7 + s[tos]
    st r7, (r3)           #tmp = tmp + s[tos]
    halt                  #halt


.pos 0x1000
a:      .long 0 #a[0]
        .long 0 #a[1]
        .long 0 #a[2]

.pos 0x2000
s:      .long 0 #s[0]
        .long 0 #s[1]
        .long 0 #s[2]
        .long 0 #s[3]
        .long 0 #s[4]

.pos 0x3000
tos:    .long 0 #tos

.pos 0x4000
tmp:    .long 0 #tmp
