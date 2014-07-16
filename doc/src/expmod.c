bignum expmod(bignum a, bignum b, bignum n)
{
    int i, j, start;
    bignum r = reminder(a, n);
    block *t;

    start = length(b) % E;
    if (start == 0)
        start = E;
    for (j = start-2; j >= 0; j--) {
        t = r.tab;
        r = multmod(r, r, n);
        free(t);

        if ((b.tab[b.size-1] >> j) & 0x1 == 1) {
            t = r.tab;
            r = multmod(r, a, n);
            free(t);
        }
    }
 
    for (i = b.size-2; i >= 0; i--) {
        for (j = E-1; j >= 0; j--) {
            t = r.tab;
            r = multmod(r, r, n);
            free(t);

            if ((b.tab[i] >> j) & 0x1 == 1) {
                t = r.tab;
                r = multmod(r, a, n);
                free(t);
            }
        }
    }

    return r;
}
