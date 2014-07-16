/*
 * Filename:      bignum.c
 * Author:        Junwei Wang(wakemecn@gmail.com)
 * Last Modified: 2014-06-22 13:48
 * Description:
 *
 */
#include "bignum.h"

bignum digit2bignum(int d)
{
    if (d < 0 || d > 9) {
        perror("Illegal input: input should be from 0 to 9.");
        exit(1);
    }

    bignum r;
    int i = 0;
    
    r.sign = 1;
    r.tab = (block *)malloc(sizeof(block) * 4);
    r.size = 1;
    while (d >= B) {
        r.size++;
        r.tab[i++] = d % B;
        d /= B;
    }
    r.tab[i] = d;

    return r;
}

bignum str2bignum(char * str)
{
    int i, cnt;
    bignum b[11], r;
    block *tmp;

    // initialize 0, ..., 9
    for (i = 0; i < 10; i++)
        b[i] = digit2bignum(i);

    // initialize 10 
    b[10].sign = 1;
    b[10].tab = (block *)malloc(sizeof(block) * 4);
    if (B > 10) {
        b[10].size = 1;
        b[10].tab[0] = 10;
    } else {
        i = 0;
        int ten = 10;
        b[10].size = 1;
        while (ten >= B) {
            b[10].size++;
            b[10].tab[i++] = ten % B;
            ten /= B;
        }
        b[10].tab[i] = ten;
    }
    
    cnt = 0;
    r.sign = 1;
    r.size = 1;
    r.tab = (block *)malloc(sizeof(block));
    r.tab[0] = 0;

    if (str[0] == '-') {
        cnt = 1;
    }

    tmp = r.tab;
    r = add(r, b[str[cnt++]-'0']);
    free(tmp);
    for (i = cnt; i < strlen(str); i++) {
        int digit = str[i] - '0';
        if (digit < 0 || digit > 9) {
            perror("Illegal input: input string should contain only \
numbers.");
            exit(0);
        }

        tmp = r.tab;
        r = mult(r, b[10]);
        free(tmp);

        tmp = r.tab;
        r = add(r, b[str[i]-'0']);
        free(tmp);
    }

    if (str[0] == '-')
        r.sign = -1;

    for (i = 0; i <= 10; i++) {
        free(b[i].tab);
        b[i].tab = NULL;
    }

    return r;
}


void printbignum(bignum a)
{
    int i;
    if (B == 16) {
        if (a.sign == -1)
            printf("-");
        printf("0x");
        for (i = a.size-1; i >= 0; i--)
            printf("%x", a.tab[i]);
        printf("\n");
    } else {
        for (i = a.size-1; i >= 0; i--)
            printf("%5d: %lu\n", i, a.tab[i]);
        printf("B = %ld, %s\n", B, a.sign==1?"positive":"negtive");
    }
}

void copy(bignum *dest, bignum src)
{
    if (dest == &src)
        return;

    *dest = src;
    dest->tab = (block *)malloc(dest->size * sizeof(block));
    memcpy(dest->tab, src.tab, dest->size*sizeof(block));
}

int iszero(bignum a)
{
    return (a.size == 1) && (a.tab[0] == 0);
}

int isone(bignum a)
{
    return (a.size == 1) && (a.sign == 1) && (a.tab[0] == 1);
}

/*
 * a < b, return -1
 * a = b, return 0
 * a > b, return 1
 */
int compare(bignum a, bignum b)
{
    if (a.sign == -1 && b.sign == 1)
        return -1;
    if (a.sign == 1 && b.sign == -1)
        return 1;

    if (a.sign == -1 && b.sign == -1) {
        a.sign = b.sign = 1;
        return compare(b, a);
    }

    if (a.size < b.size)
        return -1;
    if (a.size > b.size)
        return 1;

    int i;
    for (i = a.size - 1; i >= 0; i--) 
        if (a.tab[i] < b.tab[i])
            return -1;
        else if (a.tab[i] > b.tab[i])
            return 1;

    return 0;
}

bignum leftshift(bignum a, int k)
{
    int i, len = length(a)+k;

    bignum res;
    res.sign = 1;
    res.size = (len/E) + ((len%E == 0) ? 0 : 1);
    res.tab = (block *)malloc((res.size) * sizeof(block));

    int m = k/E, n = k%E;
    for (i = 0; i < m; i++)
        res.tab[i] = 0;
    if (n == 0) 
        for (i = m; i < res.size; i++)
            res.tab[i] = a.tab[i-m];
    else {
        res.tab[m] = (((a.tab[0] << n) & (block)MASK));
        for (i = m+1; i < res.size-1; i++) {
            res.tab[i] = a.tab[i-m-1] >> (E-n);
            res.tab[i] |= (((a.tab[i-m] << n) & (block)MASK));
        }
        res.tab[i] = a.tab[i-m-1] >> (E-n);
        if (i-m < a.size)
            res.tab[i] |= (((a.tab[i-m] << n) & (block)MASK));
    }

    return res;
}

