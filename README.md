# ckjz_rpi_hub_curl

## Installation

### Python

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

![image](https://github.com/mmazurekgda/ckjz_rpi_hub_curl/assets/12483634/143216c0-f41d-4bbf-b8a2-334a7683662e)
