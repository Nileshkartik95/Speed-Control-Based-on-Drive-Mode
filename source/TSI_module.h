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
​ ​*​ ​@file​ ​TSI_module.h
​ ​*​ ​@brief​ this header contains various API used from the touch sensor module
 *
​ ​*​ ​@author​ ​Nileshkartik Ashokkumar​
​ ​*​ ​@date​ ​09-Oct-2022
​ ​*​ ​@version​ ​1.0
​ ​*
​ ​*/

#ifndef TSI_SUBMODULE_H_
#define TSI_SUBMODULE_H_

#define VALID_TOUCH 500					/*threshold to detect the actual valid touch*/

/** @brief Performs the TSI initialization
 *
 */
void TSIInit(void);


/** @brief Function to get the current touch sensor value
 *
 */
int get_touch_val(void);
#endif /* TSI_SUBMODULE_H_ */
