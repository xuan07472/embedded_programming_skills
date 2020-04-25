
#include <stdio.h>
#include "publicdef.h"

#ifdef ALL_MODULES
#include "1_src/2-test/test.h"

int main()
{
    test();
    printf("main\n");
}
#endif /* ALL_MODULES */
