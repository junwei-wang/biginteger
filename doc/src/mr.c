int millerrabin(bignum n, int t)
{
    bignum two = digit2bignum(2);
    bignum three = digit2bignum(3);
    if (compare(n, three) == 0 || compare(n, two) == 0) {
        free(two.tab);
        free(three.tab);
        return 1;
    }

    if (n.tab[0] % 2 == 0)
        return 0;

    int i, j, s = 0, len = length(n);
    bignum r, a, beta, one, n_minus_one;
    block *bt;

    one = digit2bignum(1);
    n_minus_one = sub(n, one);

    for (j = 0; j < n_minus_one.size; j++) {
        if (n_minus_one.tab[j] == 0) {
            s += E;
            continue;
        } else {
            for (i = 0; i < E; i++)
                if (((n_minus_one.tab[j] >> i) & 0x1) == (block)0)
                    s++;
                else
                    break;
            break;
        }
    }

    r = rightshift(n_minus_one, s);

    while (t--) {
        // TODO: a should be from [2, n-1]
        a = genrandom(rand() % len);
        if (isone(a) || iszero(a)) {
            t++;
            free(a.tab);
            continue;
        }


        beta = expmod(a, r, n);
        free(a.tab);
        a.tab = NULL;


        while(!isone(beta) && compare(beta, n_minus_one) != 0) {
            j = s - 1;
            while (j-- && compare(beta, n_minus_one)!=0) {
                bt = beta.tab;
                beta = multmod(beta, beta, n);
                free(bt);
                if (isone(beta))
                    return 0;
            }

            if (compare(beta, n_minus_one) != 0)
                return 0;
        }
    }

    free(r.tab);
    free(one.tab);
    free(n_minus_one.tab);

    return 1;
}
