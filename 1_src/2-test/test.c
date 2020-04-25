#include <stdio.h>
#include "test.h"

#ifdef TEST_MODULE
int main()
{
    printf("test\n");
}
#else /* TEST_MODULE */
int test()
{
    printf("test demo\n");
}
#endif /* TEST_MODULE */
