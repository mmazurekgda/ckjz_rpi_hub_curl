# ckjz_rpi_hub_curl

## Installation

### Python

1. Make sure you have conda and pip installed with Python3. Install the dependencies:

```console
pip install -r requirements.txt
```

2. Set the right pin numbers and the URI of the host with CKJZ sockets exposed at this address. This is configurable in `rpi-hub-curl.py`

3. Run `python rpi-hub-curl.py` or run it with cron to have it ready on startup.

### C++
Tested on Debian-based system only. 

1. Install `make` and `g++`.

```console
sudo apt install make g++
```

2. Install RF24 library from https://rf24.readthedocs.io/en/v1.4.4/md_docs_linux_install.html

3. Set the right pin numbers and the URI of the host with CKJZ sockets exposed at this address. This is configurable in `rpi-hub-curl.cpp`

4. Run `make` and then `./rpi-hub-curl` or run it with cron to have it ready on startup.

## Default pinout

![image](pinout.png)
