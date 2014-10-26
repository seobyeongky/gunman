#include "sv_service.h"
#include "sv_internal.h"
#include "sv_director.h"
#include "sv_wait_scene.h"

void GoServer(const wstring & room_name)
{
	SvDirector::Begin(new SvWaitScene());
}

bool BeginSvService(const wstring & room_name)
{
	if (SvDirector::IsWorking()) return false;

	BeginServer(room_name);

	thread t(&GoServer, room_name);

	t.detach();

	while (!SvDirector::IsWorking())
	{
        chrono::milliseconds dura(50);
        this_thread::sleep_for(dura);
	}

	return true;
}

void EndSvService()
{
	SvDirector::End();

	while (SvDirector::IsWorking())
	{
        chrono::milliseconds dura(50);
        this_thread::sleep_for(dura);
	}

	EndServer();
}