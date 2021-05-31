#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int ret, flen, i, num;
    FILE *fp_r, *fp_w;
    char *p;

    fp_r = fopen("u-boot.spi.ais", "r");
    if (!fp_r)
        printf("fopen read file fail!\n");

    fp_w = fopen("u-boot.ais.bin", "w+");
    if (!fp_w)
        printf("fopen write file fail!\n");

    fseek(fp_r, 0L, SEEK_END); /* 定位到文件末尾 */
    flen = ftell(fp_r); /* 得到文件大小 */
    if (flen % 4) {
        printf("length is not multiple of 4!\n");
    }
    p = (char *)malloc(flen + 1); /* 根据文件大小动态分配内存空间 */
    if( p == NULL) {
        fclose(fp_r);
        fclose(fp_w);
        printf("malloc fail!\n");
        return -1;
    }
    fseek(fp_r, 0L, SEEK_SET); /* 定位到文件开头 */
    fread(p, flen, 1, fp_r); /* 一次性读取全部文件内容 */
    p[flen] = 0; /* 字符串结束标志 */

    for (i = 0; i < flen; i+=4) {
        num = *(int *)(&p[i]);
        fprintf(fp_w, "0x%08x,\n", num);
    }
 
    free(p);
    fflush(fp_w);
    fclose(fp_r);
    fclose(fp_w);

    return 0;
}
