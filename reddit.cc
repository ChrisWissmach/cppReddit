#include "reddit.h"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
}

// Initialize the authorization token and user-agent headers to be sent with each request
Reddit::Reddit(): headers(NULL){
}

void Reddit::setOAuthInfo(string username1, string password1, string client_id1, string client_secret1, string user_agent1){
	username = username1;
	password = password1;
	client_id = client_id1;
	client_secret = client_secret1;
	user_agent = user_agent1;
}

// authenticate the user and receive an access token
void Reddit::authenticate(){
	string resString;
	curl = curl_easy_init();
	if(curl) { 
                string data_1 = "grant_type=password&username="+username+"&password="+password;
                const char *data = data_1.c_str();
                
		string client_data1 = client_id+":"+client_secret;
		const char *client_data = client_data1.c_str();

		curl_easy_setopt(curl, CURLOPT_USERPWD, client_data);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
                curl_easy_setopt(curl, CURLOPT_URL, "https://www.reddit.com/api/v1/access_token");
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resString);

                // Perform the request, res will get the return code  
                res = curl_easy_perform(curl);
                // Check for errors  
                if(res != CURLE_OK)
                        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));

                // always cleanup  
                curl_easy_cleanup(curl);
        }
	// convert res from CURLcode to a string, to be used with JSON parser
	Json::Value root;
        std::stringstream ss;
        ss << resString;
        ss >> root;
        access_token = root.get("access_token", "default_token" ).asString();
	cout << "Authenticated with access token: " << access_token << endl;	
	
	// initialize the autorization token and user-agent headers to be sent with every request
	string header_1 = "Authorization: bearer " + access_token;
        const char *header = header_1.c_str();
        headers = curl_slist_append(headers,header);
	string user_agent_1 = "User-Agent: " + user_agent;
	const char *user_agent_header = user_agent_1.c_str();
        headers = curl_slist_append(headers, user_agent_header);


}

int Reddit::commentKarma(string user){
	string resString;
	curl = curl_easy_init();
        if(curl) {
                string data_1 = "https://oauth.reddit.com/user/"+user+"/about.json";
		const char *data = data_1.c_str();
                curl_easy_setopt(curl, CURLOPT_URL, data);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resString);

                // Perform the request, res will get the return code  
                res = curl_easy_perform(curl);
                // Check for errors  
                if(res != CURLE_OK)
                        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));

                // always cleanup  
                curl_easy_cleanup(curl);
        }
        // convert res from CURLcode to a string, to be used with JSON parser
        Json::Value root;
        std::stringstream ss;
        ss << resString;
        ss >> root;
	Json::Value data = root["data"];
        int karma= data.get("comment_karma", -1 ).asInt();
	return karma;
}

int Reddit::linkKarma(string user){
        string resString;
        curl = curl_easy_init();
        if(curl) {
                string data_1 = "https://oauth.reddit.com/user/"+user+"/about.json";
                const char *data = data_1.c_str();
                curl_easy_setopt(curl, CURLOPT_URL, data);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resString);

                // Perform the request, res will get the return code  
                res = curl_easy_perform(curl);
                // Check for errors  
                if(res != CURLE_OK)
                        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));

                // always cleanup  
                curl_easy_cleanup(curl);
        }
        // convert res from CURLcode to a string, to be used with JSON parser
        Json::Value root;
        std::stringstream ss;
        ss << resString;
        ss >> root;
        Json::Value data = root["data"];
        int karma= data.get("link_karma", -1 ).asInt();
        return karma;
}




// TESTS
int main(void){

        Reddit *r = new Reddit;
        r->setOAuthInfo(<USERNAME>, <PASSWORD>, <CLIENT_ID>, <CLIENT_SECRET> ,<CUSTOM_USER_AGENT>);
        r->authenticate();
	int comment_karma = r->commentKarma(<SOME_USERNAME>);
	int link_karma = r->linkKarma(<SOME_USERNAME>);
}
