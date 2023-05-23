#include "stdio.h"
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "cJSON_Utils.h"

cJSON *load_from_file(char *file_name)
{
    cJSON *root = NULL;

    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf("open file %s failed\n", file_name);
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

int save_to_file(char *file_name, cJSON *obj)
{
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        printf("open file %s failed\n", file_name);
        return -1;
    }

    char *buffer = cJSON_PrintUnformatted(obj);
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

int json_filter(cJSON *array)
{
    if (!cJSON_IsArray(array)) {
        return -1;
    }

    int size = cJSON_GetArraySize(array);

    // 遍历数组
    for (int i = 0; i < size; i++) {
        cJSON *item = cJSON_GetArrayItem(array, i);

        if (cJSON_IsObject(item)) {
            json_printf(item);

            // 如果op的值不是replace, 就从数组中移除该元素
            cJSON *op = cJSON_GetObjectItem(item, "op");
            if (op && cJSON_IsString(op) && strcmp(op->valuestring, "replace") != 0) {
                cJSON_DeleteItemFromArray(array, i);
                i--;
                size--;
            }

            json_printf(array);
        }
    }

    return 0;
}

int main(int argc, char const *argv[])
{
#if 1
    cJSON *obj = load_from_file("notes.json");
    cJSON *patch = load_from_file("patch-new-filtered.json");

    cJSONUtils_ApplyPatches(obj, patch);

    save_to_file("notes_patched.json", obj);

    cJSON_Delete(obj);
    cJSON_Delete(patch);
#endif

#if 0
    cJSON *obj = load_from_file("notes.json");
    cJSON *patched = load_from_file("notes_patched.json");

    cJSON *patch = cJSONUtils_GeneratePatchesCaseSensitive(obj, patched);

    save_to_file("patch-new.json", patch);

    cJSON_Delete(patch);
    cJSON_Delete(obj);
    cJSON_Delete(patched);
#endif

#if 0
    cJSON *patch = load_from_file("patch-new.json");

    if (cJSON_IsArray(patch)) {
        printf("patch is array\n");

        json_filter(patch);

    } else {
        printf("patch is not array\n");
    }

    save_to_file("patch-new-filtered.json", patch);

    cJSON_Delete(patch);
#endif

    return 0;
}
