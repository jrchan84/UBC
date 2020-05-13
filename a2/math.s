.pos 0x100
      ld $0x0, r0       #r0 = 0
      ld $0x1, r1       #r1 = 1
      ld $0x2, r2       #r2 = 2
      ld $0x4, r3       #r3 = 4
      ld $a, r4         #r4 = address of a;
      ld $b, r5         #r5 = address of b;
      ld 0x0(r5), r6    #r6 = b
      add r1, r6        #r6 + 1
      add r3, r6        #r6 + 4
      shr $0x1, r6      #r6 / 2
      ld 0x0(r5), r7    #r7 = b
      and r6, r7        #r6 & r7
      shl $0x2, r6      #r6 << 2
      st r6, 0x0(r4)    #a = r6
      halt              #halt

.pos 0x1000
a: .long 0xffffffff         # a
.pos 0x2000
b: .long 0xffffffff         # b
