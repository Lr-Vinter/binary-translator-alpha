
typedef int reg ;

class Jit_commands
{
    
    public:
        Jit_commands();
        ~Jit_commands();
    
        reg eax = 1;
        reg ebx = 2;
        reg ecx = 3;
        reg edx = 4;
        reg ebp = 5;
        reg esp = 6;
    
        char add_c  = 0x1 ;
        char sub_c  = 0x29;
        char sub_n  = 0x83;
        char xor_c  = 0x31;
        char div_c  = 0xF7;
        char cmp_c  = 0x39;
        
        char jmp_c  = 0xEB;
        char jae_c  = 0x73;
        char jbe_c  = 0x76;
        char jne_c  = 0x75;
        char jb_c   = 0x72;
        char je_c   = 0x74;
        char ja_c   = 0x77;
        
        char call_inf  = 0xE8;
        
        char call_abs_pref = 0x66;
        char call_abs_inf  = 0xE8;
        
        char ret_c     = 0xC3;
        
        char imul_f = 0x0F;
        char imul_s = 0xAF;
    
        char push_c = 0x68; //number
        char push_a = 0x50; //register
        char pop_a  = 0x58; //register 
    
        char mov_MR_c = 0x89;
        char mov_RM_c = 0x8B;
        char mov_RR_c = 0x89;
        char mov_RN_c = 0xB8;
    
        const char eax_c = 0x0 ;
        const char ebx_c = 0x3 ;
        const char ecx_c = 0x1 ;
        const char edx_c = 0x2 ;
        const char esp_c = 0x4 ;
        const char ebp_c = 0x5 ;
    
};

Jit_commands::Jit_commands()
{ }

Jit_commands::~Jit_commands()
{ }












