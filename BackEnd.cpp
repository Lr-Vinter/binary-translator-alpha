
#include  "FrontEnd.h"
#include  "Jit_platform.h"

const int ax_PV             = 41;
const int bx_PV             = 42;
const int cx_PV             = 43;
const int dx_PV             = 44;

const int x86_COMMAND_SIZE  = 100;
const int MAX_JUMP          = 100;

const int first_pass        = 1;
const int second_pass       = 0;
const int SHIFT_EAX         = -100;
const int SHIFT_EBX         = -104;
const int SHIFT_EDX         = -108;
const int SHIFT_ECX         = -112;

struct info
{
    int where_j    = 0; 
    
    int jump_to    = 0;
};

class x_86_Execute
{   
        char* x86_char;
        
        int jmp_pointer      = 0;
        int pointer_platform = 0;
        int pointer_JMP      = 0;
        
        reg eax = 1;
        reg ebx = 2;
        reg ecx = 3;
        reg edx = 4;
        reg ebp = 5;
        reg esp = 6;
        
    public:
        
        x_86_Execute() ;
        ~x_86_Execute();
        
        int make_x86com(int num);
        int create_pasm();
        int Dump       ();
        int Execute    ();
        
        std::vector<int>  x86_bytes;
        std::vector<info> jump_points;
        
        std::vector<box>  Pasm_Vector;
        Jit_platform x86_code[x86_COMMAND_SIZE] = {};
        
        //std::vector<Jit_platform> x86_code;
        
    private:        
        list Pasm_Code ;
        
        int Ex_Push  ();
        int Ex_Pushar();
        int Ex_Pop   ();
        int Ex_Popar ();
        
        
        int Ex_Compare (int type);
        
        int Ex_More    ();
        int Ex_Less    ();
        int Ex_Equal   ();
        
        int Ex_Ret   ();
        int Ex_Call  ();
        
        int Ex_Sub   ();
        int Ex_Add   ();
        int Ex_Mul   ();
        int Ex_Div   ();
        int Ex_Jmps  ();
        
        int Ex_Start_Frame ();
        int Ex_End_Frame   ();
        int Ex_Get         ();
        int Ex_Load        ();
        //
        int Ex_In          ();
        int Ex_Out         ();
        //
        //
        int find_label     (int position);
        int fill_labels    ();
};

x_86_Execute::x_86_Execute()
{
    x86_char = (char*) calloc(x86_COMMAND_SIZE , sizeof(char) );
    //x86_code.reserve(100);
}
x_86_Execute::~x_86_Execute()
{
    
}

