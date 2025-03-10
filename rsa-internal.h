/* rsa-internal.h

   The RSA publickey algorithm.

   Copyright (C) 2001, 2002 Niels Möller

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

#ifndef NETTLE_RSA_INTERNAL_H_INCLUDED
#define NETTLE_RSA_INTERNAL_H_INCLUDED

#include "rsa.h"
#include "nettle-meta.h"

#define _rsa_verify _nettle_rsa_verify
#define _rsa_verify_recover _nettle_rsa_verify_recover
#define _rsa_check_size _nettle_rsa_check_size
#define _rsa_sec_compute_root_itch _nettle_rsa_sec_compute_root_itch
#define _rsa_sec_compute_root _nettle_rsa_sec_compute_root
#define _rsa_sec_compute_root_tr _nettle_rsa_sec_compute_root_tr
#define _rsa_oaep_encrypt _nettle_rsa_oaep_encrypt
#define _rsa_oaep_decrypt _nettle_rsa_oaep_decrypt

/* Internal functions. */
int
_rsa_verify(const struct rsa_public_key *key,
	    const mpz_t m,
	    const mpz_t s);

int
_rsa_verify_recover(const struct rsa_public_key *key,
		    mpz_t m,
		    const mpz_t s);

size_t
_rsa_check_size(mpz_t n);

/* side-channel silent root computation */
mp_size_t
_rsa_sec_compute_root_itch(const struct rsa_private_key *key);
void
_rsa_sec_compute_root(const struct rsa_private_key *key,
                      mp_limb_t *rp, const mp_limb_t *mp,
                      mp_limb_t *scratch);

/* Safe side-channel silent variant, using RSA blinding, and checking the
 * result after CRT. In-place calls, with x == m, is allowed. */
int
_rsa_sec_compute_root_tr(const struct rsa_public_key *pub,
			 const struct rsa_private_key *key,
			 void *random_ctx, nettle_random_func *random,
			 mp_limb_t *x, const mp_limb_t *m);

int
_rsa_oaep_encrypt (const struct rsa_public_key *key,
		   void *random_ctx, nettle_random_func *random,
		   void *hash_ctx, const struct nettle_hash *hash,
		   size_t label_length, const uint8_t *label,
		   size_t length, const uint8_t *message,
		   uint8_t *ciphertext);

int
_rsa_oaep_decrypt (const struct rsa_public_key *pub,
		   const struct rsa_private_key *key,
		   void *random_ctx, nettle_random_func *random,
		   void *hash_ctx, const struct nettle_hash *hash,
		   size_t label_length, const uint8_t *label,
		   size_t *length, uint8_t *message,
		   const uint8_t *ciphertext);

#endif /* NETTLE_RSA_INTERNAL_H_INCLUDED */
