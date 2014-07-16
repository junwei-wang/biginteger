bignum RSAdecrypt(bignum c, bignum d, bignum n)
{
    return expmod(c, d, n);
}
