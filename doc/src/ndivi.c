bignum * normalized_divi(bignum a, bignum b)
{
    int i, j, k = a.size, l = b.size;
    bignum q, r;
    block carry, tmp;

    r.sign = 1;
    r.size = a.size;
    r.tab = (block *)malloc(sizeof(block)*(r.size+1));
    for (i = 0; i < k; i++)
        r.tab[i] = a.tab[i];
    r.tab[k] = 0;

    q.sign = 1;
    q.size = k - l + 1;
    q.tab = (block *)malloc(sizeof(block)*q.size);
    
    for (i = k-l; i >= 0; i--) {
        q.tab[i] = (r.tab[i+l]*B + r.tab[i+l-1]) / b.tab[l-1];
        if (q.tab[i] >= B)
            q.tab[i] = B-1;

        carry = 0;
        for (j = 0; j < l; j++) {
            tmp = r.tab[i+j] - q.tab[i]*b.tab[j] + carry;
            carry = tmp / B;
            r.tab[i+j] = tmp % B; 
            if (tmp < 0 && r.tab[i+j] != 0) {
                carry -= 1;
                r.tab[i+j] = r.tab[i+j] + B;
            }
        }
        r.tab[i+l] += carry;

        while (r.tab[i+l] < 0) {
            carry = 0;
            for (j = 0; j < l; j++) {
                tmp = r.tab[i+j] + b.tab[j] + carry;
                carry = tmp / B;
                r.tab[i+j] = tmp % B;
            }
            r.tab[i+l] += carry;
            q.tab[i]--;
        }
    }

    for (i = k-l; i >= 1 && q.tab[i] == 0; i--)
        ;
    q.size = i+1;
    for (i = l-1; i >= 1 && r.tab[i] == 0; i--)
        ;
    r.size = i+1;

    bignum * res = (bignum *)malloc(sizeof(bignum)*2);
    res[0] = q;
    res[1] = r;
    return res;
}
