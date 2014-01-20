/* salsa20-set-key.c
 *
 * The Salsa20 stream cipher.
 */

/* nettle, low-level cryptographics library
 *
 * Copyright (C) 2012 Simon Josefsson, Niels Möller
 *  
 * The nettle library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 * 
 * The nettle library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with the nettle library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02111-1301, USA.
 */

/* Based on:
   salsa20-ref.c version 20051118
   D. J. Bernstein
   Public domain.
*/

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>

#include "salsa20.h"

#include "macros.h"

void
salsa20_set_key(struct salsa20_ctx *ctx,
		size_t length, const uint8_t *key)
{
  switch (length)
    {
    case SALSA20_128_KEY_SIZE:
      salsa20_128_set_key (ctx, key);
      break;
    case SALSA20_256_KEY_SIZE:
      salsa20_256_set_key (ctx, key);
      break;
    default:
      abort();
    }
}
