rpi-hub-curl: rpi-hub-curl.cpp
	g++ rpi-hub-curl.cpp -o rpi-hub-curl -lrf24 -lcurl
