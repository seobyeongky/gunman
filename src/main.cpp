#define  _VARIADIC_MAX  10

#include "global.h"
#include "asset.h"
#include "director.h"
#include "intro_scene.h"
#include "net_interface.h"
#include "sfx_mgr.h"
#include "userprofile.h"
#include "colors.h"

#include <v8.h>

#ifdef _DEBUG
#include <gtest/gtest.h>
#endif
#include <locale.h>

#include "scripting/v8_utils.h"

global_t G;

class LoggerLoader
{
public:
	LoggerLoader(void){G.logger = new Logger(L"log.txt", G.window.getSystemHandle());}
	~LoggerLoader(){delete G.logger; G.logger = nullptr;}
};

class AudioDeviceLoader
{
public:
	AudioDeviceLoader(void)
	{
		if (!(G.audio_device = audiere::OpenDevice()))
			G.logger->Warning(L"AudioDeviceLoader : audio device open failed");
	}
	~AudioDeviceLoader(void){G.audio_device = nullptr;}
};

class NetInterfaceLoader
{
public:
	NetInterfaceLoader(void){NetInterface::Begin();}
	~NetInterfaceLoader(){NetInterface::End();}
};

class V8EngineLoader
{
public:
	V8EngineLoader(void){v8::V8::Initialize();}
	~V8EngineLoader(void){v8::V8::Dispose();}
};

///////////////// MAIN FUNC ///////////////////
int main(int argc, char * argv[])
{
#ifdef _WIN32
	_wsetlocale(LC_ALL, L"korean");
#else
    setlocale(LC_ALL, "korean");
#endif
	srand(static_cast<unsigned int>(time(nullptr)));

	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();;

#ifdef _DEBUG
	desktop.width /= 2;
	desktop.height = static_cast<unsigned int>(desktop.height / 1.5f);
#endif

	G.window.create(desktop, L"사격의 달인"
#ifndef _DEBUG
		, Style::Fullscreen
#endif
		);
	G.window.setFramerateLimit(60U);

	LoggerLoader l;
	AudioDeviceLoader a;
	LoadSystemAssets();

	V8EngineLoader v8;

	{
		NetInterfaceLoader n;

		if (!profile::Load())
			G.logger->Error(L"Profile load failed");

#ifdef _DEBUG
		::testing::InitGoogleTest(&argc, argv);
		printf("gtest result : %d", RUN_ALL_TESTS());
#endif

		Director::Begin(new IntroScene());

		profile::Save();
	}

	ReleaseAssets();
	
	return 0;
}

