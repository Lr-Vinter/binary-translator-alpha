
const int MOREQ   = 1;
const int LESSEQ  = 2;
const int MORE    = 3;
const int LESS    = 4;
const int EQUAL   = 5;


int x_86_Execute::Ex_Compare(int type)
{
    printf("228 322 emit compare\n");
    Jit_platform* Jit =  &x86_code[pointer_platform];
    
    Jit->emit_Mov_MR (eax , SHIFT_EAX);
    Jit->emit_Mov_MR (ebx , SHIFT_EBX);
    
    Jit->emit_Pop (eax);
    Jit->emit_Pop (ebx);
    
    Jit->emit_Push_Ar  (ebx);
    Jit->emit_Push_Ar  (eax);
    
    Jit->emit_CMP      (eax , ebx);
    Jit->emit_eax_null ();
    
    if       (type == MORE ) Jit->emit_setg_al();
    else if  (type == LESS ) Jit->emit_setl_al();
    else if  (type == EQUAL) Jit->emit_sete_al();
    else
        assert(!"Wrong type of compare");
    
    Jit->emit_Push_Ar  (eax);
    
    Jit->emit_Mov_RM (eax , SHIFT_EAX);
    Jit->emit_Mov_RM (ebx , SHIFT_EBX);
    
    return 1;
}

int x_86_Execute::Ex_Equal()
{
    return Ex_Compare (EQUAL);
}

int x_86_Execute::Ex_Less()
{
    return Ex_Compare (LESS);
}

int x_86_Execute::Ex_More()
{
    return Ex_Compare (MORE);
}




