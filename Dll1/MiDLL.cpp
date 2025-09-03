#include "pch.h"
#include "MiDLL.h"


struct StringVector {
	std::vector<std::string> data;
};

const char* stringtable[] = {
	"Cereza",
	"Fresa",
	"Mango",
	"Manzana",
	"Melon",
	"Naranja",
	"Pera",
	"Platano",
	"Sandia",
	"Uva",
};

struct memory {
	char* response;
	size_t size;
};

//callback para curl
static size_t WriteCallbackCurl(char* contents, size_t size, size_t nmemb, void* userdata)
{
	size_t realsize = size * nmemb;
	struct memory* mem = (struct memory*)userdata;
	char* ptr = (char*)realloc(mem->response, mem->size + realsize + 1);
	if (ptr == nullptr) {
		// out of memory!
		std::cerr << "not enough memory (realloc returned NULL)\n";
		return 0;
	}
	mem->response = ptr;
	memcpy(&(mem->response[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->response[mem->size] = 0;
	return realsize;
}

DLL1_API void funcSinParametros()
{
	std::cout << "Hola Mundo desde la DLL" << std::endl;
}

DLL1_API int funcConParametros(int a, int b)
{
	return a * b;
}

DLL1_API const char* getString(int id)
{
	if( id >= 0 && id < 10)
		return stringtable[id];
	else
		return "fuera de rango";
}

DLL1_API void getStringOut(int id, char* value, size_t size)
{
	if (value == nullptr || size == 0) 
	{
		return;
	}

	if (id >= 0 && id < 10)
	{
		strncpy_s(value, size, stringtable[id], size);
	}
	else
	{
		strncpy_s(value, size, "Fuera de ranko", size);
	}
}

DLL1_API int setWriteCallback(intCallback wcb)
{
	std::cout << "estableciendo intcallback" << std::endl;
	g_writeCallback = wcb;
	return  wcb != nullptr ? 0 : -1;
}

DLL1_API int setMsgCallback(msgCallback wcb)
{
	std::cout << "estableciendo msgcallback" << std::endl;
	g_msgCallback = wcb;
	return  wcb != nullptr ? 0 : -1;
}

DLL1_API void testIntCallback(int val, int secs)
{
	std::cout << "simulando operacion asincrona" << std::endl;
	std::thread([val, secs]() 
	{
		std::this_thread::sleep_for(std::chrono::seconds(secs));
		if (g_writeCallback)
		{
			g_writeCallback(val*val);
		}
	}).detach();
}

DLL1_API void testMsgCallback()
{
	std::cout << "simulando msgCallback asincrono" << std::endl;
	std::thread([]()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2800));
			if (g_msgCallback)
			{
				g_msgCallback("Mensaje desde la DLL");
			}
	}).detach();
}

DLL1_API int httpGet(const char* url, httpCallback cb)
{
	if (cb == nullptr || url == nullptr || url[0] == '\0')
	{
		return -1; // Error: Invalid parameters
	}
	if( strncmp(url, "http", 4) != 0 )
	{
		return -2; // Error: URL must start with http or https
	}

	//como es visto en https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
	CURLcode res;
	CURL* curl = curl_easy_init();
	if (!curl) 
	{
		return -3; // Error: Failed to initialize CURL
	}
	else
	{
		g_httpCallback = cb;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackCurl);
		struct memory chunk = { 0 };
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) 
		{
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
			free(chunk.response);
			return -4; // Error: CURL perform failed
		}
		else 
		{
			//llamamos al callback con los datos recibidos
			if (chunk.size > 0 && chunk.response != nullptr)
			{
				g_httpCallback(chunk.response, chunk.size);
			}
		}
		curl_easy_cleanup(curl);
	}

	return 0;
}

DLL1_API StringVector* getStringList()
{
	StringVector* sv = new StringVector();
	sv->data = { "hugo", "paco", "luis"};
	return sv;
}


