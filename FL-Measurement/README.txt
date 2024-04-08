Code structure
- Verifier/fl.py: Verifier's implementation using Python
- saml11-demo/SecureApp: SLAPP's TCB implemented using symmetric-key cryptography
- saml11-demo/NonSecureApp: Normal application implementing energy reading retrieved from PZEM-004T sensor
- ESP32-S2/pzem/pzem.ino: ESP32-S2's device's code. This ESP32-S2 acts as a bridge for PZEM-004T and SAML11 communication since SAML11 only has one UART hardware and PZEM-004T communication has to be done over serial with different serial parameters.

Hardware setup:

PZEM-004T <-> ESP32-S2 <-> SAML11 Xplained Pro <-> Verifier, where <-> denotes serial communication

- Upload pzem.ino into ESP32-S2, using Arduino IDE (you may need to install esp32 libraries on Arduino IDE in order to build it)
- Open fl-measure.atsln (only tested on Windows), which will be opened using Microchip Studio. Then, click Build -> Build Solution (F7). 
- Open and run "fl.py"

Benign execution:

- First, make sure PZEM-004T sensor is powered on and connected to ESP32-S2. Also, connect ESP32-S2 to SAML11, which also is connected to the computer. 
Do not connect ESP32-S2 to a power source now.
- After running "fl.py", in Microchip Studio, click "Start Without Debugging (Ctrl+Alt+F5)", which will start running software on SAML11.
- In fl.py, enter "i", performing "Setup" phase as described in the paper
- Next, in fl.py, enter "c" to perform "Collection" phase. After seeing "Plug in Arduino to proceed", plug ESP32-S2 to a power source.
- After seeing "You have 10 secs to unplug Arduino", unplug ESP32-S2
- To train a model, enter "t" in fl.py
- All phases should show "Token verification succeeds"
- "Collection" phase can be repeated as many times as you want.



