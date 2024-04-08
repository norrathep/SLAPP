/*
 * oak_sphincs.c
 *
 * Created: 2/15/2024 3:50:49 PM
 *  Author: Oak
 */ 
#include "oak_sphincs.h"

struct v {
	const char *parameter_set_name; /* Name of the parameter set */
	unsigned char public_key[64];   /* The public key that is generated */
	/* with the fixed seed */
	unsigned char optrand[32];      /* The optrand that was used when */
	/* creating the signature */
	unsigned char hash_sig[32];     /* The SHA256 hash of the signature */
	/* of the message "abc", using the */
	/* optrand and generated private key */
};

int fixed_rand( unsigned char *p, size_t num_bytes ) {
	for (unsigned i=0; i<num_bytes; i++) {
		*p++ = i;
	}
	return 1;
}

unsigned char optrand_buffer[32];
int optrand_rng( unsigned char *target, size_t num_bytes ) {
	memcpy( target, optrand_buffer, num_bytes );
	return 1;
}
uint8_t buffer[ 1024 ];
unsigned numSigs = 0;
unsigned get_sig() {
	unsigned n = 0;
	int rem = 17088-numSigs;
	if(rem > sizeof buffer) n = sizeof buffer;
	else n = rem;
	numSigs += n;
	uart_get_bytes(buffer, n);
	return n;
}

extern void print_token(uint8_t *token, int size);

uint8_t optrand[32] = { 0x9e, 0x9c, 0x88, 0x01, 0xbb, 0x10, 0x31, 0x83,
0xbf, 0xd6, 0x2a, 0xc9, 0xed, 0x12, 0x65, 0x1f};

/* Generate the public/private key pair */
unsigned char private_key[64] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
	0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
	0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x3b, 0x56, 0xe8, 0x16, 0x84, 0x7f, 0x00, 0x03,
0x86, 0xae, 0xec, 0x2e, 0x2b, 0xb9, 0xe1, 0xb5};
unsigned char public_key[32] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x3b, 0x56, 0xe8, 0x16, 0x84, 0x7f, 0x00, 0x03,
0x86, 0xae, 0xec, 0x2e, 0x2b, 0xb9, 0xe1, 0xb5};

void oakSphincVerify(uint8_t *message, int msgsize) {

	/* Get the parameter set */
	const struct ts_parameter_set *ps = &ts_ps_sha2_128f_simple;
	if (!ps) return 0;
		
	memcpy( optrand_buffer, optrand, 32 );

	/* And, while we're doing that, verify the signature - no reason not */
	/* to test the verify logic at the same time */
	struct ts_context verify_ctx;
	ts_init_verify( &verify_ctx, message, msgsize, ps, public_key );

	
	uart_puts("In verify, Sig:\n");
	for (;;) {
		uart_puts("READ\n");
		
		unsigned n = get_sig();
		if (n == 0) {
			break;
		}
		
		for(int ii=0; ii<n; ii++) {
			char hex[3]={0};
			sprintf(hex, "%02x", buffer[ii]);
			uart_puts(hex);
		}
		uart_puts("\n");

		if (1 != ts_update_verify( buffer, n, &verify_ctx )) {
			uart_puts( "*** VERIFY DETECTED FAILURE ***\n");
			return 0;
		}
	}
	uart_puts("Done!\n");

	if (1 != ts_verify( &verify_ctx )) {
		uart_puts( "*** SIGNATURE DID NOT VERIFY ***\n");
		return 0;
	}
}

void oakSphincSign(uint8_t *message, int msgsize) {

	/* Get the parameter set */
	const struct ts_parameter_set *ps = &ts_ps_sha2_128f_simple;
	if (!ps) return 0;

	memcpy( optrand_buffer, optrand, 32 );

	/* And sign the message; while we're generating the signature */
	/* (in pieces), hash it */
	struct ts_context ctx;
	ts_init_sign( &ctx, message, msgsize, ps, private_key, optrand_rng );

	
	uart_puts("In signing, Sig:\n");
	for (;;) {
		
		unsigned n = ts_sign( buffer, sizeof buffer, &ctx );
		if (n == 0) {
			break;
		}
		
		for(int ii=0; ii<n; ii++) {
			char hex[3]={0};
			sprintf(hex, "%02x", buffer[ii]);
			uart_puts(hex);
		}
		uart_puts("\n");
	}
	uart_puts("Done!\n");
}