bignum rightshift(bignum a, int k)
{
    int i, len = length(a)-k;

    bignum res;
    res.sign = 1;

    if (len <= 0) {
        res.size = 1;
        res.tab = (block *)malloc(sizeof(block));
        res.tab[0] = 0;

        return res;
    } 
    
    res.size = (len/E) + ((len%E == 0) ? 0 : 1);
    res.tab = (block *)malloc((res.size) * sizeof(block));
    
    int m = k/E, n = k%E;
    if (n == 0) {
        for (i = 0; i < res.size; i++)
            res.tab[i] = a.tab[i+m];
    } else {
        for (i = 0; i < res.size-1; i++) {
            res.tab[i] = a.tab[i+m] >> n;
            res.tab[i] |= ((a.tab[i+m+1] << (E-n)) & MASK);
        }
        res.tab[i] = a.tab[i+m] >> n;
        if (i+m+1 < a.size)
            res.tab[i] |= ((a.tab[i+m+1] << (E-n)) & MASK);
    }

    return res;
}

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

bignum divi(bignum a, bignum n)
{
    int i, cmp;
    bignum q, tmp, ta, tn, *quorem;

    cmp = compare(a, n);
    if (cmp == -1) {
        q.sign = q.size = 1;
        q.tab = (block *)malloc(sizeof(block));
        q.tab[0] = 0;
        return q;
    }
    if (cmp == 0) {
        q.sign = q.size = 1;
        q.tab = (block *)malloc(sizeof(block));
        q.tab[0] = 1;
        return q;
    }

    if (isnormalized(n)) {
        quorem = normalized_divi(a, n);
        q = quorem[0];
        free(quorem[1].tab);
        free(quorem);

        return q;
    }

    for (i = E-1; i >= 0; i--)
        if ((n.tab[n.size-1] >> i) & 0x1)
            break;

    ta = leftshift(a, E-i-1);
    tn = leftshift(n, E-i-1);
    quorem = normalized_divi(ta, tn);
    q = quorem[0];

    free(quorem[1].tab);
    free(quorem);
    free(ta.tab);
    free(tn.tab);
    return q;
}

bignum reminder(bignum a, bignum n)
{
    int i, cmp;
    bignum r, *quorem, ta, tn, tr;

    cmp = compare(a, n);
    if (cmp == -1) {
        copy(&r, a);
        return r;
    }
    if (cmp == 0) {
        r.sign = r.size = 1;
        r.tab = (block *)malloc(sizeof(block));
        r.tab[0] = 0;
        return r;
    }

    if (isnormalized(n)) {
        quorem = normalized_divi(a, n);
        r = quorem[1];
        free(quorem[0].tab);
        free(quorem);

        return r;
    }

    for (i = E-1; i >= 0; i--)
        if ((n.tab[n.size-1] >> i) & 0x1)
            break;

    ta = leftshift(a, E-i-1);
    tn = leftshift(n, E-i-1);
    quorem = normalized_divi(ta, tn);
    tr = quorem[1];
    r = rightshift(tr, E-i-1);

    free(quorem[0].tab);
    free(quorem);
    free(ta.tab);
    free(tn.tab);
    free(tr.tab);

    return r;
}

bignum addmod(bignum a, bignum b, bignum n)
{
    bignum sum = add(a, b);
    bignum res = reminder(sum, n);

    free(sum.tab);
    sum.tab = NULL;
    return res;
}

bignum multmod(bignum a, bignum b, bignum n)
{
    bignum prd = mult(a, b);
    bignum res = reminder(prd, n);

    free(prd.tab);
    prd.tab = NULL;
    return res;
}

bignum expmod(bignum a, bignum b, bignum n)
{
    int i, j, start;
    bignum r = reminder(a, n);
    block *t;

    start = length(b) % E;
    if (start == 0)
        start = E;
    for (j = start-2; j >= 0; j--) {
        t = r.tab;
        r = multmod(r, r, n);
        free(t);

        if ((b.tab[b.size-1] >> j) & 0x1 == 1) {
            t = r.tab;
            r = multmod(r, a, n);
            free(t);
        }
    }
 
    for (i = b.size-2; i >= 0; i--) {
        for (j = E-1; j >= 0; j--) {
            t = r.tab;
            r = multmod(r, r, n);
            free(t);

            if ((b.tab[i] >> j) & 0x1 == 1) {
                t = r.tab;
                r = multmod(r, a, n);
                free(t);
            }
        }
    }

    return r;
}

