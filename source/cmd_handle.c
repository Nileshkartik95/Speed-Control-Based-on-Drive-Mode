/*****************************************************************************
​ ​*​ ​Copyright​ ​(C)​ ​2022 ​by​ ​Nileshkartik Ashokkumar
​ ​*
​ ​*​ ​Redistribution,​ ​modification​ ​or​ ​use​ ​of​ ​this​ ​software​ ​in​ ​source​ ​or​ ​binary
​ ​*​ ​forms​ ​is​ ​permitted​ ​as​ ​long​ ​as​ ​the​ ​files​ ​maintain​ ​this​ ​copyright.​ ​Users​ ​are
​ ​*​ ​permitted​ ​to​ ​modify​ ​this​ ​and​ ​use​ ​it​ ​to​ ​learn​ ​about​ ​the​ ​field​ ​of​ ​embedded
​ ​*​ ​software.​ ​Nileshkartik Ashokkumar​ ​and​ ​the​ ​University​ ​of​ ​Colorado​ ​are​ ​not​ ​liable​ ​for
​ ​*​ ​any​ ​misuse​ ​of​ ​this​ ​material.
​ ​*
 *****************************************************************************/
/**
​ ​*​ ​@file​ ​cmd_handle.c
​ ​*​ ​@brief​ ​An​ ​abstraction​ ​for​ various command handling and command processing function for BreakfastSerial
​ ​*
​ ​*​ ​This​ ​source​ ​file​ ​provides​ interfaces and prototype for Command handing features such as author,hexdump and help .
​ ​*
​ ​*​ ​@author​ ​Nileshkartik Ashokkumar​
​ ​*​ ​@date​ ​Nov ​10​ ​2022
​ ​*​ ​@version​ ​1.0
​ ​*
​ ​*/

#include "cmd_handle.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cb_fifo.h"

#define MAX_LENVALUE (640)			/* maximum length of the hex dump*/
#define HEX_BASE_VAL (16)			/*base value for hex*/

/*callback function prototype for handle-author, handle_dump and handle_help*/
typedef void (*command_handler_t)(int argc, char * argv[]);

/*structure with the user entered command, callback function and help contents for each command */
typedef struct
{
	const char *name_cmd;				/*command name entered by the user*/
	const char *password_cmd;			/*command name entered by the user*/
	command_handler_t handler;		/*handler call back function*/
	const char *help_string;		/*constant char * storing the address of the info string for each command*/
} command_table_t;


/*a table consisting of various user details user name and Password*/
static const command_table_t commands[] =
{
		{"Nileshkartik", "Password",handle_user,"Author Password  Performs the user authentication"},						/*command author, callback function for author and help string*/
};


static const int num_commands = sizeof(commands) / sizeof(command_table_t); 	/*declaring the number of commands present in the command table*/

/*handle function for command author*/
void handle_user(int argc, char * argv[])
{
	printf("\n\rAccess Granted !\r\n");						/*prints the access rights*/
}

int process_handler(int argc, char *argv[])
{
	for (int i=0; i < num_commands; i++)
	{
		if ((strcasecmp(argv[0], commands[i].name_cmd) == 0)&&((strcasecmp(argv[1], commands[i].password_cmd) == 0)))	/*Checks if the command entered by the user matches with the existing command in the table */
		{
			commands[i].handler(argc,argv);					/*if the command matches the command in parameter table then call the handler function */
			return 1;
		}
	}
	for(int i=0; i < argc; i++)
	{
		printf("\n\rAccess Denied %s",argv[i]);				/* Prints if the access is denied*/
		return 0;
	}
	return 0;
}

int user_authetication(void)
{
	printf("\n\rAuthentication Enter user/password or Command\n\r");
	int retval = 0;
	char str[QUEUE_MAXCAPACITY];
	int i = 0;
	char curr_char;											/*storage variable for the commands received at the terminal*/
	while((curr_char = getchar()) != '\r')
	{
		if((curr_char == '\b') )							/* if the current character is backspace then store spaces in the string input*/
		{
			if((i > 0))										/*backspace only till the valid characters are present in the buffer*/
			{
				printf("%s","\b \b");						/* removes the printed character on the terminal */
				i--;
				str[i]= ' ';
			}
		}
		else
		{
			printf("%c",curr_char);							/* prints the current character that user presses*/
			str[i]= curr_char;								/* stores the current character to the string stream*/
			i++;
		}
	}
	str[i] = '\0';											/* terminates with null when the user presses enter */
	retval = process_command(str);									/* sends the string for identifying the token*/												/* reset the iteration for next string stream*/
	memset(str,0,QUEUE_MAXCAPACITY);						/*reset the string stream of the next input of the string from the user*/
	return retval;
}

int process_command(char *input)
{
	int retval = 0;
	char *p = input;
	char *end;
	int flag = 0;

	for (end = input; *end != '\0'; end++);			// find end of string

	char *argv[10];
	int argc = 0;
	memset(argv, 0, sizeof(argv));
	for (p = input; p < end; p++)
	{
		if((*p == ' '))								/*Checks if the current character is space */
		{
			if(flag == 1)							/* flag to check if the previously entered character was valid*/
			{
				*p = '\0';							/* terminate the string with NULL for building token*/
				flag = 0;							/* reset the flag*/
			}
			continue;								/* continue in case if the current and previous is space character*/
		}
		else if(flag == 0)
		{
			argv[argc] = p;							/* stores the start address of the string for building the token */
			argc++;									/* increment the argument count*/
			flag = 1;								/* set the flag to 1*/
		}
	}
	*p = '\0';										/* null termination at the end of the token */
	if (argc == 0) 									/* return in case no arguments present */
		return 0;
	retval = process_handler(argc,argv);			/* parse the token as the arguments to the process handler */
	return retval;
}
