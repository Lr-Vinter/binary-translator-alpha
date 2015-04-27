//
//  jumps translation
//
const int JUMP_SIZE_SHIFT = 2;

enum JMPS_T
{
    #define VINTER(name , code , type) name = code,
    VINTER(JMP , 10 , 3 )   
    VINTER(JNE , 11 , 3 )
    VINTER(JBE , 12 , 3 )
    VINTER(JB  , 16 , 3 )
    VINTER(JAE , 13 , 3 ) 
    VINTER(JE  , 14 , 3 )
    VINTER(JA  , 15 , 3 )
    #undef VINTER
    stop_jmp 
};

int x_86_Execute::Ex_Jmps()
{
    Jit_platform* Jit =& x86_code[pointer_platform];
    
    Jit->emit_Mov_MR (eax , SHIFT_EAX); //ATTENTION 
    Jit->emit_Mov_MR (ebx , SHIFT_EBX);
    
    int type  =  Pasm_Vector[pointer_platform].instruction ;
    
    if (type != JMP)
    {
        Jit->emit_Pop     (eax);
        Jit->emit_Pop     (ebx);
        Jit->emit_CMP     (eax , ebx);
    }
    
    //Jit->emit_Push_Ar (ebx);
    //Jit->emit_Push_Ar (eax);
    
    Jit->emit_Mov_RM (eax , SHIFT_EAX);
    Jit->emit_Mov_RM (ebx , SHIFT_EBX);
    
    int label = 0;
    if(jump_points.size() != 0)
    {   
        label = x86_bytes[jump_points[jmp_pointer].jump_to] - (jump_points[jmp_pointer].where_j + JUMP_SIZE_SHIFT);
        jmp_pointer++;
    }
    
    switch(type)
    {
        case    JA  :  Jit->emit_All_JMP(label ,  JA_T);     break;
        case    JB  :  Jit->emit_All_JMP(label ,  JB_T);     break;
        case    JAE :  Jit->emit_All_JMP(label , JAE_T);     break;
        case    JBE :  Jit->emit_All_JMP(label , JBE_T);     break;
        case    JE  :  Jit->emit_All_JMP(label ,  JE_T);     break;
        case    JNE :  Jit->emit_All_JMP(label , JNE_T);     break;
        case    JMP :  Jit->emit_All_JMP(label , JMP_T);     break;
        default:                                             break;
    };
    
    return 1;
}
