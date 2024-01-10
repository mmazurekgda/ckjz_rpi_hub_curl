
#include <cstdlib>
#include <iostream>
#include <RF24/RF24.h>
#include <stdio.h>
#include <curl/curl.h>
#include <vector>
//#include <deque>
//#include <numeric>

using namespace std;

/**************************************
	CURL query options
***************************************/


//char website_address[32] = "https://johnnybros.com";
char website_address[32] = "localhost:3000";
//char website_address[32] = "192.168.3.170:3000";

void send_request(int pipe, int modei){
	char mode[1];
	sprintf(mode,"%i",modei);
	CURL *curl;
        CURLcode res;
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        struct curl_slist *headers = NULL;
	char post_fields [10];
	char url [100];
	sprintf(post_fields,"{\"mode\":%s}",mode);
        sprintf(url,"%s/t/sensors/%d",website_address,pipe);

	if(curl){
                headers = curl_slist_append(headers, "Content-Type: application/json");
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
                res = curl_easy_perform(curl);
                if(res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                curl_easy_cleanup(curl);
                curl_slist_free_all(headers);
        }
	curl_global_cleanup();
}


/**************************************
        Sensors setup
***************************************/





// Radio pipe addresses for the 2 nodes to communicate.

const uint64_t pipes[3] ={ 0xF0F0F0F0D2LL, // base
			   0xF0F0F0F0E1LL, // male toilet
			   0xF0F0F0F0E2LL, // female toilet
			 };




// CE Pin, CSN Pin, SPI Speed
// Setup for GPIO 22 CE and GPIO 25 CSN with SPI Speed @ 1Mhz
RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_18, BCM2835_SPI_SPEED_1MHZ);


int main(int argc, char** argv) 
{
	uint8_t len;
	vector<int> male_arr;
	vector<int> female_arr;
	int female_mode =0;
	int male_mode = 0;
	radio.begin();
	radio.enableDynamicPayloads();
	radio.setAutoAck(1);
	radio.setRetries(15,15);
	radio.setDataRate(RF24_250KBPS);
	radio.setPALevel(RF24_PA_HIGH);
	radio.setChannel(108);
	radio.setCRCLength(RF24_CRC_16);

	radio.openWritingPipe(pipes[0]);
	radio.openReadingPipe(1,pipes[1]);
	radio.openReadingPipe(2,pipes[2]);

	//
	// Start listening
	//

	radio.startListening();

	//
	// Dump the configuration of the rf unit for debugging
	//

	printf("**********");
	radio.printDetails();
	printf("**********");
	printf("Output below : \n");
	delay(1);

	while(1)
	{
		char receivePayload[1];
		uint8_t pipe=1;
		
		//if (pipe == 1){
		// Start listening
		radio.startListening();

		while ( radio.available(&pipe) ) 
		{
			len = radio.getDynamicPayloadSize();
			radio.read( receivePayload, len );

			// Display it on screen
			printf("Recv: size=%i mode=%s pipe=%i \n",len,receivePayload,pipe);
			 if(pipe == 1){
				male_arr.push_back(atoi(receivePayload));
				 if( male_arr.size() > 5){
					//int sum = accumulate(male_arr.begin(), male_arr.end(), 0);
					male_arr.erase(male_arr.begin());
					int sum = 0;
					for(vector<int>::iterator it = male_arr.begin(); it != male_arr.end(); ++it) sum += *it;
					int avg = sum/male_arr.size();
					printf("Male sum: %i and avg %i \n",sum, avg );
					if (male_mode != avg){
						male_mode = avg;
						//int tmp;
						//if(avg == 1) tmp = 0;
                                                //else tmp = 1;
						printf("Sending request to the website \n");
			                        send_request(pipe, male_mode ); 
					}
				}
			}
			
			if(pipe == 2){
                               female_arr.push_back(atoi(receivePayload));
                                 if( female_arr.size() > 5){
                                        female_arr.erase(female_arr.begin());
                                        int sum = 0;
                                        for(vector<int>::iterator it = female_arr.begin(); it != female_arr.end(); ++it) sum += *it;
                                        int avg = sum/female_arr.size();
                                        printf("Female sum: %i and avg %i \n",sum, avg );
                                        if (female_mode != avg){
						female_mode = avg;
						//int tmp;
						//if(avg == 1) tmp = 0;
						//else tmp =1;
                                                printf("Sending request to the website \n");
                                                send_request(pipe, female_mode ); 
                                        }

                                }
			}
				
			


			// Send back payload to sender
			radio.stopListening();
			//if ( pipe == 1 || pipe == 2 ) {
			radio.write(receivePayload,len);
				receivePayload[len]=0;
				printf("Send: size=%i mode=%s pipe:%i\n",len,receivePayload,pipe);
			//}
			//pipe++;
			printf("%d \n", pipe);

		}
		delayMicroseconds(20);
	}
	return 0;
}

