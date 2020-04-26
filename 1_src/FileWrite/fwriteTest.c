#include <stdio.h>
#include <string.h>

FILE *fp;
char data[1024] = {0x55};
void main(void)
{
    int ret;
    memset(data, 0x55, 1024);

    fp = fopen("testFile.DAT", "w");
    if (!fp)
        printf("fopen err\n");
    
    ret = fwrite(data, 1024, 1, fp);
    if (!ret)
        printf("fwrite err\n");
    ret = fwrite(data, 1024, 1, fp);
    if (!ret)
        printf("fwrite err\n");
    ret = fwrite(data, 1024, 1, fp);
    if (!ret)
        printf("fwrite err\n");
    ret = fwrite(data, 1024, 1, fp);
    if (!ret)
        printf("fwrite err\n");
    fflush(fp);
    fclose(fp);
}
