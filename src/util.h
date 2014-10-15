#pragma once

#include "internal.h"
#include "global.h"

// 메시지 박스로 에러 메시지를 띄웁니다.
void Msgbox(const wchar_t * title, const wchar_t * format_string, ...);
void ErrorMsg(const wchar_t * format_string, ...);

class DirChanger
{
public:
	 DirChanger();
	 DirChanger(const wchar_t * new_dir);
	~DirChanger();

private:
	string		_pre_dir;
	
	void	Init();
	void	Change(const wchar_t * new_dir);
			DirChanger(const DirChanger &rhs);	// 복사 방지
			DirChanger & operator = (const DirChanger &rhs); //복사 방지
};

template <typename T>
inline void SetMiddleOfScreen(T * object)
{
	object->setPosition(
		static_cast<float>(G.window.getSize().x / 2),
		static_cast<float>(G.window.getSize().y / 2)
		);
}

// sprite의 중심을 중앙으로 위치시킵니다.
template <typename T>
inline void SetMiddleOfLocal(T * object)
{
	const auto & bounds = object->getLocalBounds();
	object->setOrigin(
		bounds.width / 2,
		bounds.height / 2
		);
}

template <typename T>
inline void SetScaleToSize(T * object, float width, float height)
{
	const auto &bounds = object->getLocalBounds();
	object->setScale(width / bounds.width,
				height / bounds.height);
}

template<typename T>
inline void FitSizeToScreen(T * object)
{
	const auto &size = G.window.getSize();
	object->setSize(Vector2f(	static_cast<float>(size.x),
								static_cast<float>(size.y)));
}

template<typename T>
inline void FitScaleToScreen(T * object)
{
	const auto &size = G.window.getSize();
	SetScaleToSize(	object,
					static_cast<float>(size.x),
					static_cast<float>(size.y));
}

template<typename T>
inline void SetScaleByWidth(T * object, float width)
{
	const auto &bounds = object->getLocalBounds();
	SetScaleToSize(object, width, width * bounds.height / bounds.width);
}

template<typename T>
inline void SetScaleByHeight(T * object, float height)
{
	const auto &bounds = object->getLocalBounds();
	SetScaleToSize(object, height * bounds.width / bounds.height, height);
}

bool GetMatchedFileList(vector<wstring> * ptr, const wstring & wfilename);
bool GetTextFromFile(const wstring & wfilename, wstring * buf);
void JoinPath(const wstring& p1, const wstring& p2, wstring * ptr);

void TransferCoord(const Transform & trans, const Event & e, Event * ne);

struct error
{
	bool	flag;
	wstring msg;
};

extern CircleShape dbgpoint;
