/*
 * Filename:      rsa.h
 * Author:        Junwei Wang(wakemecn@gmail.com)
 * Last Modified: 2014-06-14 20:49
 * Description:   Plain RSA implementation
 *
 */

#ifndef __RSA_H__
#define __RSA_H__

#include "bignum.h"

/* plain RSA algorithm */
void keygen(bignum * n, bignum * e, bignum * d, int length);
bignum RSAencrypt(bignum m, bignum e, bignum n);
bignum RSAdecrypt(bignum c, bignum d, bignum n);
void testRSA(int length);

#endif

