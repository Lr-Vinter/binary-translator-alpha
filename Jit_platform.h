
//
// Jit platform.
//
#include "Jit_commands.h"
const int MAX_ELEM_SIZE = 100;
#define $ if(1)

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------


class Jit_platform
{
    
    public:
        
        char command[MAX_ELEM_SIZE]  = {};
        int  byte_size               = 0 ;
        
        Jit_commands CMD;

        Jit_platform();
        ~Jit_platform();
        //-------------------------------
        
        int emit_Push    (int num);
        int emit_Push_Ar (reg one);
        int emit_Pop     (reg one);
        int emit_Div     (reg one);
        int emit_IMul    (reg one , reg two);
        int emit_Sub_Num (reg one , int num);
        int emit_Sub     (reg one , reg two);
        int emit_Add     (reg one , reg two);
        int emit_Xor     (reg one , reg two);
        int emit_Mov_RR  (reg one , reg two);
        int emit_Mov_RM  (reg one , int num);
        int emit_Mov_MR  (reg one , int num);
        int emit_Mov_RN  (reg one , int num);
        int emit_CMP     (reg one , reg two);
        
        
        int emit_All_JMP (int num , int type_number);
        int emit_Ja      (int num);
        int emit_Jb      (int num);
        int emit_Jae     (int num);
        int emit_Jbe     (int num);
        int emit_Je      (int num);
        int emit_jmp     (int num);
        
        int emit_setl_al  ();
        int emit_setg_al  ();
        int emit_sete_al  ();
        int emit_eax_null ();
        
        int emit_Ret ();
        int emit_Call(int num);
        
        int emit_Mov (reg one , reg two);
        //-------------------------------
        
        char emit_Reg (reg reg_number);
        char ret_byte (int number , int byte_num);
        
        int number_reverse(int num);
        
};

Jit_platform::Jit_platform()
{ }
Jit_platform::~Jit_platform()
{ }

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int Jit_platform::emit_eax_null()
{
    command[byte_size++] = 0xB8;
    
    command[byte_size++] = 0x00;
    command[byte_size++] = 0x00;
    command[byte_size++] = 0x00;
    command[byte_size++] = 0x00;
    
    return 1;
}


int Jit_platform::emit_sete_al()
{
    command[byte_size++] = 0x0F;
    command[byte_size++] = 0x94;
    command[byte_size++] = 0xC0;
    
    return 1;
}

int Jit_platform::emit_setg_al()
{
    command[byte_size++] = 0x0F;
    command[byte_size++] = 0x9F;
    command[byte_size++] = 0xC0;
    
    return 1;
}

