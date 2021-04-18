           entry
           addi r14,r0,topaddr
           % processing: t1 := 1
           addi r1,r0,1
           sw t1(r0),r1
           % processing: a := t1
           lw r1,t1(r0)
           sw a(r0),r1
           % processing: t2 := 2
           addi r1,r0,2
           sw t2(r0),r1
           % processing: b := t2
           lw r1,t2(r0)
           sw b(r0),r1
           % processing: put(b)
           lw r1,b(r0)
           % put value on stack
           sw -8(r14),r1
           % link buffer to stack
           addi r1,r0, buf
           sw -12(r14),r1
           % convert int to string for output
           jl r15, intstr
           sw -8(r14),r13
           % output to console
           jl r15, putstr
           hlt

           % space for variable a
a          res 4
           % space for variable b
b          res 4
           % space for constant 1
t1          res 4
           % space for constant 2
t2          res 4
           % buffer space used for console output
buf        res 20

