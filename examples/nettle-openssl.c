/* nettle-openssl.c

   Glue that's used only by the benchmark, and subject to change.

   Copyright (C) 2002, 2017 Niels Möller
   Copyright (C) 2017 Red Hat, Inc.

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

/* Openssl glue, for comparative benchmarking only */

#if WITH_OPENSSL

#include <assert.h>

#include <openssl/evp.h>

#include "non-nettle.h"
#include "md5.h"
#include "sha1.h"

/* We use Openssl's EVP api for all openssl ciphers. This API selects
   platform-specific implementations if appropriate, e.g., using x86
   AES-NI instructions. */
struct openssl_cipher_ctx {
  EVP_CIPHER_CTX *evp;
};

/* We use Openssl's EVP api for all openssl hashes. This API selects
   platform-specific implementations if appropriate, e.g., using x86
   AES-NI instructions. */
struct openssl_hash_ctx {
  EVP_MD_CTX *evp;
};

static void
openssl_evp_set_encrypt_key(void *p, const uint8_t *key,
			    const EVP_CIPHER *cipher)
{
  struct openssl_cipher_ctx *ctx = p;
  int ret;
  ctx->evp = EVP_CIPHER_CTX_new();
  ret = EVP_CipherInit_ex(ctx->evp, cipher, NULL, key, NULL, 1);
  assert(ret == 1);
  EVP_CIPHER_CTX_set_padding(ctx->evp, 0);
}
static void
openssl_evp_set_decrypt_key(void *p, const uint8_t *key,
			    const EVP_CIPHER *cipher)
{
  struct openssl_cipher_ctx *ctx = p;
  int ret;
  ctx->evp = EVP_CIPHER_CTX_new();
  ret = EVP_CipherInit_ex(ctx->evp, cipher, NULL, key, NULL, 0);
  assert(ret == 1);
  EVP_CIPHER_CTX_set_padding(ctx->evp, 0);
}

static void
openssl_evp_encrypt(const void *p, size_t length,
		    uint8_t *dst, const uint8_t *src)
{
  const struct openssl_cipher_ctx *ctx = p;
  int len;
  int ret = EVP_EncryptUpdate(ctx->evp, dst, &len, src, length);
  assert(ret == 1);
}
static void
openssl_evp_decrypt(const void *p, size_t length,
		    uint8_t *dst, const uint8_t *src)
{
  const struct openssl_cipher_ctx *ctx = p;
  int len;
  int ret = EVP_DecryptUpdate(ctx->evp, dst, &len, src, length);
  assert(ret == 1);
}

static void
openssl_evp_set_nonce(void *p, const uint8_t *nonce)
{
  const struct openssl_cipher_ctx *ctx = p;
  int ret = EVP_CipherInit_ex(ctx->evp, NULL, NULL, NULL, nonce, -1);
  assert(ret == 1);
}

static void
openssl_evp_update(void *p, size_t length, const uint8_t *src)
{
  const struct openssl_cipher_ctx *ctx = p;
  int len;
  int ret = EVP_EncryptUpdate(ctx->evp, NULL, &len, src, length);
  assert(ret == 1);
}

/* This will work for encryption only! */
static void
openssl_evp_gcm_digest(void *p, uint8_t *dst)
{
  const struct openssl_cipher_ctx *ctx = p;
  int ret = EVP_CIPHER_CTX_ctrl(ctx->evp, EVP_CTRL_GCM_GET_TAG, 16, dst);
  assert(ret == 1);
}

static void
openssl_evp_aead_encrypt(void *p, size_t length,
			 uint8_t *dst, const uint8_t *src)
{
  const struct openssl_cipher_ctx *ctx = p;
  int len;
  int ret = EVP_EncryptUpdate(ctx->evp, dst, &len, src, length);
  assert(ret == 1);
}

static void
openssl_evp_aead_decrypt(void *p, size_t length,
			 uint8_t *dst, const uint8_t *src)
{
  const struct openssl_cipher_ctx *ctx = p;
  int len;
  int ret = EVP_DecryptUpdate(ctx->evp, dst, &len, src, length);
  assert(ret == 1);
}

/* AES */
static nettle_set_key_func openssl_aes128_set_encrypt_key;
static nettle_set_key_func openssl_aes128_set_decrypt_key;
static nettle_set_key_func openssl_aes192_set_encrypt_key;
static nettle_set_key_func openssl_aes192_set_decrypt_key;
static nettle_set_key_func openssl_aes256_set_encrypt_key;
static nettle_set_key_func openssl_aes256_set_decrypt_key;

static void
openssl_aes128_set_encrypt_key(void *ctx, const uint8_t *key)
{
  openssl_evp_set_encrypt_key(ctx, key, EVP_aes_128_ecb());
}
static void
openssl_aes128_set_decrypt_key(void *ctx, const uint8_t *key)
{
  openssl_evp_set_decrypt_key(ctx, key, EVP_aes_128_ecb());
}

static void
openssl_aes192_set_encrypt_key(void *ctx, const uint8_t *key)
{
  openssl_evp_set_encrypt_key(ctx, key, EVP_aes_192_ecb());
}
static void
openssl_aes192_set_decrypt_key(void *ctx, const uint8_t *key)
{
  openssl_evp_set_decrypt_key(ctx, key, EVP_aes_192_ecb());
}

