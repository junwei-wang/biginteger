/*
 * Filename:      test.c
 * Author:        Junwei Wang(wakemecn@gmail.com)
 * Last Modified: 2014-06-22 12:39
 * Description:
 *
 */
//#include <stdio.h>
#include <time.h>
#include "bignum.h"
#include "rsa.h"

int main(void)
{
    srand(time(NULL));
    int i;
    printf("\nBe patient we have 10 test cases.\nPress Ctrl-C to stop test.\n");
    for (i = 0; i < 10; i++) {
        printf("\n***** case %d *****\n", i+1);
        testRSA(512);
    }

    return 0;
}
