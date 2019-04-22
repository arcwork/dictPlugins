#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include "../Globals/globals.h"
#include "PluginManager.h"


char const * const pluginPath = "plugins";        // plugins directory 

int main( int argc, char **argv ) {
  errors_t read_result = EUNDEFINED;
  errors_t load_result = EUNDEFINED;
  char currentPlugins[DICT_NAME_MAX_SIZE] = {'\0'};
  char currentLanguages[DICT_NAME_MAX_SIZE] = {'\0'};
  char currentWordTranslation[DICT_NAME_MAX_SIZE] = {'\0'};
  int current_dict_names_size = 0;

  read_result = readCurrentPluginNames(pluginPath, &current_dict_names_size, currentPlugins);

  if (EOK == read_result)
  {
	  if (current_dict_names_size < DICT_NAME_MAX_SIZE)
	  {
		  //printf("%s \n",currentPlugins);
		  load_result = loadPlugins(pluginPath, current_dict_names_size, currentPlugins);
		  
		  if (EOK == load_result)
		  {
			readLanguagesLoaded(currentLanguages);
			//printf("%s \n",currentLanguages);
			for( int i = 1; i < argc; ++i ) {
				translateWord(argv[ i ], currentWordTranslation);
				printf("%s \n", currentWordTranslation);
			}

			closePlugins();
		  }
		  else
		  {
			printf("Error %d", load_result);  
		  }
	  }
  }
 
  return(0);
}
