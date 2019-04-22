#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <dlfcn.h>

#include "PluginManager.h"
#include "../Globals/cplugin_api.h"

#define INIT_PLUGIN_API "init"
#define LANGUAGE_GETTER_PLUGIN_API "get_languagename"
#define TRANSLATE_PLUGIN_API "translate_word"

static void * plib_handles[MAX_PLUGINS_ALLOWED] = {NULL};
static unsigned int plugin_counter = 0;

errors_t readCurrentPluginNames(char const * const _cPath, int * _pCurrent_dict_names_size, char * const result) {
	int detected_size = 0;
	int last_filename_size = 0;
	// read the content of the folder _cPath
	DIR *d = NULL;
	struct dirent *dir = NULL;
	d = opendir(_cPath);
	char * pCurrentChar = result;
	
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			// calculate the detected size
			last_filename_size = strlen(dir->d_name);
			if ((detected_size + last_filename_size) < DICT_NAME_MAX_SIZE)
			{
				if ((0 != strcmp(dir->d_name, ".")) && (0 != strcmp(dir->d_name, "..")))
				{
					pCurrentChar = result + detected_size;
					strncpy(pCurrentChar, dir->d_name, last_filename_size);
					pCurrentChar = pCurrentChar + last_filename_size;
					strcat(pCurrentChar,";");
					detected_size = detected_size + last_filename_size + 1;
					//printf("last_filename_size = %d d_name = %s result = %s\n", last_filename_size, dir->d_name, result);
				}
				else
				{
					// do nothing
				}
			}
			else
			{
				return ECHAR_OVERFLOW;
			}
		}
		closedir(d);
	}
 
	*_pCurrent_dict_names_size = detected_size;
	
	return EOK;
}

errors_t loadPlugins(char const * const _cPath, const int current_dict_size, char * plugin_name_array)
{
	errors_t result = EOK;
	const char s[2] = ";";
	
	char * token = NULL;
   
    /* get the first token */
    token = strtok(plugin_name_array, s);
    result = loadPlugin(_cPath, token);
   
    /* walk through other tokens */
    while( token != NULL ) {
		token = strtok(NULL, s);
		if (NULL != token)
		{
			result = loadPlugin(_cPath, token);
		}
    }
   
	return result;
}

errors_t loadPlugin(char const * const _cPath, char const * const plugin_name)
{
	void * plib_handle = NULL;
	errors_t (*fn_init)();
	char path[MAX_PATH_NAME_LEN] = {'\0'};
	char * pError = NULL;
	errors_t result = EOK;
	
	if (plugin_counter >= MAX_PLUGINS_ALLOWED)
	{
		result = EPLUGIN_OVERFLOW;
		return result;
	}
	
	strcat(path, _cPath);
	strcat(path, "/");
	strcat(path, plugin_name);
	//printf("\n path = %s ", path);
	
	
	plib_handle = dlopen(path, RTLD_LAZY);
	if (!plib_handle)
	{
	      fprintf(stderr, "%s\n", dlerror());
	      exit(1);
	}
	else
	{
		plib_handles[plugin_counter] = plib_handle;
		plugin_counter++;
	}
	
    fn_init = dlsym(plib_handle, INIT_PLUGIN_API);
	if ((pError = dlerror()) != NULL) 
    {
		fprintf(stderr, "%s\n", pError);
        exit(1);
    }

    result = (*fn_init)();

	//printf( " path = %s plugin_name = %s \n", _cPath, plugin_name );
	return result;
}

errors_t readLanguagesLoaded(char * currentLanguages)
{
	int detected_size = 0;
	int last_language_size = 0;
	char * pCurrentChar = currentLanguages;
	char language_name[MAX_STRING_LEN] = {'\0'};
	
	void * plib_handle = NULL;
	errors_t (*fn_get_languagename)(char * const);
	int index = 0;
	char * pError = NULL;
	errors_t result = EOK;
	
	for (index = 0; index < plugin_counter; index++)
	{
		plib_handle = plib_handles[index];
		fn_get_languagename = dlsym(plib_handle, LANGUAGE_GETTER_PLUGIN_API);
		if ((pError = dlerror()) != NULL) 
		{
			fprintf(stderr, "%s\n", pError);
			exit(1);
		}
		else
		{
			//printf("in plib_handle = 0x%x API %s found OK", (unsigned int)plib_handle, LANGUAGE_GETTER_PLUGIN_API);
		}
		
		if (EOK == fn_get_languagename(language_name))
		{
			last_language_size = strlen(language_name);
			pCurrentChar = currentLanguages + detected_size;
			strncpy(pCurrentChar, language_name, last_language_size);
			pCurrentChar = pCurrentChar + last_language_size;
			strcat(pCurrentChar,";");
			detected_size = detected_size + last_language_size + 1;
			memset(language_name, '\0', MAX_STRING_LEN);
		}
		else
		{
			result = ELANGUAGE_NAME_ERR;
		}
	}

	return result;	
}

errors_t translateWord(char const * const in_word, char * const out_word)
{
	int detected_size = 0;
	int last_word_size = 0;
	char * pCurrentChar = out_word;
	char word_translated[MAX_STRING_LEN] = {'\0'};
	
	void * plib_handle = NULL;
	errors_t (*fn_translate_word)(char const * const, char *);
	int index = 0;
	char * pError = NULL;
	errors_t result = EOK;
	
	for (index = 0; index < plugin_counter; index++)
	{
		plib_handle = plib_handles[index];
		fn_translate_word = dlsym(plib_handle, TRANSLATE_PLUGIN_API);
		if ((pError = dlerror()) != NULL) 
		{
			fprintf(stderr, "%s\n", pError);
			exit(1);
		}
		else
		{
			//printf("in plib_handle = 0x%x API %s found OK", (unsigned int)plib_handle, LANGUAGE_GETTER_PLUGIN_API);
		}
		
		if (EOK == fn_translate_word(in_word, word_translated))
		{
			last_word_size = strlen(word_translated);
			pCurrentChar = out_word + detected_size;
			strncpy(pCurrentChar, word_translated, last_word_size);
			pCurrentChar = pCurrentChar + last_word_size;
			strcat(pCurrentChar,";");
			detected_size = detected_size + last_word_size + 1;
			memset(word_translated, '\0', MAX_STRING_LEN);
		}
		else
		{
			result = ETRANSLATE_ERR;
		}
	}

	return result;
}

errors_t closePlugins()
{
	int index = 0;
	errors_t result = EOK;
	
	for (index = 0; index < plugin_counter; index++)
	{
		if (0 != dlclose(plib_handles[index]))
		{
			result = ECLOSELIB_ERR;
		};
	}
	
	return result;
}