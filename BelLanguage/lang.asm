call : main_foo ;
: main
push 0
push 13
je : end_if0
push 1 ;
pop [0] ;

push 0 ;
pop [1] ;

push 0 ;
pop [0] ;

: end_if0
push 592 ;
pop [1] ;

push 10 ;
pop [0] ;

push 602 ;
pop [1] ;

ret ;
