/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*  GMime
 *  Copyright (C) 2000-2009 Jeffrey Stedfast
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
 *  02110-1301, USA.
 */


#ifndef __GMIME_CIPHER_CONTEXT_H__
#define __GMIME_CIPHER_CONTEXT_H__

#include <glib.h>
#include <glib-object.h>

#include <time.h>

#include <gmime/gmime-stream.h>

G_BEGIN_DECLS

#define GMIME_TYPE_CIPHER_CONTEXT            (g_mime_cipher_context_get_type ())
#define GMIME_CIPHER_CONTEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMIME_TYPE_CIPHER_CONTEXT, GMimeCipherContext))
#define GMIME_CIPHER_CONTEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GMIME_TYPE_CIPHER_CONTEXT, GMimeCipherContextClass))
#define GMIME_IS_CIPHER_CONTEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMIME_TYPE_CIPHER_CONTEXT))
#define GMIME_IS_CIPHER_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GMIME_TYPE_CIPHER_CONTEXT))
#define GMIME_CIPHER_CONTEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GMIME_TYPE_CIPHER_CONTEXT, GMimeCipherContextClass))

typedef struct _GMimeCipherContext GMimeCipherContext;
typedef struct _GMimeCipherContextClass GMimeCipherContextClass;

typedef struct _GMimeSigner GMimeSigner;
typedef struct _GMimeSignatureValidity GMimeSignatureValidity;


/**
 * GMimePasswordRequestFunc:
 * @ctx: the #GMimeCipherContext making the request
 * @user_id: the user_id of the password being requested
 * @prompt_ctx: a string containing some helpful context for the prompt
 * @reprompt: %TRUE if this password request is a reprompt due to a previously bad password response
 * @response: a stream for the application to write the password to (followed by a newline '\n' character)
 * @err: a #GError for the callback to set if an error occurs
 *
 * A password request callback allowing a #GMimeCipherContext to
 * prompt the user for a password for a given key.
 *
 * Returns: %TRUE on success or %FALSE on error.
 **/
typedef gboolean (* GMimePasswordRequestFunc) (GMimeCipherContext *ctx, const char *user_id, const char *prompt_ctx,
					       gboolean reprompt, GMimeStream *response, GError **err);


/**
 * GMimeCipherHash:
 * @GMIME_CIPHER_HASH_DEFAULT: The default hash algorithm.
 * @GMIME_CIPHER_HASH_MD2: The MD2 hash algorithm.
 * @GMIME_CIPHER_HASH_MD5: The MD5 hash algorithm.
 * @GMIME_CIPHER_HASH_SHA1: The SHA-1 hash algorithm.
 * @GMIME_CIPHER_HASH_SHA224: The SHA-224 hash algorithm.
 * @GMIME_CIPHER_HASH_SHA256: The SHA-256 hash algorithm.
 * @GMIME_CIPHER_HASH_SHA384: The SHA-384 hash algorithm.
 * @GMIME_CIPHER_HASH_SHA512: The SHA-512 hash algorithm.
 * @GMIME_CIPHER_HASH_RIPEMD160: The RIPEMD-160 hash algorithm.
 * @GMIME_CIPHER_HASH_TIGER192: The TIGER-192 hash algorithm.
 * @GMIME_CIPHER_HASH_HAVAL5160: The HAVAL5-160 hash algorithm.
 *
 * A hash algorithm.
 **/
typedef enum {
	GMIME_CIPHER_HASH_DEFAULT,
	GMIME_CIPHER_HASH_MD2,
	GMIME_CIPHER_HASH_MD5,
	GMIME_CIPHER_HASH_SHA1,
	GMIME_CIPHER_HASH_SHA224,
	GMIME_CIPHER_HASH_SHA256,
	GMIME_CIPHER_HASH_SHA384,
	GMIME_CIPHER_HASH_SHA512,
	GMIME_CIPHER_HASH_RIPEMD160,
	GMIME_CIPHER_HASH_TIGER192,
	GMIME_CIPHER_HASH_HAVAL5160
} GMimeCipherHash;


