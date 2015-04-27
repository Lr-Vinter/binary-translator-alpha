
int x_86_Execute::Ex_Pop()
{
    x86_code[pointer_platform].emit_Pop(eax);
    return 1;
}

int x_86_Execute::Ex_Popar()
{
    int arg = Pasm_Vector[pointer_platform].value;
    if      (arg == cx_PV) x86_code[pointer_platform].emit_Pop(ecx);
    else if (arg == dx_PV) x86_code[pointer_platform].emit_Pop(edx);
    else if (arg == ax_PV) x86_code[pointer_platform].emit_Pop(eax);
    else if (arg == bx_PV) x86_code[pointer_platform].emit_Pop(ebx);
    else 
    {
        assert(!"Wrong reg sorry Popar");
    }
    
    return 1;
}

int x_86_Execute::Ex_Push()
{
    int arg = Pasm_Vector[pointer_platform].value;
    x86_code[pointer_platform].emit_Push(arg);
    
    return 1;
}

int x_86_Execute::Ex_Pushar()
{
	Jit_platform* Jit = &x86_code[pointer_platform];
	
    int arg = Pasm_Vector[pointer_platform].value;
    if      (arg == cx_PV) Jit->emit_Push_Ar(ecx);
    else if (arg == dx_PV) Jit->emit_Push_Ar(edx);
    else if (arg == ax_PV) Jit->emit_Push_Ar(eax);
    else if (arg == bx_PV) Jit->emit_Push_Ar(ebx);
    else 
    {
        assert(!"Wrong reg sorry Pushar");
    }
    
    return 1;
}

int x_86_Execute::Ex_End_Frame()
{
    x86_code[pointer_platform].emit_Pop(ebp);
    return 1;
}

int x_86_Execute::Ex_Get()
{
    Jit_platform* Jit =  &x86_code[pointer_platform];
    Jit->emit_Mov_MR (eax , SHIFT_EAX);
    
    int arg = (Pasm_Vector[pointer_platform].value)*(-4);
    x86_code[pointer_platform].emit_Mov_RM (eax , arg);
    x86_code[pointer_platform].emit_Push_Ar(eax);
    
    Jit->emit_Mov_RM (eax , SHIFT_EAX);
    
    return 1;
}

int x_86_Execute::Ex_Load()
{
    Jit_platform* Jit =  &x86_code[pointer_platform];
    Jit->emit_Mov_MR (edx , SHIFT_EDX);
    
    int arg = (Pasm_Vector[pointer_platform].value)*(-4);
    x86_code[pointer_platform].emit_Mov_MR (edx , arg);
    
    Jit->emit_Mov_RM (edx , SHIFT_EDX);
    
    return 1;
}

int x_86_Execute::Ex_Start_Frame()
{
    Jit_platform* Jit =  &x86_code[pointer_platform];
	
    int arg = (Pasm_Vector[pointer_platform].value )*4;
    
    Jit->emit_Push_Ar(ebp);
    Jit->emit_Mov_RR (ebp , esp);
    Jit->emit_Sub_Num(esp , arg);
    
    return 1;
}

