.pos 0x100
        ld    $0x0, r0         # r0 = 0
        ld    $0x1, r1         # r1 = 1;
        ld    $t, r2           # r2 = address of t
        ld    $array, r3       # r3 = address of array
        ld    (r3, r0, 4), r4  # r4 = array[0]
        ld    (r3, r1, 4), r5  # r5 = array[1]
        st    r4, 0x0(r2)      # t = array[0]
        ld    0x0(r2), r6      # r6 = t;
        st    r5, (r3, r0, 4)  # array[0] = array[1]
        st    r6, (r3, r1, 4)  # array[1] = t
        halt                   # halt
.pos 0x1000
t:                   .long 0xffffffff         # t
.pos 0x2000
array:               .long 0xffffffff         # array[0]
                     .long 0xffffffff         # array[1]
