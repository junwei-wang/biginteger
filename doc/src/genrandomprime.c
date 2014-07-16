bignum genrandomprime(int length)
{
    bignum p = genrandom(length);

    while (!millerrabin(p, TEST_CNT)) {
        free(p.tab);
        p.tab = NULL;
        p = genrandom(length);
    }

    return p;
}
