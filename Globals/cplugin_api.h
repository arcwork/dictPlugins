#ifndef CPLUGIN_API_H
#define CPLUGIN_API_H

#include "globals.h"

errors_t init();
errors_t get_languagename(char * const result);
errors_t translate_word(char const * const in_word, char * const out_word);

#endif
