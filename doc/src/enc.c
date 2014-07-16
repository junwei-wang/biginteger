bignum RSAencrypt(bignum m, bignum e, bignum n)
{
    return expmod(m, e, n);
}
