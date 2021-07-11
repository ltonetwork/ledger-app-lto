# Ledger BOLOS application for LTO Network
![Docker Cloud Build Status](https://img.shields.io/docker/cloud/build/ignacioxyz/lto-ledger-devenv?style=for-the-badge) ![Docker Pulls](https://img.shields.io/docker/pulls/ignacioxyz/lto-ledger-devenv?style=for-the-badge)

Community made LTO Network wallet application for Ledger devices.

<br>

# Introduction
- Ledger device wallet application for the LTO Network blockchain developed by a community member.

- It is compliant with [LTO Network cryptographic specifications](https://docs.ltonetwork.com/protocol/cryptography). 

- There is a ready to use web app interface [here](https://lto.stakely.io/).

- It can be tested using the Python script located at [python/ledger-lto.py](https://github.com/iicc1/ledger-app-lto/tree/master/python).

- Special thanks to the Ledger team, Waves community and, LTO Network team.

- LTO Network Ledger APP [User Guide](https://github.com/iicc1/ledger-app-lto/wiki/How-to-use-a-Ledger-device-with-LTO-Network)

<br>

# Building

To build this application you need to create a Ledger development environment. More information can be 
found [here](https://ledger.readthedocs.io/en/latest/userspace/setup.html).

Alternatively, you can use my Docker image to compile the App with just one command.

<br>

# Prerequisites

First, you must have Docker and Python 2 or 3 installed.
- Docker installation instructions: https://docs.docker.com/engine/install/
- Python installation instructions: https://www.python.org/downloads/

Docker is used to compile the App from source and Python to load it into your device.

Next, download or clone this repository into a folder. If you have downloaded it, remember to extract it.

<br>

# Compiling

## Ledger Nano S

Open a terminal inside the folder where the repository was downloaded and enter the following command:

_Linux/Mac/Windows (PowerShell)_
```
docker run -v ${PWD}:/code ignacioxyz/lto-ledger-devenv 'make'
```
_Windows (CMD)_
```
docker run -v %cd%:/code ignacioxyz/lto-ledger-devenv 'make'
```
This command will download my Docker image from DockerHub and build the Ledger App, creating several folders within the current folder.

<br>

## Ledger Nano X

The Docker image uploaded to my Dockerhub uses the Ledger Nano S SDK. To compile this APP for a Ledger Nano X device, you need to build the Docker Image with the Nano X SDK.


To do this, edit the SDK download step in the Dockerfike
```
RUN echo "Download Ledger Nano X SDK" && \
  git clone --branch 1.2.4-5.1 https://github.com/LedgerHQ/nanox-secure-sdk.git ${BOLOS_SDK}
```

Build the Ledger image locally
```
docker build -t sdknanox .
```

And build the Ledger Nano X APP
```
docker run -v ${PWD}:/code sdknanox:latest 'make TARGET_NAME=TARGET_NANOX'
```
<br>

# Uploading

## Nano S

Now we will use Python to upload the App to your device.

You need to have `ledgerblue` depencency installed:

_Python_
```
pip install ledgerblue
```
_Python3_
```
pip3 install ledgerblue
```

And finally, load the App:

_Python_
```
python -m ledgerblue.loadApp --appFlags 0x240 --path "44'/353'" --curve secp256k1 --curve ed25519 --tlv --targetId 0x31100004 --delete --fileName bin/app.hex --appName "LTO Network" --appVersion 2.2.0 --dataSize 64 --icon 010000000000ffffffffffffffffffffffbffd7ffedffbb7ed67e6cff39ff97ffe7ffeffffffffffff
```
_Python3_
```
python3 -m ledgerblue.loadApp --appFlags 0x240 --path "44'/353'" --curve secp256k1 --curve ed25519 --tlv --targetId 0x31100004 --delete --fileName bin/app.hex --appName "LTO Network" --appVersion 2.2.0 --dataSize 64 --icon 010000000000ffffffffffffffffffffffbffd7ffedffbb7ed67e6cff39ff97ffe7ffeffffffffffff
```
After this, the installation process will start, asking for your permission on the Ledger screen.

<br>

## Nano X
Unfortunately, at the time of writing, there is no way to load a non-official Ledger APP into a Ledger Nano X.

You can use a Ledger Nano X emulator called [Speculos](https://speculos.ledger.com/).

<br>


# Web app

There are currently two web interfaces available to interact with this Ledger application:
- **Recommended**: https://github.com/iicc1/lto-network-ledger-wallet-ui - [Hosted frontend](https://lto.stakely.io)
- Deprecated: https://github.com/iicc1/lto-ledger-vue - [Hosted frontend](https://lto-ledger-beta.netlify.app/)

<br>


# CLI app

Once the Ledger app is loaded, you can try to communicate with it using the CLI app.
You can download the precompiled binaries from the [releases tag](https://github.com/iicc1/ledger-app-lto/releases). Make sure you download the correct executable for your platform.
Then execute the program and the app will guide you through the process.

You can also run the CLI app from source, it is just a Python script located at the python/ folder. 
Python 2.7 is required for this, the latest releases are found here [here](https://www.python.org/downloads/release/python-2716/).

Install dependencies:
```bash
pip install ledgerblue
pip install colorama
pip install base58
```

Then enter the LTO Network app on your ledger and start the script:
```bash
python python/ledger-lto.py
```
