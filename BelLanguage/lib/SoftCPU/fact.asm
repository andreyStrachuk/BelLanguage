in
push 1
pop ax
call :fact
out
hlt
:fact
dec
pop cx
push cx
push cx
push 1
jb :fact
push cx
inc
pop cx
push cx
push ax
mul
pop ax
push ax
ret
