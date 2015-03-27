#define SSID "vlohome" 				// change this to match your WiFi SSID
#define PASS "dolunay123456" 		// change this to match your WiFi password
#define PORT "80" 					// using port 80 by default


#define esp Serial
#define BUFFER_SIZE 500

class Wifi {
public:
	void begin(const int,  const int);
}