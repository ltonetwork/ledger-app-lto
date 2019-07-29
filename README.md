# Ledger BOLOS application for LTO Network
Community made LTO Network wallet application for Ledger devices.

# Introduction
This is a Ledger device wallet application for the LTO Network blockchain developed by a community member.

It is compliant with [LTO Network cryptographic specifications](https://docs.lto.network/project/developer-area/lto_protocol/cryptographic_details). 

It can be tested quickly using the python script located at [python/ledger-lto.py](https://github.com/iicc1/ledger-app-lto-network-unofficial/tree/master/python).

Special thanks to Ledger team, Waves community and LTO Network team.

[User guide](https://github.com/iicc1/ledger-app-lto/wiki/How-to-use-a-Ledger-device-with-LTO-Network)


# Building

In order to build this application you need to create a Ledger development environment. More information can be 
found here: https://ledger.readthedocs.io/en/latest/userspace/getting_started.html
Message me if you want a virtual machine with a Ledger development environment prepared to compile.


Compile the app:
```bash
make
```
Load the app onto the device:
```bash
make load
```
Remove the app from the device:
```bash
make delete
```
Clean compilation files:
```bash
make clean
```


# Run cli app

Once the Ledger app is compiled and/or loaded, you can try to communicate with it using the cli app.
You can download the precompiled binaries from the [releases tag](https://github.com/iicc1/ledger-app-lto/releases). Make sure you download the correcty executable for your platform.
Then execute the program and the app will guide you throught the process.

You can also run the cli app from source, it is just a Python script located at the python/ folder. 
Python 2.7 is required for this, latest releases are found here [here](https://www.python.org/downloads/release/python-2716/).

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


# Web app

There is also a Vuejs web app project located here: https://github.com/iicc1/lto-ledger-vue