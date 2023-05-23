#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cJSON *load_from_file(const char *file)
{
    cJSON *root = NULL;

    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("open file %s failed\n", file);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = (char *)cJSON_malloc(file_size + 1);
    if (buffer == NULL)
    {
        printf("malloc failed\n");
        fclose(fp);
        return NULL;
    }

    fread(buffer, 1, file_size, fp);

    root = cJSON_Parse(buffer);

    cJSON_free(buffer);
    fclose(fp);

    return root;
}

int save_to_file(const char *file, const cJSON *obj, cJSON_bool format)
{
    char *buffer = NULL;
    FILE *fp = fopen(file, "w");
    if (fp == NULL)
    {
        printf("open file %s failed\n", file);
        return -1;
    }

    if (format) {
        buffer = cJSON_Print(obj);
    } else {
        buffer = cJSON_PrintUnformatted(obj);
    }

    if (buffer == NULL)
    {
        printf("cJSON_Print failed\n");
        fclose(fp);
        return -1;
    }

    fwrite(buffer, 1, strlen(buffer), fp);

    cJSON_free(buffer);
    fclose(fp);

    return 0;
}

void json_printf(cJSON *obj)
{
    char buffer[1024] = {0};
    cJSON_PrintPreallocated(obj, buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);
    return;
}
