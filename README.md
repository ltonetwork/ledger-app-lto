# Ledger BOLOS application for LTO Network
Community made LTO Network wallet application for Ledger Nano S/X/Blue

# Introduction
This is a wallet application for the LTO Network blockchain done by a member of the community.

It is compliant with [LTO Network cryptographic specifications](https://docs.lto.network/project/developer-area/lto_protocol/cryptographic_details). 

It can be tested quickly using the python script located at [python/ledger-lto.py](https://github.com/iicc1/ledger-app-lto-network-unofficial/tree/master/python).

Special thanks to Ledger team and Waves community.

# Building

In order to build this application you need to create a Ledger development environment.  More information can be 
found here: https://ledger.readthedocs.io/en/latest/userspace/getting_started.html


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


# Run test app

Once the app is compiled and loaded, you can try to communicate with it using the Python script.

Install dependencies:
```bash
pip install -r python2.7-dependencies.txt --no-index
```

Then enter the LTO Network app on your ledger and start the script_
```bash
python python/ledger-lto.py
```
