#define  _VARIADIC_MAX  10

#include "global.h"
#include "asset.h"
#include "director.h"
#include "intro_scene.h"
#include "net_interface.h"
#include "sfx_mgr.h"
#include "userprofile.h"
#include "colors.h"

#ifdef _DEBUG
#include <gtest\gtest.h>
#endif
#include <locale.h>

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
			G.logger->Warning(L"AudioDeviceLoader : audio device 열기 실패");
	}
	~AudioDeviceLoader(void){G.audio_device = nullptr;}
};

class NetInterfaceLoader
{
public:
	NetInterfaceLoader(void){NetInterface::Begin();}
	~NetInterfaceLoader(){NetInterface::End();}
};

///////////////// MAIN FUNC ///////////////////
int main(int argc, char * argv[])
{
	_wsetlocale(LC_ALL, L"korean");
	srand(static_cast<unsigned int>(time(nullptr)));

	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();;

#ifdef _DEBUG
	desktop.width /= 2;
	desktop.height /= 1.5;
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

	{
		NetInterfaceLoader n;

		if (!profile::Load())
			G.logger->Error(L"프로필 로드 실패!");

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

