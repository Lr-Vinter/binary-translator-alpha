#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <vector>

const int poison = 433;
const int exit_i =  30;

const int NO_ARG_T = 4;
const int NUMBER_T = 1;
const int VAR_T    = 2;
const int JUMP_T   = 3;

const int MAX_SCOM = 15 ;
const int MAX_SIZE = 100;

class box
{
    public:
        box();
        ~box();
        
        int instruction = 0;
        int number_arg  = 0;
        int type        = 0;
        int value       = 0;
    
};

box::box()
{ }

box::~box()
{ }

class list
{
    int pointer = 0;
    
    public:
        list();
        ~list();
        
        std::vector<box> create (const char* name);
        int make_list (FILE* in  );
        int dump      ();
        int dump_shift();
        
        std::vector<box> boxes ;
        std::vector <int> adres;
        
    private:
        int get_length(FILE* in);
        
        
        int create_box(int type, box* current); 
        
        int list_shift();
        int get_number();
        int space_pass();
        int get_type       (int num);
        int get_instruction(int num);
        
        char* buf;
        
};

list::list()
{ 
    //boxes.reserve(100);
    //adres.reserve(100);
}

list::~list()
{ }


std::vector<box> list::create(const char* name)
{   
    FILE* in  = fopen(name , "rb");
    int length  = get_length(in);
    
    buf = new char[length];
    fread(buf , length , sizeof(char) , in);
    
    make_list(in);
    list_shift();
    
    return boxes;
}



//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int list::make_list(FILE* /*in*/)
{
    int shift = 0;
    adres.push_back(0);
    
    for(;;)
    {
        int number         = get_number();
        int instruction_g  = get_instruction(number);
        int type_g         = get_type       (number);
        
        box current = {};
        current.instruction = instruction_g;
        current.type        = type_g;
        create_box(type_g , &current);
        
        int local_shift = current.number_arg + 1;
        shift = shift + local_shift;
        
        boxes.push_back(current);
        adres.push_back(shift  );
        
        if(instruction_g == exit_i) 
            break;
    }
    return 1;
}

int list::list_shift()
{
    for(int i = 0; i < boxes.size(); i++)
    {
        if(boxes[i].type == JUMP_T)
        {
            for(int j = 0; j < adres.size(); j++)
            {
                if(boxes[i].value == adres[j])
                    boxes[i].value = j;
            }
        }
    }
    
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int list::create_box(int type, box* current)
{
    int arg = 0;
    switch(type)
        {
            case NO_ARG_T :
                current->value       = poison;
                current->number_arg  = 0;
                
                break;
            case VAR_T    :
                arg = get_number();
                current->value       = arg;
                current->number_arg  = 1;
                
                break;
            case NUMBER_T :
                arg = get_number();
                current->value       = arg;
                current->number_arg  = 1;
                
                break;
            case JUMP_T   :
                arg = get_number();
                current->value       = arg;
                current->number_arg  = 1;
                
                break;
                
            default       :
                printf("\nThere are no such fucken shit\n");
                break;
        }
        
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int list::get_length (FILE* in)
{
    assert (in);

    fseek(in , 0 , SEEK_END);
    int length = ftell(in);
    fseek(in , 0 , SEEK_SET);
    
    return length;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int list ::get_number()
{
    int num = *(int*)(buf + pointer);
    pointer += sizeof(int);
    printf("read %d\n", num);
    return num;
    
    space_pass(); 
    int number = 0;
    while(isdigit(buf[pointer]) )
    {
        number = 10*number + buf[pointer] - '0';
        pointer++;
    }
    
    return number;
}

int list::space_pass()
{
    return 1;
    while(isspace(buf[pointer]) )
        pointer++;
    
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------


int list::get_instruction(int num)
{
    if(0);
    #define VINTER(NAME , NUMBER , TYPE) else if(num == NUMBER)  return NUMBER;
    #include "commands.h"
    #undef VINTER
    
    return poison;
}

int list::get_type(int num)
{
    if(0);
    #define VINTER(NAME , NUMBER , TYPE) else if(num == NUMBER)  return TYPE;
    #include "commands.h"
    #undef VINTER
    
    return poison;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int list::dump()
{
    for(int i = 0; i < boxes.size(); i++)
    {   
        char instruction[MAX_SCOM] = {};
        if(0);
        #define VINTER(NAME , NUMBER , TYPE) else if(boxes[i].instruction == NUMBER)  strcpy(instruction , #NAME);
        #include "commands.h"
        #undef VINTER
        
        printf("LIST %d\n" , i);
        printf("-----------------------------------------\n");
        printf("instruction: %d  name: %s \n" , boxes[i].instruction , instruction);
        printf("type       : %d  \n" , boxes[i].type        );
        printf("number_arg : %d  \n" , boxes[i].number_arg  );
        printf("value      : %d  \n" , boxes[i].value       );
        printf("-----------------------------------------\n");
    }
    return 1;
}

int list::dump_shift()
{
    for(int i = 0; i < adres.size(); i++)
        printf("number %d , shift %d\n" , i , adres[i]);
    
    return 1;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int instruction = 0;
int number_arg  = 0;
int type        = 0;
int value       = 0;

/*
int main()
{
    printf("ZDOROVO\n");
    
    list one;
    one.create("pasm.txt");
    //one.dump();
    
    one.dump_shift();
    one.dump      ();
    
    return 0;
    
}
*/
