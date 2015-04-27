
int x_86_Execute::Ex_Ret()
{
    x86_code[pointer_platform].emit_Ret();
    return 1;
}

int x_86_Execute::Ex_Call()
{
    Jit_platform* Jit = &x86_code[pointer_platform];
    
    int label = 0;
    if(jump_points.size() != 0)
    {
        label = x86_bytes[jump_points[jmp_pointer].jump_to] - (jump_points[jmp_pointer].where_j + 2);
        jmp_pointer++;
    }
    
    Jit->emit_Call(label);
    
    return 1;
}
