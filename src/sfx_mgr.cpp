#include "sfx_mgr.h"
#include "global.h"

using namespace audiere;

opzSFXMgr::opzSFXMgr(void)
	: _device(OpenDevice()),
	_loaded()
{
}

opzSFXMgr::~opzSFXMgr(void)
{
	_device = 0;
	for (auto source : _loaded)
	{
		source.element()->unref();
	}
}

#include <stdio.h>

void opzSFXMgr::Play(const wstring & filename)
{
	printf("0\n");
	if (!_device) return;
	if (_streams.size() > 10) return;

	SampleSourcePtr	source;
	samplemap::Iter	it;
	printf("1\n");
	if (!_loaded.find(filename, &it))
	{
		string mfilename;
		uni2multi(filename, &mfilename);
		source = OpenSampleSource(mfilename.c_str());
		if (!source) 
		{
			G.logger->Warning(L"opzSFXMgr:File %s open failed", filename.c_str());
			return;
		}
		source->ref();
		_loaded.insert(filename, source);
	}
	else
	{
		source = (*it).element();
	}

	OutputStreamPtr stream(OpenSound(_device, source, false));

	if (!stream)
	{
		G.logger->Warning(L"opzSFXMgr:Play stream opening failed");
		return;
	}

	stream->play();
	_streams.push_back(stream);
	printf("2\n");
}

void opzSFXMgr::Update()
{
	for(std::list<OutputStreamPtr>::iterator i = _streams.begin();
		i != _streams.end();)
	{
		if((*i)->isPlaying())
			++i;
		else
		{
			(*i)->stop();
			*i = NULL;
			i = _streams.erase(i);
		}
	}
}