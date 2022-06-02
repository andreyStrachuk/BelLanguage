push 6
pop [0+bx]

push [0+bx]

call : FUCK
push cx
out
pop dx
hlt

: FUCK

pop  ax
push ax

push [0+bx]
push 1
jbe : one

push [0+bx]

push [0+bx]
push 1
sub

pop [1+bx]

push bx
push 1
add
pop bx

call : FUCK

push bx
push 1
sub
pop bx

push cx
mul

pop cx
ret

: one
push 1
pop cx
ret