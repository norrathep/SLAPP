(1) To measure stack usage: make sure #define BENCHMARK_STACK in secure.c is not commented out
(2) To use HMAC-SHA256: 
	- In secure.c comment #define CRYPTO_SPHINCS and #define CRYPTO_ED25519 out and leave #define CRYPTO_HMAC_SHA256 in.
	- Also in main.c for Non-Secure World, comment #define CRYPTO_ED25519 out and leave #define CRYPTO_HMAC_SHA256 in.
	- In Verifier/main.py, set CRYPTO = CRYPTO_HMAC_SHA256
(3) To use ED25519: similar to (2) but change to ED25519
(4) To use SPHNICS+
	- In Verifier/main.py, set CRYPTO = CRYPTO_HMAC_SHA256 	(this dont matter but has to match what comes below)
	- Also in main.c for Non-Secure World, comment #define CRYPTO_ED25519 out and leave #define CRYPTO_HMAC_SHA256 in.
	- In secure.c use only #define CRYPTO_SPHINCS
	- In Verifier/main.py, change the file: './sigWrapperTest85bc.txt' to correspond to the signature when wrapperTest (in Non-Secure World) moves
	- To get a signature, in Non-Secure World's main.c, uncomment code block in between "TODO: Sphincs" and set "sign" to 1. In main.py, make sure "counter" ,"f" and "inp" in genRawRequest() is set accordingly.
	- To verify signature on the device, set "sign" to 0