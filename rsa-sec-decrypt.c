/* rsa-sec-decrypt.c

   RSA decryption, using randomized RSA blinding to be more resistant
   to side-channel attacks like timing attacks or cache based memory
   access measurements.

   Copyright (C) 2001, 2012 Niels Möller, Nikos Mavrogiannopoulos
   Copyright (C) 2018 Red Hat, Inc.

   This file is part of GNU Nettle.

   GNU Nettle is free software: you can redistribute it and/or
   modify it under the terms of either:

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at your
       option) any later version.

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at your
       option) any later version.

   or both in parallel, as here.

   GNU Nettle is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see http://www.gnu.org/licenses/.
*/

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include "rsa.h"
#include "pkcs1-internal.h"
#include "rsa-internal.h"

#include "gmp-glue.h"

/* Variant without range check of the input, to ease testing for
   side-channel silence. */
int
_rsa_sec_decrypt (const struct rsa_public_key *pub,
		  const struct rsa_private_key *key,
		  void *random_ctx, nettle_random_func *random,
		  size_t length, uint8_t *message,
		  const mpz_t gibberish)
{
  TMP_GMP_DECL (m, mp_limb_t);
  TMP_GMP_DECL (em, uint8_t);
  int res;

  TMP_GMP_ALLOC (m, mpz_size(pub->n));
  TMP_GMP_ALLOC (em, key->size);

  /* We need a copy because m can be shorter than key_size,
   * but _rsa_sec_compute_root_tr expect all inputs to be
   * normalized to a key_size long buffer length */
  mpz_limbs_copy(m, gibberish, mpz_size(pub->n));

  res = _rsa_sec_compute_root_tr (pub, key, random_ctx, random, m, m);

  mpn_get_base256 (em, key->size, m, mpz_size(pub->n));

  res &= _pkcs1_sec_decrypt (length, message, key->size, em);

  TMP_GMP_FREE (em);
  TMP_GMP_FREE (m);
  return res;
}

int
rsa_sec_decrypt (const struct rsa_public_key *pub,
		 const struct rsa_private_key *key,
		 void *random_ctx, nettle_random_func *random,
		 size_t length, uint8_t *message,
		 const mpz_t gibberish)
{
  /* First check that input is in range. */
  if (mpz_sgn (gibberish) < 0 || mpz_cmp (gibberish, pub->n) >= 0)
    return 0;

  return _rsa_sec_decrypt (pub, key, random_ctx, random, length, message, gibberish);
}