/**
 * GMimeCipherContext:
 * @parent_object: parent #GObject
 * @request_passwd: a callback for requesting a password
 * @sign_protocol: signature protocol (must be set by subclass)
 * @encrypt_protocol: encryption protocol (must be set by subclass)
 * @key_protocol: key exchange protocol (must be set by subclass)
 *
 * A crypto context for use with MIME.
 **/
struct _GMimeCipherContext {
	GObject parent_object;
	
	GMimePasswordRequestFunc request_passwd;
	
	/* these must be set by the subclass in the instance_init() */
	const char *sign_protocol;
	const char *encrypt_protocol;
	const char *key_protocol;
};

struct _GMimeCipherContextClass {
	GObjectClass parent_class;
	
	GMimeCipherHash          (* hash_id)     (GMimeCipherContext *ctx, const char *hash);
	
	const char *             (* hash_name)   (GMimeCipherContext *ctx, GMimeCipherHash hash);
	
	int                      (* sign)        (GMimeCipherContext *ctx, const char *userid,
						  GMimeCipherHash hash, GMimeStream *istream,
						  GMimeStream *ostream, GError **err);
	
	GMimeSignatureValidity * (* verify)      (GMimeCipherContext *ctx, GMimeCipherHash hash,
						  GMimeStream *istream, GMimeStream *sigstream,
						  GError **err);
	
	int                      (* encrypt)     (GMimeCipherContext *ctx, gboolean sign,
						  const char *userid, GPtrArray *recipients,
						  GMimeStream *istream, GMimeStream *ostream,
						  GError **err);
	
	GMimeSignatureValidity * (* decrypt)     (GMimeCipherContext *ctx, GMimeStream *istream,
						  GMimeStream *ostream, GError **err);
	
	int                      (* import_keys) (GMimeCipherContext *ctx, GMimeStream *istream,
						  GError **err);
	
	int                      (* export_keys) (GMimeCipherContext *ctx, GPtrArray *keys,
						  GMimeStream *ostream, GError **err);
};


GType g_mime_cipher_context_get_type (void);

void g_mime_cipher_context_set_request_password (GMimeCipherContext *ctx, GMimePasswordRequestFunc request_passwd);

/* hash routines */
GMimeCipherHash      g_mime_cipher_context_hash_id (GMimeCipherContext *ctx, const char *hash);

const char *         g_mime_cipher_context_hash_name (GMimeCipherContext *ctx, GMimeCipherHash hash);

/* cipher routines */
int                  g_mime_cipher_context_sign (GMimeCipherContext *ctx, const char *userid,
						 GMimeCipherHash hash, GMimeStream *istream,
						 GMimeStream *ostream, GError **err);

GMimeSignatureValidity *g_mime_cipher_context_verify (GMimeCipherContext *ctx, GMimeCipherHash hash,
						      GMimeStream *istream, GMimeStream *sigstream,
						      GError **err);

int                  g_mime_cipher_context_encrypt (GMimeCipherContext *ctx, gboolean sign,
						    const char *userid, GPtrArray *recipients,
						    GMimeStream *istream, GMimeStream *ostream,
						    GError **err);

GMimeSignatureValidity *g_mime_cipher_context_decrypt (GMimeCipherContext *ctx, GMimeStream *istream,
						       GMimeStream *ostream, GError **err);

/* key/certificate routines */
int                  g_mime_cipher_context_import_keys (GMimeCipherContext *ctx, GMimeStream *istream,
							GError **err);

int                  g_mime_cipher_context_export_keys (GMimeCipherContext *ctx, GPtrArray *keys,
							GMimeStream *ostream, GError **err);


/* signature status structures and functions */

/**
 * GMimeSignerTrust:
 * @GMIME_SIGNER_TRUST_NONE: No trust assigned.
 * @GMIME_SIGNER_TRUST_NEVER: Never trust this signer.
 * @GMIME_SIGNER_TRUST_UNDEFINED: Undefined trust for this signer.
 * @GMIME_SIGNER_TRUST_MARGINAL: Trust this signer maginally.
 * @GMIME_SIGNER_TRUST_FULLY: Trust this signer fully.
 * @GMIME_SIGNER_TRUST_ULTIMATE: Trust this signer ultimately.
 *
 * The trust value of a signer.
 **/
