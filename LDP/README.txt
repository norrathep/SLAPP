Code structure
- Verifier/ldp.py: Verifier's implementation using Python
- saml11-demo/SecureApp: SLAPP's TCB implemented using symmetric-key cryptography
- saml11-demo/NonSecureApp: Normal application implementing energy reading retrieved from PZEM-004T sensor
- ESP32-S2/pzem/pzem.ino: ESP32-S2's device's code. This ESP32-S2 acts as a bridge for PZEM-004T and SAML11 communication since SAML11 only has one UART hardware and PZEM-004T communication has to be done over serial with different serial parameters.

Hardware setup:

PZEM-004T <-> ESP32-S2 <-> SAML11 Xplained Pro <-> Verifier, where <-> denotes serial communication

- Upload pzem.ino into ESP32-S2, using Arduino IDE (you may need to install esp32 libraries on Arduino IDE in order to build it)
- Open ldp.atsln (only tested on Windows), which will be opened using Microchip Studio. Then, click Build -> Build Solution (F7). 
- Open and run "ldp.py"

Benign execution:

- First, make sure PZEM-004T sensor is powered on and connected to ESP32-S2. Also, connect ESP32-S2 to SAML11, which also is connected to the computer. 
Do not connect ESP32-S2 to a power source now.
- After running "ldp.py", in Microchip Studio, click "Start Without Debugging (Ctrl+Alt+F5)", which will start running software on SAML11.
- In ldp.py, enter "i", performing "Setup" phase as described in the paper
- Next, in ldp.py, enter "c" to perform "Collect" phase. After seeing "Plug in Arduino to proceed", plug ESP32-S2 to a power source.
- After seeing "You have 10 secs to unplug Arduino", unplug ESP32-S2
- Both of these phases should show "Token verification succeeds"
- "Collect" phase can be repeated as many times as you want.

Malicious execution:

We simulate 4 poisoning attack scenarios in LDP in this folder.

1) A1: poisoning the initiali state when "Setup" phase is executed. To simulate A1, uncomment "#define ATTACK_A1" in "saml11-demo/NonSecureApp/main.c" and rebuild and reupload the binary. Then, repeat the process as if it's running benign execution. ldp.py should throw an error in "Setup" phase.
2) A2: performing a poisoning attack by compromising the prover after Setup phase completes, with the goal of directly manipulating raw energy data received from PZEM-004T. To simulate this, first, we execute "Setup" phase (by entering "i") using benign SAML11's binary. After the "Setup" phase completes, uncomment "#define ATTACK_A2" (make sure other attacks are commented out). Then, rebuild and reupload SAML11's binary and execute "Collect" phase (by entering "c"). ldp.py should throw an error in "Collect" phase.
3) A3: performing a poisoning attack by compromising the prover after "Setup" phase completes, with the goal of manipulating the state variable (Map B). Do the same steps as A2 but instead uncomment "#define ATTACK_A3". It should raise an exception.
4) A4: performing a poisoning attack by compromising the prover after a proof is generated in "Collect" phase, with the goal of manipulating noisy energy data. In this attack, we simulate a network adversary, which modifies the noisy output during transmission. Thus, in SAML11, run the benign code but in ldp.py, set: "ATTACK_A4 = True". It should raise an exception.

