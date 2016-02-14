#include <stdio.h>
#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include "jsoncpp.cpp"
#include "json/json.h"
#include "json/json-forwards.h"
#include <string.h>
using namespace std;

class Reddit{
        CURL *curl;
	CURLcode res;
	curl_slist *headers;
        string username;
        string password;
        string client_id;
        string client_secret;
        string access_token;
	string user_agent;
public:
	Reddit();
        void setOAuthInfo(string username1, string password1, string client_id1, string client_secret1, string user_agent1);
        void authenticate();
	int commentKarma(string user);
	int linkKarma(string user);

};
