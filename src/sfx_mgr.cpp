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
}

void opzSFXMgr::Play(const wstring & filename)
{
	if (!_device) return;
	if (_streams.size() > 10) return;

	SampleSourcePtr	source;
	samplemap::Iter	it;
	if (!_loaded.find(filename, &it))
	{
		string mfilename;
		uni2multi(filename, &mfilename);
		source = OpenSampleSource(mfilename.c_str());
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