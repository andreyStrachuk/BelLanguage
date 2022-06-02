call : main
push [1+bx]
out
hlt

: main
push 1
pop bx

in
pop [bx]
push 1
pop [1+bx]
call : fact
ret


: fact
push 1
push [bx]

jne : repeat

ret

: repeat
  push [bx]
  dec
  pop [bx]
call : fact

push [bx]
inc
pop [bx]

push [bx]
push [1+bx]
mul
pop [1+bx]

ret




