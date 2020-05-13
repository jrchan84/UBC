.pos 0x100
code:
       ld $i, r0               #r0 = address of i
       ld $s, r1               #r1 = address of s
       ld (r0), r2             #r2 = value of i
       ld (r1, r2, 4), r3      #r3 = s.x[i]
       ld $v0, r4              #r4 = address of v0
       st r3, (r4)             #v0 = s.x[i]

       ld 0x8(r1), r0          #r0 = address of s.y[i]
       ld $v1, r4              #r4 = address of v1
       ld (r0, r2, 4), r5      #r5 = s.y[i]
       st r5, (r4)             #v1 = s.y[i]

       ld 0xC(r1), r0          #r0 = address s.z
       ld $v2, r6              #r6 = address of v2
       ld (r0, r2, 4), r7      #r7 = s.z.x[i]
       st r7, (r6)             #v2 = s.z->x[i]

       ld 0xC(r0), r1          #r2 = address of s.z.z
       ld 0x8(r1), r0          #r0 = address of s.z.z.y
       ld $v3, r6              #r4 = address of v3
       ld (r0, r2, 4), r7      #r5 = s.z.z.y[i]
       st r7, (r6)             #v3 = s.z->z->y[i]
       halt                    #halt

.pos 0x2000
static:
i:       .long 0x0     #i
v0:      .long 0x0     #v0
v1:      .long 0x0     #v1
v2:      .long 0x0     #v2
v3:      .long 0x0     #v3
s:       .long 0x1     #s.x[0]
         .long 0x2     #s.x[1]
         .long s_y     #s.y
         .long s_z     #s.z

.pos 0x3000
heap:
s_y:     .long 0x3     #s.y[0]
         .long 0x4     #s.y[1]
s_z:     .long 0x5     #s.z->x[0]
         .long 0x6     #s.z->x[1]
         .long 0x0     #s.z->y
         .long s_z_z   #s.z->z

s_z_z:   .long 0x7     #s.z.z->x[0]
         .long 0x8     #s.z.z->x[1]
         .long s_z_z_y #s.z.z.y
         .long 0x0     #s.z.z->z

s_z_z_y: .long 0x9     #s.z.z.y[0]
         .long 0x10    #s.z.z.y[1]
