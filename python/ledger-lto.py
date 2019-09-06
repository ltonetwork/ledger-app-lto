#!/usr/bin/env python2.7
# *******************************************************************************
# *   Ledger Blue
# *   (c) 2016 Ledger
# *
# *  Licensed under the Apache License, Version 2.0 (the "License");
# *  you may not use this file except in compliance with the License.
# *  You may obtain a copy of the License at
# *
# *      http://www.apache.org/licenses/LICENSE-2.0
# *
# *  Unless required by applicable law or agreed to in writing, software
# *  distributed under the License is distributed on an "AS IS" BASIS,
# *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# *  See the License for the specific language governing permissions and
# *  limitations under the License.
# ********************************************************************************

# ********************************************************************************
# *
# *  Script edited and improved by iicc1 to support LTO Network blockchain.
# *  with more features, less dependencies and fully plaftorm independent.
# *  https://github.com/iicc1/ledger-app-lto
# *
# ********************************************************************************

from ledgerblue.comm import getDongle
from ledgerblue.commException import CommException
from colorama import init, Fore, Style
import base58
import struct
import sys
import time

global dongle
dongle = None

# Colorama for Windows OS
init()

# 'T' for testnet, 'L' for mainnet
chain_id = 'L'


def getKeysFromDongle(path, networkByte):
    global dongle
    while (True):
        try:
            data_bytes = bytes(("800400" + '{0:x}'.format(ord(networkByte)) + "14").decode('hex')) + path_to_bytes(path)
            data = dongle.exchange(data_bytes)
            return [data[0:32], data[32:67]]
        except CommException as e:
            if (e.sw == 0x6985):
                print(Fore.RED + "Required condition failed.")
            if (e.sw == 0x9100):
                print(Fore.RED + "User denied signing request on Ledger Nano S device.")
            break
        except Exception as e:
            raw_input(
                "An error occurred while processing the request, repeat or correct your request (note what all the bip32 path parts should be hardened)")
            sys.exc_clear()
            break

def getVersionFromDongle():
    global dongle
    while (True):
        try:
            data_bytes = bytes(("8006000000").decode('hex'))
            data = dongle.exchange(data_bytes)
            return data[0:3]
        except CommException as e:
            if (e.sw == 0x6985):
                print(Fore.RED + "Required condition failed.")
            if (e.sw == 0x9100):
                print(Fore.RED + "User denied signing request on Ledger Nano S device.")
            break
        except Exception as e:
            raw_input(
                "An error occurred while processing the request, repeat or correct your request (note what all the bip32 path parts should be hardened)")
            sys.exc_clear()
            break

# 128 - 5 service bytes
CHUNK_SIZE = 123
PRIME_DERIVATION_FLAG = 0x80000000


def path_to_bytes(path):
    return ''.join([struct.pack(">I", n) for n in path])


def convert_prime(n):
    # Convert minus signs to uint32 with flag
    return [int(abs(x) | PRIME_DERIVATION_FLAG) if x < 0 else x for x in n]


def expand_path(n):
    # Convert string of bip32 path to list of uint32 integers with prime flags
    # 0/-1/1' -> [0, 0x80000001, 0x80000001]
    if not n:
        return []

    n = n.split('/')

    # m/a/b/c => a/b/c
    if n[0] == 'm':
        n = n[1:]

    path = []
    for x in n:
        prime = False
        if x.endswith("'"):
            x = x.replace('\'', '')
            prime = True
        if x.startswith('-'):
            prime = True

        x = abs(int(x))

        if prime:
            x |= PRIME_DERIVATION_FLAG

        path.append(x)

    return path


def build_transfer_bytes(publicKey, recipient, amount, attachment, txFee, version, timestamp, tx_type):
    
    sData = chr(tx_type) + version + chr(tx_type)

    if version == b'\2':
        sData += version
    if tx_type == 4: # transfer
        sData += base58.b58decode(publicKey) + \
                struct.pack(">Q", timestamp) + \
                struct.pack(">Q", amount) + \
                struct.pack(">Q", txFee) + \
                base58.b58decode(recipient) + \
                struct.pack(">H", len(attachment))
    elif tx_type == 8: # start lease
        sData += base58.b58decode(publicKey) + \
                base58.b58decode(recipient) + \
                struct.pack(">Q", amount) + \
                struct.pack(">Q", txFee) + \
                struct.pack(">Q", timestamp)
    elif tx_type == 9: # cancel lease
        sData += base58.b58decode(publicKey) + \
                struct.pack(">Q", txFee) + \
                struct.pack(">Q", timestamp) + \
                base58.b58decode(recipient)


    return sData

