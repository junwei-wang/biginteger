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
