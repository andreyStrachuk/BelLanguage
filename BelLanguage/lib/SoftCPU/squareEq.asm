in
pop ax
in
pop bx
in
pop cx

push 0
push ax

je :lineara

push 0
push cx

je :linearc

call :square


hlt

:lineara

push 0
push bx

je :noroots

push bx

push -1
push cx
mul

div

push 1

out

out

hlt


ret


:linearc


push ax

push -1
push bx
mul

div

push 0

push 2

out
out
out

hlt

ret

:square

push 0

push ax
push cx
push 4
mul
mul

push bx
push bx
mul

sub

pop dx
push dx

jb :noroots

push 0
push dx

je :oneroot

push 2
push ax
mul

push -1
push bx
mul

push dx
sqrt

add

div

;

push 2
push ax
mul

push dx
sqrt

push -1
push bx
mul

sub

div

push 2
out
out
out

ret

:noroots

push 0
push cx

je :inf

push 0

out

hlt

ret


:inf

push -1
out
hlt
ret


:oneroot

push 2
push ax
mul

push -1
push bx
mul

push dx

add

div

push 1
out
out
hlt
ret