static void
openssl_aes256_set_encrypt_key(void *ctx, const uint8_t *key)
{
  openssl_evp_set_encrypt_key(ctx, key, EVP_aes_256_ecb());
}
static void
openssl_aes256_set_decrypt_key(void *ctx, const uint8_t *key)
{
  openssl_evp_set_decrypt_key(ctx, key, EVP_aes_256_ecb());
}

const struct nettle_cipher
nettle_openssl_aes128 = {
  "openssl aes128", sizeof(struct openssl_cipher_ctx),
  16, 16,
  openssl_aes128_set_encrypt_key, openssl_aes128_set_decrypt_key,
  openssl_evp_encrypt, openssl_evp_decrypt
};

const struct nettle_cipher
nettle_openssl_aes192 = {
  "openssl aes192", sizeof(struct openssl_cipher_ctx),
  16, 24,
  openssl_aes192_set_encrypt_key, openssl_aes192_set_decrypt_key,
  openssl_evp_encrypt, openssl_evp_decrypt
};

const struct nettle_cipher
nettle_openssl_aes256 = {
  "openssl aes256", sizeof(struct openssl_cipher_ctx),
  16, 32,
  openssl_aes256_set_encrypt_key, openssl_aes256_set_decrypt_key,
  openssl_evp_encrypt, openssl_evp_decrypt
};

/* AES-GCM */
static void
openssl_gcm_aes128_set_encrypt_key(void *ctx, const uint8_t *key)
{
  openssl_evp_set_encrypt_key(ctx, key, EVP_aes_128_gcm());
}
static void
openssl_gcm_aes128_set_decrypt_key(void *ctx, const uint8_t *key)
{
  openssl_evp_set_decrypt_key(ctx, key, EVP_aes_128_gcm());
}

static void
openssl_gcm_aes192_set_encrypt_key(void *ctx, const uint8_t *key)
{
  openssl_evp_set_encrypt_key(ctx, key, EVP_aes_192_gcm());
}
static void
openssl_gcm_aes192_set_decrypt_key(void *ctx, const uint8_t *key)
{
  openssl_evp_set_decrypt_key(ctx, key, EVP_aes_192_gcm());
}

static void
openssl_gcm_aes256_set_encrypt_key(void *ctx, const uint8_t *key)
{
  openssl_evp_set_encrypt_key(ctx, key, EVP_aes_256_gcm());
}
static void
openssl_gcm_aes256_set_decrypt_key(void *ctx, const uint8_t *key)
{
  openssl_evp_set_decrypt_key(ctx, key, EVP_aes_256_gcm());
}

const struct nettle_aead
nettle_openssl_gcm_aes128 = {
  "openssl gcm_aes128", sizeof(struct openssl_cipher_ctx),
  16, 16, 12, 16,
  openssl_gcm_aes128_set_encrypt_key, openssl_gcm_aes128_set_decrypt_key,
  openssl_evp_set_nonce, openssl_evp_update,
  openssl_evp_aead_encrypt, openssl_evp_aead_decrypt,
  openssl_evp_gcm_digest
};

const struct nettle_aead
nettle_openssl_gcm_aes192 = {
  "openssl gcm_aes192", sizeof(struct openssl_cipher_ctx),
  16, 24, 12, 16,
  openssl_gcm_aes192_set_encrypt_key, openssl_gcm_aes192_set_decrypt_key,
  openssl_evp_set_nonce, openssl_evp_update,
  openssl_evp_aead_encrypt, openssl_evp_aead_decrypt,
  openssl_evp_gcm_digest
};

const struct nettle_aead
nettle_openssl_gcm_aes256 = {
  "openssl gcm_aes256", sizeof(struct openssl_cipher_ctx),
  16, 32, 12, 16,
  openssl_gcm_aes256_set_encrypt_key, openssl_gcm_aes256_set_decrypt_key,
  openssl_evp_set_nonce, openssl_evp_update,
  openssl_evp_aead_encrypt, openssl_evp_aead_decrypt,
  openssl_evp_gcm_digest
};


/* Hash functions */

static void
openssl_hash_update(void *p,
		    size_t length,
		    const uint8_t *src)
{
  struct openssl_hash_ctx *ctx = p;
  EVP_DigestUpdate(ctx->evp, src, length);
}

#define OPENSSL_HASH(NAME, name)					\
static void								\
openssl_##name##_init(void *p)						\
{									\
  struct openssl_hash_ctx *ctx = p;					\
  if ((ctx->evp = EVP_MD_CTX_new()) == NULL)				\
    return;								\
									\
  EVP_DigestInit(ctx->evp, EVP_##name());				\
}									\
									\
static void								\
openssl_##name##_digest(void *p, uint8_t *dst)				\
{									\
  struct openssl_hash_ctx *ctx = p;					\
									\
  EVP_DigestFinal(ctx->evp, dst, NULL);					\
  EVP_DigestInit(ctx->evp, EVP_##name());				\
}									\
									\
const struct nettle_hash						\
nettle_openssl_##name = {						\
  "openssl " #name, sizeof(struct openssl_hash_ctx),			\
  NAME##_DIGEST_SIZE, NAME##_BLOCK_SIZE,				\
  openssl_##name##_init,						\
  openssl_hash_update,							\
  openssl_##name##_digest						\
};

OPENSSL_HASH(MD5, md5)
OPENSSL_HASH(SHA1, sha1)

#endif /* WITH_OPENSSL */
