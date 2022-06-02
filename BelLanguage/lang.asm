call : main ;
hlt
: main
push 39
push bx
add
pop bx
push 0
pop [0+bx] ;

push 0
pop [1+bx] ;

push 0
pop [2+bx] ;

push 0
pop [3+bx] ;

in
pop [0+bx]
in
pop [1+bx]
in
pop [2+bx]
push [0+bx]
push 0
jne : end_if0
push [1+bx]
push 0
jne : end_if0
push [2+bx]
push 0
jne : end_if0
push 1
push -1
mul
pop [3+bx] ;

push [3+bx]
out
push [3+bx]
push 0
pop ax
push 39
push bx
sub
pop bx
ret
: end_if0
: end_if1
: end_if2
push [2+bx]
push [0+bx]
push 4
mul
mul
push [1+bx]
push [1+bx]
mul
sub
pop [4+bx] ;

push [4+bx]
push 0
jne : end_if3
push [0+bx]
push 2
push [1+bx]
push 1
mul
div
div
push -1
mul
pop [5+bx] ;

push 1
pop [3+bx] ;

push [3+bx]
out
push [3+bx]
push [5+bx]
out
push [5+bx]
push 0
pop ax
push 39
push bx
sub
pop bx
ret
: end_if3
push [4+bx]
push 0
jb : end_if4
push 0
pop [3+bx] ;

push [3+bx]
out
push [3+bx]
push 0
pop ax
push 39
push bx
sub
pop bx
ret
: end_if4
push [4+bx]
sqrt
pop ax
push ax
pop [6+bx] ;

push 2
push [6+bx]
push [1+bx]
push 1
mul
add
push -1
mul
div
pop [7+bx] ;

push 2
push [6+bx]
push [1+bx]
push 1
mul
sub
push -1
mul
div
pop [8+bx] ;

push 2
pop [3+bx] ;

push [3+bx]
out
push [3+bx]
push [7+bx]
out
push [7+bx]
push [8+bx]
out
push [8+bx]
push 39
push bx
sub
pop bx
ret ;