int millerrabin(bignum n, int t)
{
    bignum two = digit2bignum(2);
    bignum three = digit2bignum(3);
    if (compare(n, three) == 0 || compare(n, two) == 0) {
        free(two.tab);
        free(three.tab);
        return 1;
    }

    if (n.tab[0] % 2 == 0)
        return 0;

    int i, j, s = 0, len = length(n);
    bignum r, a, beta, one, n_minus_one;
    block *bt;

    one = digit2bignum(1);
    n_minus_one = sub(n, one);

    for (j = 0; j < n_minus_one.size; j++) {
        if (n_minus_one.tab[j] == 0) {
            s += E;
            continue;
        } else {
            for (i = 0; i < E; i++)
                if (((n_minus_one.tab[j] >> i) & 0x1) == (block)0)
                    s++;
                else
                    break;
            break;
        }
    }

    r = rightshift(n_minus_one, s);

    while (t--) {
        // TODO: a should be from [2, n-1]
        a = genrandom(rand() % len);
        if (isone(a) || iszero(a)) {
            t++;
            free(a.tab);
            continue;
        }


        beta = expmod(a, r, n);
        free(a.tab);
        a.tab = NULL;


        while(!isone(beta) && compare(beta, n_minus_one) != 0) {
            j = s - 1;
            while (j-- && compare(beta, n_minus_one)!=0) {
                bt = beta.tab;
                beta = multmod(beta, beta, n);
                free(bt);
                if (isone(beta))
                    return 0;
            }

            if (compare(beta, n_minus_one) != 0)
                return 0;
        }
    }

    free(r.tab);
    free(one.tab);
    free(n_minus_one.tab);

    return 1;
}

/* slow algorithm */
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

int length(bignum a)
{
    block n = a.tab[a.size-1];
    int len = a.size * E;

    int i;
    for (i = E-1; i > 0; i--)
        if (((n >> i) & 0x1) == 0)
            len--;
        else
            break;

    return len;
}

bignum inverse(bignum a, bignum n)
{
    int mark = 0, mark1 = 1;
    bignum r[2], v[2], q;
    block *t;

    r[0] = reminder(n, a);
    copy(&r[1], a);
    q = divi(n, a);
    
    v[1] = digit2bignum(1);
    /*
    v[1].sign = 1;
    v[1].size = 1;
    v[1].tab = (block *)malloc((v[1].size)*sizeof(block));
    v[1].tab[0] = 1;
    */
    copy(&v[0], q);
    free(q.tab);
    if (!iszero(v[0]))
        v[0].sign *= -1;


    bignum tmp0, tmp1;
    while (!iszero(r[mark])) {
        mark ^= 1;
        mark1 ^= 1;

        q = divi(r[mark], r[mark1]);

        t = r[mark].tab;
        r[mark] = reminder(r[mark], r[mark1]); 
        free(t);

        tmp0 = mult(q, v[mark1]);
        tmp1 = sub(v[mark], tmp0);

        free(v[mark].tab);
        v[mark] = reminder(tmp1, n);

        free(q.tab);
        free(tmp0.tab);
        free(tmp1.tab);
    }

    tmp0 = add(v[mark^1], n);
    bignum res = reminder(tmp0, n);

    free(tmp0.tab);
    free(v[0].tab);
    free(v[1].tab);
    free(r[0].tab);
    free(r[1].tab);

    return res;
}


bignum gcd(bignum a, bignum b)
{
    bignum res;
    bignum tmp;
    res.sign = 1;

    if (iszero(b)) {
        res.tab = (block *)malloc((res.size) * sizeof(block));
        copy(&res, a);
        return res;
    }
    if (iszero(a)) {
        res.tab = (block *)malloc((res.size) * sizeof(block));
        copy(&res, b);
        return res;
    }

    if (compare(a, b)) {
        tmp = reminder(a, b);
        res = gcd(b, tmp);
        free(tmp.tab);
        return res;
    } else {
        tmp = reminder(b, a);
        res = gcd(a, tmp);
        free(tmp.tab);
        return res;
    }
}

int isnormalized(bignum a)
{
    if (a.sign == -1)
        return 0;

    return (a.tab[a.size-1] >> (E-1)) & 0x1; 
}

// a > b > 0, and b is normalized;
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
