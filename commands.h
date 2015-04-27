
//
// VINTER (NAME , NUMBER , TYPE )
//
//
//  TYPE = 3  jump type , arg is position to jump.
//  TYPE = 4  no arg
//  TYPE = 1  arg is number
//  TYPE = 2  arg is variable
//

VINTER(push  , 1 , 1 )
VINTER(pushar, 4 , 2 )
VINTER(pop   , 2 , 4 )
VINTER(popar , 3 , 2 )

VINTER(start_frame , 31 , 1)
VINTER(get         , 32 , 1)
VINTER(load        , 34 , 1)
VINTER(end_frame   , 33 , 4)
       

VINTER(mul  , 5  , 4)
VINTER(add  , 6  , 4)
VINTER(sub  , 7  , 4)
VINTER(divi , 8  , 4)
       
VINTER(jmp , 10 , 3 )
VINTER(jne , 11 , 3 )
VINTER(jbe , 12 , 3 )
VINTER(jb  , 16 , 3 )
VINTER(jae , 13 , 3 ) 
VINTER(je  , 14 , 3 )
VINTER(ja  , 15 , 3 )

VINTER(call , 20 , 3 )
VINTER(ret  , 25 , 4 )

VINTER(more   , 26 , 4)
VINTER(less   , 27 , 4)
VINTER(equal  , 28 , 4)

VINTER(in     , 77 , 4)
VINTER(out    , 78 , 4)

VINTER(exit_asm   , 30 , 4)