void oakSphinc(uint8_t *message, int msgsize) {
	uart_puts("Generating the key\n");
	struct v vv = {
		"sha2_128f_simple",
		{ 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
			0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
			0x3b, 0x56, 0xe8, 0x16, 0x84, 0x7f, 0x00, 0x03,
		0x86, 0xae, 0xec, 0x2e, 0x2b, 0xb9, 0xe1, 0xb5, },
		{ 0x9e, 0x9c, 0x88, 0x01, 0xbb, 0x10, 0x31, 0x83,
		0xbf, 0xd6, 0x2a, 0xc9, 0xed, 0x12, 0x65, 0x1f, },
		{ 0x64, 0x18, 0x40, 0xfb, 0xae, 0x50, 0x52, 0x29,
			0x31, 0xc6, 0x79, 0xcb, 0xc2, 0x71, 0xc5, 0x92,
			0x2c, 0x54, 0xef, 0xd0, 0xab, 0xd8, 0x0a, 0xa3,
		0xe1, 0x1e, 0xd4, 0xea, 0x4f, 0x4e, 0x10, 0x8a, }
	};
	struct v *v = &vv;

	/* Get the parameter set */
	const struct ts_parameter_set *ps = &ts_ps_sha2_128f_simple;
	if (!ps) return 0;

	/*uart_puts("Generating pub/priv keys\n");
	if (!ts_gen_key( private_key, public_key, ps, fixed_rand )) {
		uart_puts( "*** ERROR GENERATING KEY\n" );
		return 0;
	}
	uart_puts("Complete\n");

	unsigned len_public_key = ts_size_public_key( ps );
	if (0 != memcmp( v->public_key, public_key, len_public_key )) {
		uart_puts( "*** GENERATED DIFFERENT PUBLIC KEY FOR %s\n");
		return 0;
	}*/

	memcpy( optrand_buffer, optrand, 32 );

	/* And sign the message; while we're generating the signature */
	/* (in pieces), hash it */
	struct ts_context ctx;
	ts_init_sign( &ctx, message, msgsize, ps, private_key, optrand_rng );
	SHA256_CTX hash_ctx;
	ts_SHA256_init( &hash_ctx );

	/* And, while we're doing that, verify the signature - no reason not */
	/* to test the verify logic at the same time */
	struct ts_context verify_ctx;
	ts_init_verify( &verify_ctx, message, msgsize, ps, public_key );

	
	uart_puts("Sig:\n");
	for (;;) {
		uart_puts("READ\n");
		
		//unsigned n = ts_sign( buffer, sizeof buffer, &ctx );
		unsigned n = get_sig();
		if (n == 0) {
			//uart_puts("0 bytes???\n");
			break;
		}
		//uart_puts("WRITE\n");
		
		for(int ii=0; ii<n; ii++) {
			char hex[3]={0};
			sprintf(hex, "%02x", buffer[ii]);
			uart_puts(hex);
		}
		uart_puts("\n");

		ts_SHA256_update( &hash_ctx, buffer, n );

		if (1 != ts_update_verify( buffer, n, &verify_ctx )) {
			uart_puts( "*** VERIFY DETECTED FAILURE ***\n");
			uart_puti(n);
			uart_puts("\n");
			//return 0;
			break;
		}
	}
	uart_puts("Done!\n");

	unsigned char hash[32];
	ts_SHA256_final( hash, &hash_ctx );

	/*if (0 != memcmp( v->hash_sig, hash, 32 )) {
		uart_puts( "*** GENERATED DIFFERENT SGNATURES ***\n" );
		return 0;
	}*/

	if (1 != ts_verify( &verify_ctx )) {
		uart_puts( "*** SIGNATURE DID NOT VERIFY ***\n");
		return 0;
	}
	
	uart_puts("Sphincs+ PK: \n");
	print_token(public_key, 32);
	uart_puts("Sphincs+ SK: \n");
	print_token(private_key, 64);
}

