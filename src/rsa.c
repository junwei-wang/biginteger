/*
 * Filename:      rsa.c
 * Author:        Junwei Wang(wakemecn@gmail.com)
 * Last Modified: 2014-06-22 10:33
 * Description:   
 *
 */
#include "rsa.h"

void keygen(bignum * n, bignum * e, bignum * d, int len)
{
    bignum p, q, phi_n;
    bignum t0, t1, bgcd, tmp;
    bignum ONE = digit2bignum(1);

    p = genrandomprime(len);
    q = genrandomprime(len);

    while (compare(p, q) == 0) {
        free(q.tab);
        q = genrandomprime(len);
    }
    *n = mult(p, q);
    t0 = sub(p, ONE);
    t1 = sub(q, ONE);
    phi_n = mult(t0, t1);
    free(t0.tab);
    free(t1.tab);
    
    *e = digit2bignum(3);

    while (1) {
        bgcd = gcd(*e, phi_n);
        if (compare(bgcd, ONE) == 0) {
            free(bgcd.tab);

            *d = inverse(*e, phi_n);
            break;
        }

        int e_len;
        do {
            e_len = rand() % (length(*n));
        } while (e_len <= 1);

        do {
            free(e->tab);
            *e = genrandom(e_len);
        } while (iszero(*e) || isone(*e));
    }

    free(ONE.tab);
    free(p.tab);
    free(q.tab);
    free(phi_n.tab);
}

bignum RSAencrypt(bignum m, bignum e, bignum n)
{
    return expmod(m, e, n);
}

bignum RSAdecrypt(bignum c, bignum d, bignum n)
{
    return expmod(c, d, n);
}

void testRSA(int len)
{
    bignum n, e, d, m, c, _m;

    m = genrandom(len - 5);
    printf("Keygen\n");
    keygen(&n, &e, &d, len);
    printf("Encrypt\n");
    c = RSAencrypt(m, e, n);
    printf("Decrypt\n");
    _m = RSAdecrypt(c, d, n);

    printf("%s\n", (compare(m, _m) == 0) ? "Success" : "Fail");

    if (compare(m, _m) != 0) {
        printf ("n = ");
        printbignum(n);
        printf ("e = ");
        printbignum(e);
        printf ("d = ");
        printbignum(d);
        printf ("m = ");
        printbignum(m);
        printf ("c = ");
        printbignum(c);
        printf ("_m = ");
        printbignum(_m);
    }
    
    free(n.tab);
    free(e.tab);
    free(d.tab);
    free(m.tab);
    free(c.tab);
    free(_m.tab);
}
