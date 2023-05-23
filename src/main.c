#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "cJSON_Utils.h"

#include "utils.h"

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
            // 如果op的值不是replace, 就从数组中移除该元素
            cJSON *op = cJSON_GetObjectItem(item, "op");
            if (op && cJSON_IsString(op) && strcmp(cJSON_GetStringValue(op), "replace") != 0) {
                cJSON_DeleteItemFromArray(array, i);
                i--;
                size--;
            }
        }
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    int ret = 0;
    cJSON *template = NULL;
    cJSON *patch = NULL;
    cJSON *patched = NULL;
    cJSON *target = NULL;

    // 参数检测
    if (argc != 4) {
        printf("Usage: %s <template> <patch_file> <target_file>\n", argv[0]);
        return -1;
    }

    // 读取模板文件
    template = load_from_file(argv[1]);
    if (template == NULL) {
        printf("load_from_file %s failed\n", argv[1]);
        ret = -1;
        goto out;
    }

    // 读取patch文件
    patch = load_from_file(argv[2]);
    if (patch == NULL) {
        printf("load_from_file %s failed\n", argv[2]);
        ret = -1;
        goto out;
    }

    // 生成 json patch
    patched = cJSONUtils_GeneratePatchesCaseSensitive(template, patch);
    if (patched == NULL) {
        printf("cJSONUtils_GeneratePatchesCaseSensitive failed\n");
        ret = -1;
        goto out;
    }

    // 过滤 json patch
    ret = json_filter(patched);
    if (ret != 0) {
        printf("json_filter failed\n");
        goto out;
    }

    // 深拷贝 template 作为 target
    target = cJSON_Duplicate(template, 1);
    if (target == NULL) {
        printf("cJSON_Duplicate failed\n");
        ret = -1;
        goto out;
    }

    // 应用 json patch 到 target
    ret = cJSONUtils_ApplyPatches(target, patched);
    if (ret != 0) {
        printf("cJSONUtils_ApplyPatches failed\n");
        goto out;
    }

    // sort target
    cJSONUtils_SortObject(target);

    // 保存 target 到文件
    ret = save_to_file(argv[3], target, 0);
    if (ret != 0) {
        printf("save_to_file %s failed\n", argv[3]);
        goto out;
    }

out:
    if (target) {
        cJSON_Delete(target);
        target = NULL;
    }

    if (patched) {
        cJSON_Delete(patched);
        patched = NULL;
    }

    if (patch) {
        cJSON_Delete(patch);
        patch = NULL;
    }

    if (template) {
        cJSON_Delete(template);
        template = NULL;
    }

    return ret;
}
