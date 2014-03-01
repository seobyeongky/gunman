#ifdef _DEBUG
	#pragma comment(lib, "sfml-graphics-d.lib")
	#pragma comment(lib, "sfml-main-d.lib")
	#pragma comment(lib, "sfml-window-d.lib")
	#pragma comment(lib, "sfml-system-d.lib")

	#pragma comment(lib, "opznet-client-d.lib")
	#pragma comment(lib, "opznet-server-d.lib")

	#pragma comment(lib, "icui18n-d.lib")
	#pragma comment(lib, "icuuc-d.lib")
	#pragma comment(lib, "v8_base.ia32-d.lib")
	#pragma comment(lib, "v8_nosnapshot.ia32-d.lib")
	#pragma comment(lib, "v8_snapshot-d.lib")

	#pragma comment(lib, "gtestd.lib")

	#pragma comment(lib, "TmxParserd.lib")

	#pragma comment(lib, "libprotobuf-d.lib")
#else
	#pragma comment(lib, "sfml-graphics.lib")
	#pragma comment(lib, "sfml-main.lib")
	#pragma comment(lib, "sfml-window.lib")
	#pragma comment(lib, "sfml-system.lib")

	#pragma comment(lib, "opznet-client.lib")
	#pragma comment(lib, "opznet-server.lib")

	#pragma comment(lib, "icui18n.lib")
	#pragma comment(lib, "icuuc.lib")
	#pragma comment(lib, "v8_base.ia32.lib")
	#pragma comment(lib, "v8_nosnapshot.ia32.lib")
	#pragma comment(lib, "v8_snapshot.lib")

	#pragma comment(lib, "TmxParser.lib")

	#pragma comment(lib, "libprotobuf.lib")
#endif

#pragma comment(lib, "audiere.lib")

#pragma comment(lib, "xmllite.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// for V8
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "advapi32.lib")

#pragma comment(lib, "grpapi.lib")

#pragma comment(lib, "opengl32.lib")