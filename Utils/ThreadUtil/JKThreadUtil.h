#pragma once


void pause_thread(size_t milliseconds);

template<typename Variable, typename Mtx>
class JKVariableRWMtx
{
public:
	JKVariableRWMtx(Variable _var)
		: var(_var)
	{
	};
	~JKVariableRWMtx()
	{
	}

	Variable getVariable()
	{
		std::unique_lock<std::mutex> ulck(varMtx);
		return var;
	}
	void setVariable(Variable _var)
	{
		std::unique_lock<std::mutex> ulck(varMtx);
		var = _var;
	}

private:
	Variable var;
	Mtx varMtx;
};


