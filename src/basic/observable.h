#pragma once

#include "s_map.h"

#include <vector>
#include <functional>

template <typename T>
class Observable
{
private:
	typedef std::function<void(const T &)> fn_t;

public:
	Observable(void)
		: _val()
		, _callbacks()
		, _id_counter(0)
	{
	}

	Observable(const T & val)
		: _val(val)
		, _callbacks()
		, _id_counter(0)
	{
	}

	T get() const {return _val;}

	std::function<void()> on(fn_t fn);
	
	void set(const T & r);

private:
	T					_val;
	smap<int, fn_t>		_callbacks;
	int					_id_counter;
};

template <typename T>
void Observable<T>::set(const T & r)
{
	_val = r;
	for (auto & callback : _callbacks)
	{
		callback.element()(_val);
	}
}

template <typename T>
std::function<void()> Observable<T>::on(fn_t fn)
{
	int id = _id_counter++;
	_callbacks[id] = fn;
	fn(_val);
	return [this, id](){
		_callbacks.erase(id);
	};
}