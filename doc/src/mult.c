bignum mult(bignum a, bignum b)
{
    int i, j;
    block tmp, carry;
    bignum prd;

    if (iszero(a) || iszero(b)) {
        prd.sign = 1;
        prd.size = 1;
        prd.tab = (block *)malloc(prd.size * sizeof(block));
        prd.tab[0] = 0;
        return prd;
    }

    if (b.size > a.size)
         return mult(b, a);

    prd.sign = a.sign * b.sign;
    prd.size = a.size + b.size;
    prd.tab = (block *)malloc((prd.size)*sizeof(block));

    for (i = 0; i < prd.size; i++)
        prd.tab[i] = 0;

    for (i = 0; i < b.size; i++) {
        carry = 0;
        for (j = 0; j < a.size; j++) {
            tmp = b.tab[i] * a.tab[j] + prd.tab[i+j] + carry;
            carry = tmp / B;
            prd.tab[i+j] = tmp % B;
        }
        prd.tab[i+a.size] = carry;
    }

    for (i = prd.size-1; prd.tab[i] == 0; i--)
        ;
    prd.size = i + 1;
    return prd;
}
