This project consists of several folders.

- Baseline: for measuring performance of the baseline approach 

- Measurement: for measuring performance of SLAPP

- LDP: for running a case study based on LDP using SLAPP_{SK}

- LDP-Baseline: for running a case study based on LDP using Baseline_{SK}

- FL: for running a case study based on Federated Learning using SLAPP_{SK}

- FL-Baseline: for running a case study based on Federated Learning using Baseline_{SK}

- Plot: for plotting all graphs in the paper

Hardware setup (for LDP/LDP-baseline/FL/FL-Baseline):

NUCLEO-L552ZE-Q <-> Verifier, where <-> denotes serial communication

Hardware setup (for Baseline/Measurement):

PZEM-004T <-> CucumberR <-> NUCLEO-L552ZE-Q <-> Verifier, where <-> denotes serial communication

For installation:

1) Download STM32CubeIDE, see this repo for how to set up NUCLEO-L552ZE-Q: https://github.com/RIT-CHAOS-SEC/ISC-FLAT_open_source
2) In STM32CubeIDE, File -> Import -> Existing Projects into Workspace -> Select the folder of interest
3) Click "TRACES_NonSecure (in NonSecure)" folder -> click "Run" (the play symbol surrounded by a green circle) -> Select Startup -> Add Secure counterpart
4) Each folder should come with "Verifier" folder with main.py. Run main.py (in my case I have to re-plug the USB cable for NUCLEO-L552ZE-Q after building/uploading firmware but before running main.py
5.1) For LDP case study, first connect CucumberR (ESP32-S2) to PZEM-004t (if not connected, CucumberR will send a default value of 22) where (RX, TX) = (16, 17) on CucumberR. CucumberR acts as an interface for NUCLEO-L552ZE-Q to read data from PZEM-004t. Technically, we do not need CucumberR and NUCLEO-L552ZE-Q can connect to PZEM-004t directly. But implementing the actual reading driver in CucumberR is easier and can be done using Arduino IDE.
5.2) To download CucumberR firmware, download "esp32" library (from the left toolbar of Arduino IDE) and perhaps Arduino AVR Boards as well. Then, open pzem.ino from LDP -> PZEM-interface and click build and upload the firmware. Connect CucumberR to NUCLEO-L552ZE-Q by connecting CucumberR's IO2 to PA2 and IO3 to PA3 (see ldp-setup.jpg)


LDP attack simulation:
1) A1: poisoning the initiali state when "Setup" phase is executed. To simulate A1, uncomment "#define ATTACK_A1" in "LDP/TRACES_NonSecure/Core/Src/main.c" and rebuild and reupload the binary. Then, repeat the process as if it's running benign execution. ldp.py should throw an error in "Setup" phase.
2) A2: performing a poisoning attack by compromising the prover after Setup phase completes, with the goal of directly manipulating raw energy data received from PZEM-004T. To simulate this, first, we execute "Setup" phase (by entering "i") using benign binary. After the "Setup" phase completes, uncomment "#define ATTACK_A2" (make sure other attacks are commented out). Then, rebuild and reupload the binary and execute "Collect" phase (by entering "c"). ldp.py should throw an error in "Collect" phase.
3) A3: performing a poisoning attack by compromising the prover after "Setup" phase completes, with the goal of manipulating the state variable (Map B). Do the same steps as A2 but instead uncomment "#define ATTACK_A3". It should raise an exception.
4) A4: performing a poisoning attack by compromising the prover after a proof is generated in "Collect" phase, with the goal of manipulating noisy energy data. In this attack, we simulate a network adversary, which modifies the noisy output during transmission. Thus, in SAML11, run the benign code but in ldp.py, set: "ATTACK_A4 = True". It should raise an exception.
NOTE: whenever you are updating the benign Non-Secure World's code, you must replace Verifier/TRACES_NonSecure.elf with the newly built binary (from NonSecure/Debug/)
NOTE: for PMEM size's experiment, in secure_nsc.c from TRACES_Secure/Core/Src, you might need to update "mem_len" to 0x40000 and mem to 0x80401f8

FL attack simulation:
1) AD: poisoning the training dataset before "Local Training" is executed. To simulate AD, after the "Setup" and "Collection" phases are complete, kill fl.py and uncomment "#define ATTACK_AD" in "TRACES_NonSecure/Core/Src/main.c" and rebuild and reupload the binary. Then, repeat the process as if it's running benign execution. fl.py should throw an error in "Setup" phase.
2) AM: performing a poisoning attack by compromising the prover after a proof is generated in "Local Training" phase, with the goal of manipulating model's parameters. In this attack, we simulate a network adversary, which modifies the noisy output during transmission. Thus, in NUCLEO, run the benign code but in fl.py, set: "ATTACK_AM = True". Repeat the process as if it's running benign execution. fl.py should raise an exception.

Known issue (TODO list):
- Apparently, NUCLEO-L552ZE-Q's serial driver is buggy. The energy value can be read only once and it stops working after that.
- I also could not figure out how to implement serial driver in Non-Secure World. So right now, it resides in Secure World but its code size is small, compared to other HAL code; it shouldnt affect much to the experimental results.
- Implementation of FL case study is now based on dummy data; incorporating with PZEM-004t (+CucumberR) is left to be done but it should not impact anything, experiment-wise.
- This README is horribly written (need to be updated).

Experiment methodology:

- Peak data allocated at runtime is computed from stack usage plus rodata, data and bss sections. Stack usage is estimated based on Static Stack Analyzer tool in STM32CubeIDE. For FL, it also includes heap allocation which was kept track using "heapSize" variable in NonSecure/Core/Src/main.c and this is updated everytime malloc/alloc/calloc is called.
- FLASH size is determined based on TRACES_Secure/Debug/TRACE_Secure.list (.text)
- Runtime broken down by Phases is computed based on the printout: "PhaseX start".
