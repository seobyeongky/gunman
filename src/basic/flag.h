#pragma once

class Flag
{
private:
    int _val;

public:
	Flag() : _val(0) {}
	Flag(int val) : _val(val) {}

	bool has(int v) const
	{
		return (_val & v) != 0;
	}

	bool not_(int v) const
	{
		return (_val & v) == 0;
	}

	void add(int v)
	{
		_val |= v;
	}

	void drop(int v)
	{
		_val &= ~v;
	}

	int get() const
	{
		return _val;
	}
};