int Jit_platform::emit_setl_al()
{
    command[byte_size++] = 0x0F;
    command[byte_size++] = 0x9C;
    command[byte_size++] = 0xC0;
    
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int Jit_platform::emit_Call(int num)
{
    command[byte_size++] = CMD.call_pref;
    command[byte_size++] = CMD.call_inf ;
    command[byte_size++] = num & 0xFF;
    command[byte_size++] = (num >> 8) & 0xFF;
    
    return 1;
}

int Jit_platform::emit_Ret()
{
    command[byte_size++] = CMD.ret_c;
    
    return 1;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

char Jit_platform::emit_Reg(reg reg_number)
{
    if     (reg_number == CMD.eax) return CMD.eax_c;
    else if(reg_number == CMD.ebx) return CMD.ebx_c;
    else if(reg_number == CMD.ecx) return CMD.ecx_c;
    else if(reg_number == CMD.edx) return CMD.edx_c;
    else if(reg_number == CMD.esp) return CMD.esp_c;
    else if(reg_number == CMD.ebp) return CMD.ebp_c;
    else
    {
        assert(!"ERROR , wrong reg");
    }
}

int Jit_platform::emit_CMP(reg one, reg two)
{
    char first  = emit_Reg(one);
    char second = emit_Reg(two);
    
    command[byte_size++] = CMD.cmp_c;
    command[byte_size++] = (0xC0)|(second<<3)|(first);
    
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

enum jump_t
{
    JA_T   = 1,
    JE_T   = 2,
    JAE_T  = 3,
    JBE_T  = 4,
    JMP_T  = 5,
    JB_T   = 6,
    JNE_T  = 7
};


int Jit_platform::emit_All_JMP(int num , int type_number)
{   
    switch(type_number)
    {
        case JA_T :   command[byte_size++] = CMD.ja_c;          break;
        case JE_T :   command[byte_size++] = CMD.je_c;          break;
        case JB_T :   command[byte_size++] = CMD.jb_c;          break;
        case JAE_T:   command[byte_size++] = CMD.jae_c;         break;
        case JBE_T:   command[byte_size++] = CMD.jbe_c;         break;
        case JMP_T:   command[byte_size++] = CMD.jmp_c;         break;
        case JNE_T:   command[byte_size++] = CMD.jne_c;         break;         
        default   :                                             break;
    };
    
    printf("###### 228 322 GENERATED OFFSET %d\n", num);
    command[byte_size++] = num;
    
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int Jit_platform::emit_Add(reg one , reg two)
{
    char first  = emit_Reg(one);
    char second = emit_Reg(two);
    
    command[byte_size++] = CMD.add_c;
    command[byte_size++] = (0xC0)|(second<<3)|(first);
    
    $ printf(" Add\n");
    
    return 1;
}

int Jit_platform::emit_IMul(reg one , reg two)
{
    char first  = emit_Reg(one);
    char second = emit_Reg(two);
    
    command[byte_size++] = CMD.imul_f;
    command[byte_size++] = CMD.imul_s;
    
    command[byte_size++] = (0xC0)|(first<<3)|(second);
    
     $ printf("imul \n");
    
    return 1;
}

int Jit_platform::emit_Sub_Num(reg one, int num)
{
    char first = emit_Reg(one);
    
    command[byte_size++] = CMD.sub_n;
    command[byte_size++] = (0xE8)|(first);
    command[byte_size++] = num;
    
    return 1;
}


int Jit_platform::emit_Div(reg one)
{
    char first = emit_Reg(one);
    
    command[byte_size++] = CMD.div_c;
    command[byte_size++] = (0xF0)|(first);
    
    $ printf(" Div\n");
    
    return 1;
}

int Jit_platform::emit_Push(int num)
{
    command[byte_size++]           = CMD.push_c;
    *((int*)(command + byte_size)) = num;
    byte_size+=4;
    
    $ printf("push num\n");
    
    return 1;
}

int Jit_platform::emit_Push_Ar(reg one)
{   
    command[byte_size++] = (CMD.push_a)|(emit_Reg(one));
    
    $ printf("push arg\n");
    
    return 1;
}

int Jit_platform::emit_Pop   (reg one)
{
    command[byte_size++] = (CMD.pop_a)|(emit_Reg(one));
    
    $ printf("pop reg\n");
    
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int Jit_platform::emit_Mov_MR(reg one, int num) 
{
    command[byte_size++] = CMD.mov_MR_c;
    command[byte_size++] = (0x45)|(emit_Reg(one) << 3);
    command[byte_size++] = num;
    
    $ printf("mov MR\n");
    
    return 1;
}

int Jit_platform::emit_Mov_RN(reg one, int num)
{
    command[byte_size++] = (0xB8)|(emit_Reg(one));
    command[byte_size++] = num;
    
    $ printf("mov RN\n");
    
    return 1;
}

int Jit_platform::emit_Mov_RM(reg one, int num)
{
    command[byte_size++] = CMD.mov_RM_c;
    command[byte_size++] = (0x45)|(emit_Reg(one) << 3);
    command[byte_size++] = num;
    
    $ printf("mov RM\n");
    
    return 1;
}

int Jit_platform::emit_Mov_RR(reg one, reg two)
{
    command[byte_size++] = CMD.mov_RR_c;
    command[byte_size++] = (0xC0)|(emit_Reg(two) << 3)|(emit_Reg(one) << 0);
    
    $ printf("mov RR\n");
    
    return 1;
}

int Jit_platform::emit_Sub(reg one, reg two)
{
    command[byte_size++] = CMD.sub_c;
    command[byte_size++] = (0xC0)|(emit_Reg(two) << 3)|(emit_Reg(one));
    
    $ printf("SUB\n");
    
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int Jit_platform::emit_Xor(reg one, reg two)
{
    command[byte_size++] = CMD.xor_c;
    command[byte_size++] = (0xC0)|(emit_Reg(two) << 3)|(emit_Reg(one));
    
    $ printf("Xor\n");
    
    return 1;
}
// 

#undef $

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------