typedef enum {
	GMIME_SIGNER_TRUST_NONE,
	GMIME_SIGNER_TRUST_NEVER,
	GMIME_SIGNER_TRUST_UNDEFINED,
	GMIME_SIGNER_TRUST_MARGINAL,
	GMIME_SIGNER_TRUST_FULLY,
	GMIME_SIGNER_TRUST_ULTIMATE
} GMimeSignerTrust;


/**
 * GMimeSignerStatus:
 * @GMIME_SIGNER_STATUS_NONE: No status.
 * @GMIME_SIGNER_STATUS_GOOD: Good signature.
 * @GMIME_SIGNER_STATUS_BAD: Bad signature.
 * @GMIME_SIGNER_STATUS_ERROR: An error occurred.
 *
 * A value representing the signature status for a particular
 * #GMimeSigner.
 **/
typedef enum {
	GMIME_SIGNER_STATUS_NONE,
	GMIME_SIGNER_STATUS_GOOD,
	GMIME_SIGNER_STATUS_BAD,
	GMIME_SIGNER_STATUS_ERROR
} GMimeSignerStatus;


/**
 * GMimeSignerError:
 * @GMIME_SIGNER_ERROR_NONE: No error.
 * @GMIME_SIGNER_ERROR_EXPSIG: Expired signature.
 * @GMIME_SIGNER_ERROR_NO_PUBKEY: No public key found.
 * @GMIME_SIGNER_ERROR_EXPKEYSIG: Expired signature key.
 * @GMIME_SIGNER_ERROR_REVKEYSIG: Revoked signature key.
 *
 * Possible errors that a #GMimeSigner could have.
 **/
typedef enum {
	GMIME_SIGNER_ERROR_NONE,
	GMIME_SIGNER_ERROR_EXPSIG      = (1 << 0),  /* expired signature */
	GMIME_SIGNER_ERROR_NO_PUBKEY   = (1 << 1),  /* no public key */
	GMIME_SIGNER_ERROR_EXPKEYSIG   = (1 << 2),  /* expired key */
	GMIME_SIGNER_ERROR_REVKEYSIG   = (1 << 3)   /* revoked key */
} GMimeSignerError;


/**
 * GMimeSigner:
 * @next: Pointer to the next #GMimeSigner.
 * @status: A #GMimeSignerStatus.
 * @errors: A bitfield of #GMimeSignerError values.
 * @trust: A #GMimeSignerTrust.
 * @unused: Unused expansion bits for future use; ignore this.
 * @issuer_serial: The issuer of the certificate if known.
 * @issuer_name: The issuer of the certificate if known.
 * @fingerprint: A hex string representing the signer's fingerprint.
 * @sig_created: The creation date of the signature.
 * @sig_expires: The expiration date of the signature.
 * @key_created: The creation date of the signature key.
 * @key_expires: The expiration date of the signature key.
 * @keyid: The signer's key id.
 * @email: The email address of the person or entity.
 * @name: The name of the person or entity.
 *
 * A structure containing useful information about a signer.
 **/
struct _GMimeSigner {
	GMimeSigner *next;
	unsigned int status:2;    /* GMimeSignerStatus */
	unsigned int errors:4;    /* bitfield of GMimeSignerError's */
	unsigned int trust:3;     /* GMimeSignerTrust */
	unsigned int unused:21;   /* unused expansion bits */
	char *issuer_serial;
	char *issuer_name;
	char *fingerprint;
	time_t sig_created;
	time_t sig_expires;
	time_t key_created;
	time_t key_expires;
	char *keyid;
	char *email;
	char *name;
};


GMimeSigner *g_mime_signer_new (void);
void         g_mime_signer_free (GMimeSigner *signer);

GMimeSigner *g_mime_signer_next (GMimeSigner *signer);

