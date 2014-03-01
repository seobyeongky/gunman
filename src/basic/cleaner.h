#pragma once

#include <functional>
#include <vector>

class Cleaner
{
public:
	Cleaner();
	~Cleaner();

	typedef std::function<void()> voidfn_t;

	void Register(voidfn_t fn);
	void Activate();

private:
	std::vector<voidfn_t> _arr;
};