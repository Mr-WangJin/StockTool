#pragma once



class JKTagContextBase
{
public:
	JKTagContextBase();
	virtual ~JKTagContextBase();

	void setState(const bool& b) { bState = b; };
	const bool &getState() { return bState; };

private:
	bool bState = false;
};

