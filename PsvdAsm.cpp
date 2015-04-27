#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>

const int MAX_COMMAND_SIZE  = 20;
const int MAX_TAG_NUMBER    = 100;

struct label_info
{
    char name[MAX_COMMAND_SIZE] = {};
	int  number;
    int  position;
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

class labels
{
	
	int tag_number;
	
	public:
		labels();
		~labels();
		
        int get_position (char name[]);
		int add_label    (char name[] , int place_);
        int dump         ();
		
	private:
		label_info tag[MAX_TAG_NUMBER];
		
};

labels::labels():
	tag_number(0)
{ }

labels::~labels()
{ }

int labels::add_label(char name[] , int place_)
{
	tag[tag_number].number = tag_number;
    
	strcpy(tag[tag_number].name , name);
    tag[tag_number].position = place_;
	
	tag_number++;
	
	return 1;
}

int labels::get_position(char name[])
{
    for(int i = 0 ; i < tag_number; i++)
        if(strcmp(name , tag[i].name) == 0 ) return tag[i].position;
        
    return 0;   //means error
}

int labels::dump()
{
    for(int i = 0 ; i < tag_number; i++)
        printf("tag number %d name %s  position %d \n" , i , tag[i].name , tag[i].position);
    
    return 1;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

enum commands_t
{
	#define VINTER(NAME , NUMBER , TYPE) NAME = NUMBER,
    #include "commands.h"
    #undef VINTER
    FINISH
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------


class assembler
{       
    int pointer = 0;
    int cp      = 1;
    
    public:
        
        assembler ();
        ~assembler();
        int create_binary(const char* in_ , const char* out_);
        int print_tags   ();
    
    private:
		labels library;
    
        char* buf;
        
        int get_length   (FILE* in) ;
        int analis       (FILE* out);
        
        int first_tags     ();
        int second_code    ();
        
        int get_command  (char command[]);
        
        int fill_label   (char command[]);
        int check_label  (char command[]);
        
        int get_type     (char command[]);
        int get_opCode   (char command[]);
        
        // аргументы 
        int get_number   ();
        int get_arg      ();
        int get_jump     ();
        
        int space_pass   ();
};

assembler::assembler():
    pointer(0),
    cp(1)
{ }

assembler::~assembler()
{ }

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int assembler::get_length   (FILE* in)
{
    assert (in);

    fseek(in , 0 , SEEK_END);
    int length = ftell(in);
    fseek(in , 0 , SEEK_SET);
    
    return length;
}

int assembler::create_binary(const char* in_ , const char* out_)
{
    FILE*  in  = fopen(in_  , "r" );   assert(in );
    FILE* out  = fopen(out_ , "wb");   assert(out);
    
    int length = get_length(in);
    
    buf = new char[length];
    fread (buf , length , sizeof(char) , in);
    
    printf("commands \n %s" , buf);
    printf("\n--------------------\n");
    analis(out);
    
    return 1;
}

int assembler::analis(FILE* /*out*/)
{
	first_tags ();
	pointer   = 0;
    cp        = 0;
    second_code();
    
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int assembler::first_tags()
{
	for(;;)
	{
        char command[MAX_COMMAND_SIZE] = {};
		get_command(command);                       //sp point to the next command 1 2 3 
        
		if(check_label(command) ) fill_label(command);
		else 
		{	
            cp++;
			if(get_opCode(command) == exit_asm) 
            { break; }
            printf("nothing\n");
		}                    
	}
	return 1;
}

//  TYPE = 3  jump type , arg is position to jump.
//  TYPE = 4  no arg
//  TYPE = 1  arg is number
//  TYPE = 2  arg is variable
//

const int NOARG_T = 4;
const int JUMP_T  = 3;
const int VAR_T   = 2;
const int NUM_T   = 1;

int assembler::second_code()
{
    FILE* pasm = fopen("pasm.bin" , "wb");
    for(;;)
    {
        
        char command[MAX_COMMAND_SIZE] = {};
        get_command(command);
        
        int type   = get_type  (command) ;
        int opCode = get_opCode(command) ;
        
        if   (opCode        != 0) cp++ ;
        else
        { 
            check_label(command); continue;
        }
        
        printf("\n\nEBANIY OPCODE %d\n" , opCode);
        int written = fwrite(&opCode , sizeof(int) , 1 , pasm);
        printf("written %d bytes\n", written);
        
        int arg = 0;
        switch(type)
        {
            case NOARG_T:         
                
                break;
                
            case JUMP_T :
                
                arg = get_jump();
                fwrite(&arg , sizeof(int) , 1 , pasm);
                break;
            
            case NUM_T  :
                
                arg = get_number();
                fwrite(&arg , sizeof(int) , 1 , pasm);
                break;
                
                
            case VAR_T  :
                
                arg = get_arg();
                fwrite(&arg , sizeof(int) , 1 , pasm);
                break;
                
            default:
                
                printf("\nATTENTION : there are no-matching type\n");
                exit(1);
                break;
        
        }
        
        if   (opCode == exit_asm) break;
    }
    
    return 1;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int assembler::get_command(char command[])
{
	space_pass(); int step = 0;
    
	while(!isspace(buf[pointer]) )
	{
        //printf("\nsymbol %c\n" , buf[pointer] );
		command[step++] = buf[pointer++];
	}
	
	printf("command %s , step %d \n" , command , pointer);
    
	return 1;
}

int assembler::check_label(char command[])
{
	if(command[0] == ':') return 1;
	else                  return 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int assembler::get_opCode(char command[])
{
    if (0);
	#define VINTER(NAME , NUMBER , TYPE) else if(strcmp(#NAME , command) == 0) return NUMBER;
    #include "commands.h"
    #undef VINTER
    else return 0;
}

int assembler::get_type(char command[])
{
	if(0);
	#define VINTER(NAME , NUMBER , TYPE) else if(strcmp(#NAME , command) == 0)  return TYPE;
    #include "commands.h"
    #undef VINTER
    else return 0;
    
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int assembler::fill_label(char command[])
{
	assert(command[0] == ':'); 
	library.add_label(command + 1 , cp); 
	
	return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

int assembler::space_pass()
{
    while(isspace(buf[pointer]) )
        pointer++;
    
    return 1;
}


int assembler::print_tags()
{
    library.dump();
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int assembler::get_jump()
{
    char tag[MAX_COMMAND_SIZE] = {};
    get_command(tag);
    
    int    place = library.get_position(tag);
    return place - 1;
}

int assembler::get_number()
{
    space_pass();
    
    bool minus = false;
    if (buf[pointer] == '-')
    {
        minus = true;
        pointer++;
    }
    
    int number = 0;
    while(isdigit(buf[pointer]) )
    {
        number = 10*number + buf[pointer] - '0';
        pointer++;
    }
    
    if (minus)
        number = -number;
    
    return number;
}

int assembler::get_arg()
{
    space_pass();
    char reg[MAX_COMMAND_SIZE] = {}; int step = 0;
    
    while(isalpha(buf[pointer]) )
    {
        reg[step++] = buf[pointer++];
    }
    
    if       (strcmp("ax" , reg) == 0 ) return 41;
    else if  (strcmp("bx" , reg) == 0 ) return 42;
    else if  (strcmp("cx" , reg) == 0 ) return 43;
    else if  (strcmp("dx" , reg) == 0 ) return 44;
    else      
    {
        printf("was red %s\n" , reg);
        assert(!"There are no such register");
    }
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
    printf("OK\n");
    
    assembler pasm;
    pasm.create_binary("in.txt" , "out.txt");
    
    printf("----------------------\n");
    pasm.print_tags   ();
    
    /*
    int a = 7;
    FILE* ololo = fopen("pasm.bin", "wb");
    
    fwrite(&a, sizeof(int) , 1 , ololo);
    */
    
    return 0;
}

