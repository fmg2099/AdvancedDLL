/*
*	Suite de pruebas para MiDLL.dll
*/
#include "MiDLL.h"
#include <iostream>
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <vector>
//handles para esperar a los eventos asincronos lanzados por la DLL
//sin ellos el programa se cierra antes de que se complete la operacion
HANDLE g_event;
HANDLE g_event2;

//callback de ejemplo que trae un parametro int
int cbTest(int val)
{
	std::cout << "Callback llamado con: " << val << std::endl;
	SetEvent(g_event);
	return 0;
}

//callback de ejemplo que trae un parametro string
void msgTest(const char* msg)
{
	std::cout << "msgCallback llamado con: " << msg << std::endl;
	SetEvent(g_event2);
}

// El MAIN
int main()
{
    std::cout << "Hello World!\n";
	funcSinParametros();
	int resultado = funcConParametros(3, 4);
	std::cout << "El resultado de funcConParametros es: " << resultado << std::endl;

	//prueba de funcion que retorna vector de strings
	StringVector* list = getStringList();
	

	g_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	//Registramos el callback ante la DLL
	int ret = setWriteCallback(cbTest);
	//imprimimos el valor devuelto para comprobar que se ha registrado correctamente
	std::cout << "setWriteCallback retorna: " << ret << std::endl;
	//Registramos el callback de char* ante la DLL
	ret = setMsgCallback(msgTest);
	g_event2 = CreateEventW(NULL, FALSE, FALSE, NULL);
	std::cout << "setMsgCallback retorna: " << ret << std::endl;

	//llamamos a una funcion de la DLL que lanza una operacion asincrona que al completarse llama al callback
	testMsgCallback();
	testIntCallback(28, 3);
	

	for(int i = 0; i <= 5; i++)
	{
		std::cout << "getString " << i << ": " << getString(i) << std::endl;
	}
	for (int i = 6; i <= 10; i++)
	{
		char buffer[50];
		getStringOut(i, buffer, sizeof(buffer));
		std::cout << "getStringOut " << i << ": " << buffer << std::endl;
	}

	//hacemos una llamada http get
	const char* url = "http://monsterballgo.com/api/names";
	httpGet(url, [](const char* data, size_t size)
		{
			std::cout << "callback size:  " << size << std::endl;
			std::cout << "data: " << data << std::endl;
	});
	std::cout << "httpGet llamada" << std::endl;

	//para evitar que se cierre la aplicacion antes de que se complete la operacion asincrona
	if (g_event != NULL)
	{
		WaitForSingleObject(g_event, INFINITE);
		CloseHandle(g_event);
	}
	if (g_event2 != NULL)
	{
		WaitForSingleObject(g_event2, INFINITE);
		CloseHandle(g_event2);
	}

	return 0;
}

