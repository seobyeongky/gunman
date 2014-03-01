#pragma once

class Flag
{
public:
	Flag() : _val(0) {}
	Flag(int val) : _val(val) {}

	bool has(int v) const
	{
		return (_val & v) != 0;
	}

	bool not(int v) const
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

private:
	int _val;
};