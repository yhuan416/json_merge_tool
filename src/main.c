#include "stdio.h"

#include "cJSON.h"
#include "cJSON_Utils.h"

int main(int argc, char const *argv[])
{
    printf("json_merge_tool: %s.\r\n", cJSON_Version());

    return 0;
}
