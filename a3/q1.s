.pos 0x100
    ld $0x3, r0            #r0 = 3
    ld $i, r1             #r1 = address of i
    ld $a, r2             #r2 = address of a
    ld (r2, r0, 4), r3    #r3 = a[3]
    st r3, (r1)           #i = a[3]
    ld (r1), r4           #r4 = value of i
    ld (r2, r4, 4), r5    #r5 = a[i]
    st r5, (r1)           #i = a[i]
    ld $p, r6             #r6 = address of p
    ld $j, r7             #r7 = address of j
    st r7, (r6)           #p = &j
    ld $0x4, r0            #r0 = 4
    ld (r6), r1           #r1 = p
    st r0, (r1)           #*p = 4
    ld $0x2, r0            #r0 = 2
    ld (r2, r0, 4), r3    #r3 = a[2]
    shl $0x2, r3          #r3 = r3 * 4
    add r2, r3            #r3 = address of a + r3
    st r3, (r6)           #p = address of a[a[2]]
    ld (r6), r1           #r1 = p
    ld $0x4, r0            #r0 = 4
    ld (r2, r0, 4), r3    #r3 = a[4]
    ld (r1), r7           #r7 = value of r1
    add r7, r3            #r3 = p + r7
    st r3, (r1)           #*p = *p + a[4]
    halt                  #halt


.pos 0x1000
i:  .long 0  #int i = 0

.pos 0x2000
j:  .long 0  #int j = 0

.pos 0x3000
p:  .long 0  #int* p = 0

.pos 0x4000
a:  .long 0  #a[0]
    .long 0  #a[1]
    .long 0  #a[2]
    .long 0  #a[3]
    .long 0  #a[4]
    .long 0  #a[5]
    .long 0  #a[6]
    .long 0  #a[7]
    .long 0  #a[8]
    .long 0  #a[9]
    .long 0  #a[10]
