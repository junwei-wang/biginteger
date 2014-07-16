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