while (True):
    while (dongle == None):
        try:
            dongle = getDongle(True)
        except Exception as e:
            answer = raw_input(Fore.RED + Style.BRIGHT + 
                "Please connect your Ledger Nano S, unlock, and launch the LTO Network app. Press <enter> when ready. (Q quits)" + Fore.WHITE)
            if (answer.upper() == 'Q'):
                sys.exit(0)
            sys.exc_clear()

    print("")
    print(Fore.CYAN + Style.BRIGHT + "Ledger Nano S - LTO Network cli app")
    print("To copy text in Windows you must select the text and press enter. It will be saved in your clipboard.")
    print("To paste text in Windows, do a right click in the console.")
    print(Fore.WHITE + "\t 1. Get PublicKey/Address from Ledger Nano S")
    print("\t 2. Sign transfer using Ledger Nano S")
    print("\t 3. Sign start lease using Ledger Nano S")
    print("\t 4. Sign cancel lease using Ledger Nano S")
    print("\t 5. Get app version from Ledger Nano S")
    print("\t 6. Exit")
    select = raw_input(Fore.CYAN + "Please select> ")

    if (select == "1"):
        path = raw_input(
            Fore.BLUE + "Please input BIP-32 path (default, press enter: \"44'/353'/0'/0'/0'\")> " + Fore.WHITE + Style.DIM)
        if len(path) == 0:
            path = "44'/353'/0'/0'/0'"
        keys = getKeysFromDongle(expand_path(path), chain_id)
        if keys:
            publicKey = keys[0]
            address = keys[1]
            print(Fore.BLUE + Style.BRIGHT + "publicKey (base58): " + base58.b58encode(str(publicKey)))
            print("address: " + address)
    elif (select == "2" or select == "3" or select == "4"):
        path = raw_input(
            Fore.BLUE + "Please input BIP-32 path (default, press enter: \"44'/353'/0'/0'/0'\")> ")
        if len(path) == 0:
            path = "44'/353'/0'/0'/0'"
        binary_data = path_to_bytes(expand_path(path))
        print(Fore.WHITE + Style.DIM + "path bytes: " + base58.b58encode(str(path_to_bytes(expand_path(path)))) + Style.BRIGHT)

        # tx amount asset decimals
        binary_data += chr(8)
        # fee amount asset decimals
        binary_data += chr(8)

        timestamp = int(time.time() * 1000)
        publicKey = None
        recipient = None
        amount = None
        
        # Transaction type depending on input
        tx_type = 4
        if select == "3":
            tx_type = 8
        elif select == "4":
            tx_type = 9
        
        input = raw_input(Fore.BLUE + "Please input your public key> ")
        if len(input) == 0:
            break
        else:
            publicKey = input

        if tx_type == 4 or tx_type == 8:
            input = raw_input(Fore.BLUE + "Please input the recipient LTO address> ")
            if len(input) == 0:
                break
            else:
                recipient = input
            input = raw_input(Fore.BLUE + "Please input the amount of LTO to send> ")
            if len(input) == 0:
                break
            else:
                amount = int(input) * 100000000
        else:
            input = raw_input(Fore.BLUE + "Please input the transaction Id of the lease to cancel> ")
            if len(input) == 0:
                break
            else:
                recipient = input

        # Gets the binary data that will be signed in Ledger
        binary_data += build_transfer_bytes(publicKey, recipient, amount, '',100000000 , b'\1', timestamp, tx_type)
        signature = None
        while (True):
            try:
                offset = 0
                while offset != len(binary_data):
                    if (len(binary_data) - offset) > CHUNK_SIZE:
                        chunk = binary_data[offset: offset + CHUNK_SIZE]
                    else:
                        chunk = binary_data[offset:]
                    if (offset + len(chunk)) == len(binary_data):
                        p1 = 0x80
                    else:
                        p1 = 0x00

                    if (offset == 0):
                        print(Fore.WHITE + Style.BRIGHT + "Waiting for approval to sign on the Ledger Nano S" + Style.DIM)

                    apdu = bytes("8002".decode('hex')) + chr(p1) + chain_id + chr(len(chunk)) + bytes(chunk)
                    signature = dongle.exchange(apdu)
                    offset += len(chunk)
                print(Fore.BLUE + Style.BRIGHT + "\n     ** Transaction signed successfully **\n  Now broadcast it by pasting the JSON here:"
"\n  https://nodes.lto.network/api-docs/index.html#!/transactions/broadcast\n"
"  Then you can track your transaction here: https://explorer.lto.network\n")
                print(Fore.MAGENTA + "{\"senderPublicKey\":\"" + publicKey + "\",")
                if tx_type == 4 or tx_type == 8:
                    print("\"amount\":" + str(amount) + ",")
                print("\"signature\":\"" + base58.b58encode(str(signature)) + "\",")
                print("\"fee\":100000000,")
                if tx_type == 9:
                    print("\"txId\":\"" + recipient + "\",")
                else:
                    print("\"recipient\":\"" + recipient + "\",")
                print("\"type\":" + str(tx_type) + ",")
                print("\"timestamp\":" + str(timestamp) + "}")
                break
            except CommException as e:
                if (e.sw == 0x6990):
                    print(Fore.RED + "Transaction buffer max size reached.")
                if (e.sw == 0x6985):
                    print(Fore.RED + "Required condition failed.")
                if (e.sw == 0x9100):
                    print(Fore.RED + "User denied signing request on Ledger Nano S device.")
                break
            except Exception as e:
                print(e, type(e))
                answer = raw_input(
                    "Please connect your Ledger Nano S, unlock, and launch the LTO Network app. Press <enter> when ready. (Q quits)")
                if (answer.upper() == 'Q'):
                    sys.exit(0)
                sys.exc_clear()
    elif (select == "5"):
        version = getVersionFromDongle()
        print('App version is {}.{}.{}'.format(version[0],version[1],version[2]))
    else:
        break