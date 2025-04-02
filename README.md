# Project Overview

This project consists of several folders:

- **Baseline**: For measuring performance of the baseline approach
- **Measurement**: For measuring performance of SLAPP
- **LDP**: For running a case study based on LDP using `SLAPP_{SK}`
- **LDP-Baseline**: For running a case study based on LDP using `Baseline_{SK}`
- **FL**: For running a case study based on Federated Learning using `SLAPP_{SK}`
- **FL-Baseline**: For running a case study based on Federated Learning using `Baseline_{SK}`
- **Overhead**: Computing the runtime overhead w.r.t. existing work
- **Plot**: For plotting all graphs in the paper

## Hardware Setup

### For LDP/LDP-Baseline/FL/FL-Baseline:
**NUCLEO-L552ZE-Q** ↔ **Verifier** (where ↔ denotes serial communication)

### For Baseline/Measurement:
**PZEM-004T** ↔ **CucumberR** ↔ **NUCLEO-L552ZE-Q** ↔ **Verifier** (where ↔ denotes serial communication)

## Installation

1. Download **STM32CubeIDE**. See this repo for setting up `NUCLEO-L552ZE-Q`: [ISC-FLAT_open_source](https://github.com/RIT-CHAOS-SEC/ISC-FLAT_open_source)
2. In STM32CubeIDE:
   - **File** → **Import** → **Existing Projects into Workspace** → Select the folder of interest
   - Click **TRACES_NonSecure** (in `NonSecure`) folder → Click **Run** (green play symbol) → Select **Startup** → Add Secure counterpart
3. Each folder should include a **Verifier** folder with `main.py`. Run `main.py`. (You may need to re-plug the USB cable for `NUCLEO-L552ZE-Q` after building/uploading firmware before running `main.py`.)
4. 

   - **For LDP case study**:
     - Connect **CucumberR (ESP32-S2)** to **PZEM-004T**.
     - `(RX, TX) = (16, 17)` on CucumberR.
     - **CucumberR** acts as an interface for `NUCLEO-L552ZE-Q` to read data from **PZEM-004T**.
     - **CucumberR firmware**:
       - Download "esp32" library (from the left toolbar in Arduino IDE) and possibly **Arduino AVR Boards**.
       - Open `pzem.ino` from `LDP -> PZEM-interface` and build/upload the firmware.
       - Connect **CucumberR** to **NUCLEO-L552ZE-Q**:
         - **CucumberR IO2** → **PA2**
         - **CucumberR IO3** → **PA3** (See `ldp-setup.jpg`)

## LDP Attack Simulation

1. **A1**: Poisoning the initial state during `Setup` phase.
   - Uncomment `#define ATTACK_A1` in `LDP/TRACES_NonSecure/Core/Src/main.c`
   - Rebuild and reupload the binary, then execute as normal.
   - `ldp.py` should throw an error in `Setup` phase.
2. **A2**: Poisoning attack after `Setup` phase to manipulate raw energy data.
   - Execute `Setup` with benign binary.
   - Uncomment `#define ATTACK_A2`, rebuild, and reupload.
   - Execute `Collect` phase (`c` command in `ldp.py`).
   - `ldp.py` should throw an error in `Collect` phase.
3. **A3**: Poisoning attack after `Setup` phase to manipulate state variable (Map B).
   - Same steps as **A2**, but uncomment `#define ATTACK_A3`.
   - It should raise an exception.
4. **A4**: Attack during `Collect` phase to modify noisy energy data.
   - Run benign code in SAML11, but set `ATTACK_A4 = True` in `ldp.py`.
   - It should raise an exception.

**Note:**
- When updating the benign Non-Secure World's code, replace `Verifier/TRACES_NonSecure.elf` with the newly built binary (`NonSecure/Debug/`).
- For **PMEM size experiments**, update `mem_len` in `secure_nsc.c` (`TRACES_Secure/Core/Src`) to `0x40000` and `mem` to `0x80401f8`.

## FL Attack Simulation

1. **AD**: Poisoning training data before `Local Training`.
   - After `Setup` and `Collection` phases, kill `fl.py`.
   - Uncomment `#define ATTACK_AD` in `TRACES_NonSecure/Core/Src/main.c`.
   - Rebuild and reupload the binary, then execute as normal.
   - `fl.py` should throw an error in `Setup` phase.
2. **AM**: Attack in `Local Training` phase to manipulate model parameters.
   - Run benign code in NUCLEO but set `ATTACK_AM = True` in `fl.py`.
   - Execute as normal; `fl.py` should raise an exception.

## Known Issues (TODO List)

- **NUCLEO-L552ZE-Q serial driver** is buggy—energy value reads only once before stopping.
- **Serial driver implementation** in Non-Secure World is not functional; currently resides in Secure World. However, its small code size should not significantly impact experimental results.
- **FL case study** is based on dummy data. Incorporating **PZEM-004T (+CucumberR)** remains incomplete but should not affect experimental validity.

## Experiment Methodology

- **Peak runtime memory usage**:
  - Computed from stack usage + `.rodata`, `.data`, and `.bss` sections.
  - Stack usage estimated using **Static Stack Analyzer** in STM32CubeIDE.
  - For FL, heap allocation tracked via `heapSize` in `NonSecure/Core/Src/main.c`.
- **FLASH size**: Determined from `TRACES_Secure/Debug/TRACE_Secure.list (.text)`.
- **Runtime breakdown by phases**: Based on printed messages (`PhaseX start`).
