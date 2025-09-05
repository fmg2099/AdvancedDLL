#pragma once

#ifdef DLL1_EXPORTS
#define DLL1_API __declspec(dllexport)
#else
#define DLL1_API __declspec(dllimport)
#endif

//forward declaration del apuntador opaco
struct StringVector;

typedef int (*intCallback)(int value);
typedef void(*msgCallback) (const char* msg);

//firma del callback que se debe establecer para recibir respuesta GET
typedef void (*httpCallback)(const char* data, size_t size);

//firma de callback con la lista de strings
typedef void (*httpStringListCallback)(StringVector * sv, size_t size);

static intCallback g_writeCallback = nullptr;
static msgCallback g_msgCallback = nullptr;
static httpCallback g_httpCallback = nullptr;
static httpStringListCallback g_httpSLCB = nullptr;



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

	//API para recuperar una lista de strings
	DLL1_API StringVector* getStringList(); //esta si es de ejemplo
	DLL1_API const char* getStringAt(const StringVector* sv, size_t index);
	DLL1_API size_t getStringListSize(const StringVector* sv);

	//Wrapper para http get usando libcurl
	DLL1_API int httpGet(const char* url, httpCallback cb);

	//retorna una lista de nombres permitidos desde el servidor
	// no lleva URL porque ya esta establecida 
	DLL1_API int GetAuthorizedNames(httpStringListCallback cb);
}