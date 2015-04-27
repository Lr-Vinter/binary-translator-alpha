
int x_86_Execute::Ex_Add()
{
	Jit_platform* Jit = &x86_code[pointer_platform];
	
	Jit->emit_Mov_MR (eax , SHIFT_EAX);
    Jit->emit_Mov_MR (ebx , SHIFT_EBX);
	
    Jit->emit_Pop     (eax);
    Jit->emit_Pop     (ebx);
    
    Jit->emit_Add     (eax , ebx);
    Jit->emit_Push_Ar (eax);
    
    Jit->emit_Mov_RM (eax , SHIFT_EAX);
    Jit->emit_Mov_RM (ebx , SHIFT_EBX);
    
    return 1;
}

int x_86_Execute::Ex_Div()
{
	Jit_platform* Jit = &x86_code[pointer_platform];
	
	Jit->emit_Mov_MR (eax , SHIFT_EAX);
    Jit->emit_Mov_MR (ebx , SHIFT_EBX);
    Jit->emit_Mov_MR (edx , SHIFT_EDX);
	
    Jit->emit_Mov_RN  (edx, 0);
    Jit->emit_Pop     (eax);
    Jit->emit_Pop     (ebx);
    
    Jit->emit_Div     (ebx);  
    Jit->emit_Push_Ar (eax);
    
    Jit->emit_Mov_RM (eax , SHIFT_EAX);
    Jit->emit_Mov_RM (ebx , SHIFT_EBX);
    Jit->emit_Mov_RM (edx , SHIFT_EDX);
    
    return 1;
    
}

int x_86_Execute::Ex_Mul()
{    
	Jit_platform* Jit = &x86_code[pointer_platform];
	
	Jit->emit_Mov_MR (eax , SHIFT_EAX);
    Jit->emit_Mov_MR (ebx , SHIFT_EBX);
	
    Jit->emit_Pop     (eax);
    Jit->emit_Pop     (ebx);
    
    Jit->emit_IMul    (eax , ebx); 
    Jit->emit_Push_Ar (eax);
    
    Jit->emit_Mov_RM (eax , SHIFT_EAX);
    Jit->emit_Mov_RM (ebx , SHIFT_EBX);
    
    return 1;
}

int x_86_Execute::Ex_Sub()
{
	Jit_platform* Jit = &x86_code[pointer_platform];
	
	Jit->emit_Mov_MR (eax , SHIFT_EAX);
    Jit->emit_Mov_MR (ebx , SHIFT_EBX);
	
    Jit->emit_Pop     (eax);
    Jit->emit_Pop     (ebx);
    
    Jit->emit_Sub     (eax , ebx);
    Jit->emit_Push_Ar (eax);
    
    Jit->emit_Mov_RM (eax , SHIFT_EAX);
    Jit->emit_Mov_RM (ebx , SHIFT_EBX);
    
    return 1;
}