void g_mime_signer_set_status (GMimeSigner *signer, GMimeSignerStatus status);
GMimeSignerStatus g_mime_signer_get_status (GMimeSigner *signer);

void g_mime_signer_set_errors (GMimeSigner *signer, GMimeSignerError error);
GMimeSignerError g_mime_signer_get_errors (GMimeSigner *signer);

void g_mime_signer_set_trust (GMimeSigner *signer, GMimeSignerTrust trust);
GMimeSignerTrust g_mime_signer_get_trust (GMimeSigner *signer);

void g_mime_signer_set_issuer_serial (GMimeSigner *signer, const char *issuer_serial);
const char *g_mime_signer_get_issuer_serial (GMimeSigner *signer);

void g_mime_signer_set_issuer_name (GMimeSigner *signer, const char *issuer_name);
const char *g_mime_signer_get_issuer_name (GMimeSigner *signer);

void g_mime_signer_set_fingerprint (GMimeSigner *signer, const char *fingerprint);
const char *g_mime_signer_get_fingerprint (GMimeSigner *signer);

void g_mime_signer_set_key_id (GMimeSigner *signer, const char *key_id);
const char *g_mime_signer_get_key_id (GMimeSigner *signer);

void g_mime_signer_set_email (GMimeSigner *signer, const char *email);
const char *g_mime_signer_get_email (GMimeSigner *signer);

void g_mime_signer_set_name (GMimeSigner *signer, const char *name);
const char *g_mime_signer_get_name (GMimeSigner *signer);

void g_mime_signer_set_sig_created (GMimeSigner *signer, time_t created);
time_t g_mime_signer_get_sig_created (GMimeSigner *signer);

void g_mime_signer_set_sig_expires (GMimeSigner *signer, time_t expires);
time_t g_mime_signer_get_sig_expires (GMimeSigner *signer);

void g_mime_signer_set_key_created (GMimeSigner *signer, time_t created);
time_t g_mime_signer_get_key_created (GMimeSigner *signer);

void g_mime_signer_set_key_expires (GMimeSigner *signer, time_t expires);
time_t g_mime_signer_get_key_expires (GMimeSigner *signer);


/**
 * GMimeSignatureStatus:
 * @GMIME_SIGNATURE_STATUS_NONE: No status.
 * @GMIME_SIGNATURE_STATUS_GOOD: Good signature.
 * @GMIME_SIGNATURE_STATUS_BAD: Bad signature.
 * @GMIME_SIGNATURE_STATUS_UNKNOWN: Unknown signature status.
 *
 * The status of a message signature.
 **/
typedef enum {
	GMIME_SIGNATURE_STATUS_NONE,
	GMIME_SIGNATURE_STATUS_GOOD,
	GMIME_SIGNATURE_STATUS_BAD,
	GMIME_SIGNATURE_STATUS_UNKNOWN
} GMimeSignatureStatus;


/**
 * GMimeSignatureValidity:
 * @status: The overall signature status.
 * @signers: A list of #GMimeSigner structures.
 * @details: A string containing more user-readable details.
 *
 * A structure containing information about the signature validity of
 * a signed stream.
 **/
struct _GMimeSignatureValidity {
	GMimeSignatureStatus status;
	GMimeSigner *signers;
	char *details;
};


GMimeSignatureValidity *g_mime_signature_validity_new (void);
void                    g_mime_signature_validity_free (GMimeSignatureValidity *validity);

GMimeSignatureStatus    g_mime_signature_validity_get_status (const GMimeSignatureValidity *validity);
void                    g_mime_signature_validity_set_status (GMimeSignatureValidity *validity, GMimeSignatureStatus status);

const char             *g_mime_signature_validity_get_details (const GMimeSignatureValidity *validity);
void                    g_mime_signature_validity_set_details (GMimeSignatureValidity *validity, const char *details);

const GMimeSigner      *g_mime_signature_validity_get_signers (const GMimeSignatureValidity *validity);
void                    g_mime_signature_validity_add_signer  (GMimeSignatureValidity *validity, GMimeSigner *signer);

G_END_DECLS

#endif /* __GMIME_CIPHER_CONTEXT_H__ */
