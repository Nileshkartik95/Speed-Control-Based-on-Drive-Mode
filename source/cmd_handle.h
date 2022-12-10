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
​ ​*​ ​@file​ ​cmd_handle.h
​ ​*​ ​@brief​ ​An​ ​abstraction​ ​for​ various command handling and command processing function for BreakfastSerial
​ ​*
​ ​*​ ​This​ ​header​ ​file​ ​provides​ interfaces and prototype for Command handing features such as author,hexdump and help .
​ ​*
​ ​*​ ​@author​ ​Nileshkartik Ashokkumar​
​ ​*​ ​@date​ ​Nov ​10​ ​2022
​ ​*​ ​@version​ ​1.0
​ ​*
​ ​*/

#ifndef CMD_HANDLE_H_
#define CMD_HANDLE_H_


/** @brief Authenticates the user to have access to the vehicle.
 *
 *	function is called by process_handler if the user command matches the command in the parameter table
 *  @param argc the number of arguments present in the user entered string, ideally separated by spaces
 *  @param argv the array of character pointers to store the different tokens present in the user string after lexical analysis
 */
void handle_user(int argc, char * argv[]);

/** @brief Check if the user details matches the command in the parameter table and calls the handler function
 *
 *	function is called by process_command after the lexical analysis and tokenization.
 *  @param argc the number of arguments present in the user entered string, ideally separated by spaces
 *  @param argv the array of character pointers to store the different tokens present in the user string after lexical analysis
 */
int process_handler(int argc, char *argv[]);


/** @brief Check if the input string is null terminated and performs the lexical analysis and identifies each token
 *
 *	function is called by main if the user presses carriage return
 *  @param input the number of arguments present in the user entered string, ideally separated by spaces
 */
int process_command(char *input);

/** @brief Performs the User Authentication
 *
 *	function is called by main to perform the user access rights
 *  @return true if the access is granted otherwise returns false
 */
int user_authetication(void);


#endif /* CMD_HANDLE_H_ */
