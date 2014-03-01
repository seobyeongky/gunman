/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	sfx_msg.h

	효과음을 재생시켜줍니다.
=======================================================*/

#pragma once

#include "audiere.h"
#include "internal.h"

class opzSFXMgr
{
public:
			 opzSFXMgr(void);
			~opzSFXMgr(void);

	void	 Load(const wstring & filename);
	void	 Play(const wstring & filename);
	void	 PlayRandom(const wstring & filename, int nr_files);

	void	 Update();

private:
	typedef smap<wstring, audiere::SampleSourcePtr> samplemap;
	typedef std::list<audiere::OutputStreamPtr> streamlist;

	audiere::AudioDevicePtr	_device;
	samplemap	_loaded;
	streamlist	_streams;
};

