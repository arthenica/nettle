/* gcm-camellia256.c

   Copyright (C) 2011, 2014 Niels Möller

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

#include <assert.h>

#include "gcm.h"

void
gcm_camellia256_set_key(struct gcm_camellia256_ctx *ctx, const uint8_t *key)
{
  GCM_SET_KEY(ctx, camellia256_set_encrypt_key, camellia256_crypt, key);
}

void
gcm_camellia256_set_iv (struct gcm_camellia256_ctx *ctx,
			size_t length, const uint8_t *iv)
{
  GCM_SET_IV (ctx, length, iv);
}

void
gcm_camellia256_update (struct gcm_camellia256_ctx *ctx,
			size_t length, const uint8_t *data)
{
  GCM_UPDATE (ctx, length, data);
}

void
gcm_camellia256_encrypt(struct gcm_camellia256_ctx *ctx,
			size_t length, uint8_t *dst, const uint8_t *src)
{
  GCM_ENCRYPT(ctx, camellia256_crypt, length, dst, src);
}

void
gcm_camellia256_decrypt(struct gcm_camellia256_ctx *ctx,
			size_t length, uint8_t *dst, const uint8_t *src)
{
  GCM_DECRYPT(ctx, camellia256_crypt, length, dst, src);
}

void
gcm_camellia256_digest(struct gcm_camellia256_ctx *ctx,
		       uint8_t *digest)
{
  GCM_DIGEST(ctx, camellia256_crypt, digest);
}
