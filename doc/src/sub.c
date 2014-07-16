bignum sub(bignum a, bignum b)
{
    int i, j;
    block tmp, carry;
    bignum diff;
    
    if (a.sign == 1 && b.sign == -1) {
        b.sign = 1;
        return add(a, b);
    } 
    if (a.sign == -1 && b.sign == 1) {
        b.sign = -1;
        return add(a, b);
    } 

    if (a.sign == -1 && b.sign == -1) {
        a.sign = b.sign = 1;
        return sub(b, a);
    } 

    if (a.size < b.size) {
        diff = sub(b, a);
        diff.sign = -1;
        return diff;
    }

    if (a.size == b.size) {
        for (i = a.size-1; (i >=0) && (a.tab[i] == b.tab[i]); i--) 
            ;
        if (i == -1) {
            diff.sign = 1;
            diff.size = 1;
            diff.tab = (block *)malloc(sizeof(block));
            diff.tab[0] = 0;
            return diff;
        }

        diff.size = i + 1;
        diff.tab = (block *)malloc(diff.size * sizeof(block));
        carry = 0;
        if (a.tab[i] > b.tab[i]) {
            diff.sign = 1;
            for (j = 0; j <= i; j++) {
                tmp = a.tab[j] - b.tab[j] + carry;
                carry = (tmp < 0) ? -1 : 0;
                diff.tab[j] = (tmp + B) % B;
            }
        } else {
            diff.sign = -1;
            for (j = 0; j <= i; j++) {
                tmp = b.tab[j] - a.tab[j] + carry;
                carry = (tmp < 0) ? -1 : 0;
                diff.tab[j] = (tmp + B) % B;
            }
        }
    } else {
        diff.sign = a.sign;
        diff.size = a.size;
        diff.tab = (block *)malloc((diff.size)*sizeof(block));
        carry = 0;
        for (i = 0; i < b.size; i++) {
            tmp = a.tab[i] - b.tab[i] + carry;
            carry = (tmp < 0) ? -1 : 0;
            diff.tab[i] = (tmp + B) % B;
        }

        for (; i < a.size; i++) {
            tmp = a.tab[i] + carry;
            carry = (tmp < 0) ? -1 : 0;
            diff.tab[i] = (tmp + B) % B;
        }
    }

    for (i = diff.size-1; diff.tab[i] == 0; i--)
        ;
    diff.size = i + 1;
    return diff;
}
