#include "cleaner.h"

using namespace std;

Cleaner::Cleaner()
	: _arr()
{
}

Cleaner::~Cleaner()
{
	Activate();
}

void Cleaner::Register(voidfn_t fn)
{
	_arr.push_back(fn);
}

void Cleaner::Activate()
{
	for (auto & fn : _arr) fn();
	_arr.clear();
}