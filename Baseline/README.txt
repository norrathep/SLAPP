Code structure
- Verifier/main.py: Verifier's implementation using Python
- saml11-demo/SecureApp: SLAPP's TCB
- saml11-demo/NonSecureApp: Normal applications 

Hardware setup:

SAML11 Xplained Pro <-> Verifier, where <-> denotes serial communication

- Upload pzem.ino into ESP32-S2, using Arduino IDE (you may need to install esp32 libraries on Arduino IDE in order to build it)
- Open measure.atsln (only tested on Windows), which will be opened using Microchip Studio. Then, click Build -> Build Solution (F7). 
- Open and run "main.py"

Running instruction:

- After running "main.py", in Microchip Studio, click "Start Without Debugging (Ctrl+Alt+F5)", which will start running software on SAML11.

SLAPP_SK:

- To use SLAPP_SK, in "main.py", set CRYPTO to CRYPTO_HMAC_SHA256
- In ./NonSecureApp/main.c, uncomment "#define CRYPTO_HMAC_SHA256" and comment out the rest
- In ./SecureApp/Secure_Functions/secure.c, uncomment "#define CRYPTO_HMAC_SHA256" and comment out the rest
- To measure stack usage, uncomment "#define BENCHMARK_STACK". Also, to be sure, put a breakpoint inside stack_used() (in secure.c) to monitor stack usage (stack area that is not 0xd9 is considered used) -- Also, make sure to adjust STACK_BASE to the starting stack address in NonSecureApp.lss

SLAPP_PK:

- Do similarly as SLAPP_SK but look for CRYPTO_ECC instead

SLAPP_PQ:

- in "main.py", set CRYPTO to CRYPTO_HMAC_SHA256 (not a typo here)
- In ./NonSecureApp/main.c, uncomment "#define CRYPTO_SPHINCS" and comment out the rest
- In ./SecureApp/Secure_Functions/secure.c, uncomment "#define CRYPTO_SPHINCS" and comment out the rest
- This crypto streams out the signature in chunks!
