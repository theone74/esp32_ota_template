
#ifndef HIDECHIPID
#define HIDECHIPID 1
#endif // !HIDECHIPID

#ifndef OTAURL
#define OTAURL "http://example.ru/%s"
#endif // !OTAURL

#ifndef CHECKUPDATEINTERVAL
#define CHECKUPDATEINTERVAL 60
#endif // !CHECKUPDATEINTERVAL

namespace OTA {
	
	extern bool stop;
	extern bool is_updating;
	
	void ota_setup();

};