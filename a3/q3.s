.pos 0x100
    ld $0x3, r0          #r0 = 3
    ld $a, r1            #r1 = address of a
    ld $p, r2            #r2 = address of p
    ld $b, r3            #r3 = address of b

    st r0, (r1)          #a = 3
    st r1, (r2)          #p = &a
    ld (r2), r4          #r4 = p
    ld (r4), r5          #r5 = *p
    dec r5               #r4 =r4 - 1
    st r5, (r4)          #*p = *p - 1

    st r3, (r2)          #p = &b[0]
    ld (r2), r7          #r7 = &b[0]
    inca r7              #r7 = &b[0] + 1
    st r7, (r2)          #p = &[b] + 1
    ld (r1), r0          #r0 = a
    ld (r3, r0, 4), r6   #r6 = b[a]
    st r6, (r2, r0, 4)   #p[a] = b[a]
    ld $0x3, r0          #r0 = 3
    ld $0x0, r7          #r7 = 0
    ld (r3, r7, 4), r6   #r6 = b[0]
    st r6, (r2, r0, 4)   #*(p+3) = b[0]

    halt                 #halt



.pos 0x1000
a:  .long 0  #int a = 0

.pos 0x2000
p:  .long 0  #int* p = 0

.pos 0x3000
b:  .long 0  #b[0]
    .long 0  #b[1]
    .long 0  #b[2]
    .long 0  #b[3]
    .long 0  #b[4]
