bignum addmod(bignum a, bignum b, bignum n)
{
    bignum sum = add(a, b);
    bignum res = reminder(sum, n);

    free(sum.tab);
    sum.tab = NULL;
    return res;
}
