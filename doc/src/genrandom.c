bignum genrandom(int length)
{
    int i, j, n;
    bignum ret;

    if (length == 0) {
        ret.sign = 1;
        ret.size = 1;
        ret.tab = (block *)malloc(sizeof(block));
        ret.tab[0] = 0;
        return ret;
    }

    ret.sign = 1;
    ret.size = length / E;
    if (length % E != 0)
        ret.size++;
    ret.tab = (block *)malloc(sizeof(block)*ret.size);

    for (i = 0; (i+1) * E < length; i++) {
        ret.tab[i] = rand() % B;
    }

    n = length - i*E;
    ret.tab[i] = ((block)rand()) % B;
    ret.tab[i] |= ((block)0x1 << (n-1));

    for (j = n; j < E; j++)
        ret.tab[i] &= ~((block)0x1 << j);

    return ret;
}
