/*************************
*  example include file  *
*************************/

#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include "../Globals/globals.h"

errors_t readCurrentPluginNames(char const * const _cPath, int * current_dict_size, char * const result);
errors_t loadPlugins(char const * const _cPath, const int current_dict_size, char * plugin_name_array);
errors_t loadPlugin(char const * const _cPath, char const * const plugin_name);
errors_t readLanguagesLoaded(char * currentLanguages);
errors_t translateWord(char const * const in_word, char * const out_word);
errors_t closePlugins();

#endif