int x_86_Execute::create_pasm()
{
    Pasm_Vector = Pasm_Code.create("pasm.bin");
    
    for(int i = 0; i < Pasm_Vector.size(); i++)
    {   
        char instruction[MAX_SCOM] = {};
        if(0);
        #define VINTER(NAME , NUMBER , TYPE) else if(Pasm_Vector[i].instruction == NUMBER)  strcpy(instruction , #NAME);
        #include "commands.h"
        #undef  VINTER
        
        printf("LIST %d\n" , i);
        printf("-----------------------------------------\n");
        printf("instruction: %d  name: %s \n" , Pasm_Vector[i].instruction , instruction);
        printf("type       : %d  \n" , Pasm_Vector[i].type        );
        printf("number_arg : %d  \n" , Pasm_Vector[i].number_arg  );
        printf("value      : %d  \n" , Pasm_Vector[i].value       );
        printf("-----------------------------------------\n");
    }
    
    
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

enum CMD
{
    #define VINTER(name , number , type) name = number,
    #include "commands.h"
    #undef  VINTER
    finish
};

int x_86_Execute::make_x86com(int num)
{
    if(num) create_pasm();
    
    for(pointer_platform = 0; pointer_platform < Pasm_Vector.size(); pointer_platform++)
    {
        int instruction = Pasm_Vector[pointer_platform].instruction;
        switch(instruction)
        {   
            case call        :       Ex_Call() ;             break;
            case ret         :       Ex_Ret()  ;             break;
            case push        :       Ex_Push() ;             break;
            case pop         :       Ex_Pop()  ;             break;
            case popar       :       Ex_Popar();             break;
            case more        :       Ex_More();              break;
            case less        :       Ex_Less();              break;                
            case equal       :       Ex_Equal();             break;
            case sub         :       Ex_Sub();               break;
            case mul         :       Ex_Mul();               break;
            case add         :       Ex_Add();               break;
            case divi        :       Ex_Div();               break;
            case pushar      :       Ex_Pushar();            break;
            case get         :       Ex_Get();               break;
            case load        :       Ex_Load();              break;
            case start_frame :       Ex_Start_Frame();       break;
            case end_frame   :       Ex_End_Frame();         break;
            case jae         :       Ex_Jmps();              break;
            case jbe         :       Ex_Jmps();              break;
            case ja          :       Ex_Jmps();              break;
            case jb          :       Ex_Jmps();              break;
            case jmp         :       Ex_Jmps();              break;
            case je          :       Ex_Jmps();              break;
                
            //--------------------------------
            case in          :       Ex_In   ();              break;
            case out         :       Ex_Out  ();              break;
            
            case exit_asm :  break;
                
            default:
                
                printf("ERROR : %d\n" , instruction);
                assert(!"There are no variants");
                break;
        }
    }    
    
    fill_labels();
    
    if(num == 1)
    {
        jmp_pointer       = 0;
        for(int i = 0; i < pointer_platform; i++)
        {
            x86_code[i].byte_size = 0;
        }   
        pointer_platform  = 0;
    }
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

#include "MathOpers.h"

#include "CallLib.h"

#include "PasmCompare.h"

#include "PasmJumps.h"

#include "PasmStackOper.h"

//---------------------------------------------------------------------------------------------------------------------------------------------------------

const int JMPF_SHIFT  = 16;
const int USUAL_JMP   =  12;
const int CALLF_SHIFT =  2;

const int IS_JMP_TYPE =  3;
const int IS_JMP      = 10;
const int IS_CALL     = 20;

int x_86_Execute::fill_labels()
{
    int sum = 0;
    for(int i = 0; i < pointer_platform; i++)
    {
        printf("number %d sum %d \n\n" , i , sum);
        
        x86_bytes.push_back(sum);
        int size = x86_code[i].byte_size;
        
        sum = sum + size;
    }
    
    for(int i = 0; i < pointer_platform; i++)
    {
        if(Pasm_Vector[i].type == IS_JMP_TYPE)
        {
            info other = {};
            
            other.where_j  = x86_bytes[i] + JMPF_SHIFT;
            
            if(Pasm_Vector[i].instruction == IS_CALL)
            {
                other.where_j  = x86_bytes[i] + CALLF_SHIFT;
            }
            
            if(Pasm_Vector[i].instruction == IS_JMP)
            {
                other.where_j  = x86_bytes[i] + USUAL_JMP;
            }
            
            printf("value %d\n\n" , Pasm_Vector[i].value);
            printf("bytes %d\n\n" , x86_bytes[i]        );
            
            other.jump_to  = Pasm_Vector[i].value;
            
            jump_points.push_back(other);
        }
    }
    
    for(int i = 0; i < jump_points.size(); i++)
    {
        printf("bytes in  place to go %d \n" , x86_bytes[jump_points[i].jump_to]);
        printf("bytes in place from  %d \n" ,  (jump_points[i].where_j + 2)     );
        
        printf("NUMBER %d SHIFT %d \n" , i , x86_bytes[jump_points[i].jump_to] - (jump_points[i].where_j + 2));
    }
    
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

int x_86_Execute::Dump()
{
    FILE* objectFile = fopen("asm.o", "wb");
    
    printf("-------------------------------\n");
    for(int i = 0; i < pointer_platform; i++)
    {
        for(int j = 0; j < x86_code[i].byte_size; j++)
        {
            printf("%02hhX ", x86_code[i].command[j]);
            fputc(x86_code[i].command[j], objectFile);
        }
        printf("\n\n");
        
    }
    printf("-------------------------------\n");
    
    fclose(objectFile);
    
    int errorCode = system("objdump -D -b binary -mi386 asm.o -M intel");
    assert(errorCode == 0);
    
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

#include <sys/mman.h>

int x_86_Execute::Execute()
{
    char* buffer = (char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
    int bufferPos = 0;
    
    for(int i = 0; i < pointer_platform; i++)
        for(int j = 0; j < x86_code[i].byte_size; j++)
            buffer[bufferPos++] = x86_code[i].command[j];

    typedef int (*JitFunctionType)(int);
    JitFunctionType jitFunction = (JitFunctionType)buffer;
    
    for (int argument = 1; argument <= 8; argument++)
    {
        int result = jitFunction(argument);
        
        printf("jit function executed and returned %d\n", result);
    }
    
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
int 


int x_86_Execute::Ex_In()
{
    
    
    return 1;
}

int x_86_Execute::Ex_Out()
{
    return 1;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------


int main()
{
    x_86_Execute Jit;
    Jit.make_x86com(first_pass );
    Jit.make_x86com(second_pass);
    Jit.Dump       ();
    Jit.Execute    ();
    
    return 0;
}



