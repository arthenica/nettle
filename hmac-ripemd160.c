/* hmac-ripemd160.c

   HMAC-RIPEMD160 message authentication code.

   Copyright (C) 2011, 2025 Niels Möller

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

#include "hmac.h"
#include "hmac-internal.h"

void
hmac_ripemd160_set_key(struct hmac_ripemd160_ctx *ctx,
		       size_t key_length, const uint8_t *key)
{
  _nettle_hmac_set_key (sizeof(ctx->outer), ctx->outer, ctx->inner, &ctx->state,
			ctx->state.block, &nettle_ripemd160, key_length, key);
}

void
hmac_ripemd160_update(struct hmac_ripemd160_ctx *ctx,
		      size_t length, const uint8_t *data)
{
  ripemd160_update(&ctx->state, length, data);
}

void
hmac_ripemd160_digest(struct hmac_ripemd160_ctx *ctx,
		      uint8_t *digest)
{
  _NETTLE_HMAC_DIGEST (ctx->outer, ctx->inner, &ctx->state, ripemd160_digest,
		       RIPEMD160_DIGEST_SIZE, digest);
}
