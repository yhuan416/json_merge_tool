#ifndef __UTILS_H__
#define __UTILS_H__

#include "cJSON.h"
#include "cJSON_Utils.h"

cJSON *load_from_file(const char *file);
int save_to_file(const char *file, const cJSON *obj, cJSON_bool format);

void json_printf(cJSON *obj);

#endif
