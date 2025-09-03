#pragma once

#ifdef DLL1_EXPORTS
#define DLL1_API __declspec(dllexport)
#else
#define DLL1_API __declspec(dllimport)
#endif

typedef int (*intCallback)(int value);
typedef void(*msgCallback) (const char* msg);

typedef void (*httpCallback)(const char* data, size_t size);

static intCallback g_writeCallback = nullptr;
static msgCallback g_msgCallback = nullptr;
static httpCallback g_httpCallback = nullptr;

//forward declaration
struct StringVector;

extern "C"
{
	DLL1_API void funcSinParametros();
	DLL1_API int funcConParametros(int a, int b);

	//ejemplos para retornar strings
	DLL1_API const char* getString(int id);
	DLL1_API void getStringOut(int id, char* value, size_t size);

	DLL1_API int setWriteCallback(intCallback  wcb);
	DLL1_API int setMsgCallback(msgCallback  wcb);
	DLL1_API void testIntCallback(int val, int secs);
	DLL1_API void testMsgCallback();

	//Wrapper para http get usando libcurl
	DLL1_API int httpGet(const char* url, httpCallback cb );

	DLL1_API StringVector* getStringList();
	DLL1_API const char* getStringAt(const StringVector*, size_t index);
	DLL1_API size_t getStringListSize(const StringVector*);
}