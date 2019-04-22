#include <string.h>
#include <stdio.h>

#include "../Globals/cplugin_api.h"

#define LANGUAGE_NAME "Spanish"

#define BEER_SPANISH "cerveza"

errors_t init(){
	return EOK;
}

errors_t get_languagename(char * const result)
{
	//char language_name[] = "German";
	strncpy(result, LANGUAGE_NAME, strlen(LANGUAGE_NAME));
	printf("%s\n", result);
	
	return EOK;
}

errors_t translate_word(char const * const in_word, char * const out_word)
{
	if (NULL == in_word)
	{
		return EWRONG_PARAMETER;
	}
	
	if ((0 == strcmp(in_word, "beer")) || (0 == strcmp(in_word, "Beer"))
		|| (0 == strcmp(in_word, "BEER")))
	{
		strncpy(out_word, BEER_SPANISH, strlen(BEER_SPANISH));
	}
	else
	{
		return ETRANSLATE_ERR;
	}

	return EOK;
}

