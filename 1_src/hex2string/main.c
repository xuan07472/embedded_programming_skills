#include <stdio.h>
#include <stdlib.h>

char hex2str(unsigned char i)
{
    if (i < 10)
        return '0' + i;
    else if ((i >= 10) && (i < 16) )
        return 'A' + i - 10;
    else
        printf("char %x error!\n", i);
    
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int len;
    char *p;

    if (argc < 2)
        printf("Use %s filename\n", argv[0]);

    fp = fopen(argv[1], "r");
    fseek(fp, 0L, SEEK_END);
    len = ftell(fp);

    p = malloc(len);
    fseek(fp, 0L, SEEK_SET);
    fread(p, 1, len, fp);
    fflush(fp);
    fclose(fp);

//    for (int k = 0; k < 128; k++)
//        printf("%02X ", p[k]);

    fp = fopen("output.txt", "w+");
    char w[4];
    int i = 0;
    char tmp;
    while (i < len) {
        for (int j = 0; j < 16; j++, i++) {
            tmp = p[i];
            w[0] = hex2str((tmp & 0xF0) >> 4);
            w[1] = hex2str(tmp & 0x0F);
            w[2] = ' ';
            w[3] = '\0';
            fwrite(w, 1, 3, fp);
        }
        fwrite("\n", 1, 1, fp);
    }

    fflush(fp);
    fclose(fp);
}
