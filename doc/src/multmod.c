bignum multmod(bignum a, bignum b, bignum n)
{
    bignum prd = mult(a, b);
    bignum res = reminder(prd, n);

    free(prd.tab);
    prd.tab = NULL;
    return res;
}
