bignum add(bignum a, bignum b)
{
    if (a.sign == -1 && b.sign == 1) {
        a.sign = 1;
        return sub(b, a);
    } else if (a.sign == 1 && b.sign == -1) {
        b.sign = 1;
        return sub(a, b);
    }

    if (b.size > a.size)
        return add(b, a);

    int i; 
    block carry = 0, tmp;
    bignum sum;

    sum.sign = a.sign;
    sum.tab = (block *)malloc((a.size + 1) * sizeof(block));
    sum.size = a.size;

    for (i  = 0; i < b.size; i++) {
        tmp = a.tab[i] + b.tab[i] + carry;
        carry = tmp / B;
        sum.tab[i] = tmp % B;
    }
    for (; i < a.size; i++) {
        tmp = a.tab[i] + carry;
        carry = tmp / B;
        sum.tab[i] = tmp % B;
    }

    sum.tab[i] = carry;
    if (carry)
        sum.size++;

    return sum;
}
