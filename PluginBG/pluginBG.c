#include <string.h>
#include <stdio.h>
#include "../Globals/cplugin_api.h"

#define LANGUAGE_NAME "BG"
#define BEER_BG "тест 2"

errors_t init(){
	return EOK;
}

errors_t get_languagename(char * const result)
{
	//char language_name[] = "BG";
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
		strncpy(out_word, BEER_BG, strlen(BEER_BG));
	}
	else
	{
		return ETRANSLATE_ERR;
	}

	return EOK;
}