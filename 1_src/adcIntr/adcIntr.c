/* ADC device option */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

int main(void)
{
    FILE *fd;
    size_t ret;
    char a_data[16] = {0x03, 0x04, 0x0};
    int i;

    sleep(3);
    /* in_voltage0_scale is battery voltage, in_voltage1_scale is uart RXD. */
    fd = fopen("/sys/bus/iio/devices/iio:device0/in_voltage1_scale", "r+");
    if (fd == NULL) {
        printf("->Open in_voltage1_scale fail!\n");
        return 0;
    }
    
    /* 也可以使用echo 1 > /sys/bus/iio/devices/iio:device0/in_voltage0_scale */
    /* the int of fwrite string: '0' enable interrupt, '1' disable interrupt */
    /* The data is into ads7924_write_raw() val, val2 is always 0*/
#if 0
    ret = fwrite("1", 1, 2, fd);
    if (ret != 1) {
        printf("->Write data fail!\n");
    }
#else /* 如果写和读在一起，读的时候只有val2的int值被转换成了字符串 */
    /* 也可以cat /sys/bus/iio/devices/iio:device0/in_voltage0_scale的话就显示val.val2 */
    fread(a_data, 1, 15, fd);
    printf("->The data:%s.\n", a_data); /* The float data is back from ads7924_read_raw() val.val2 */
#endif
    fclose(fd);
    return 0;
}
