#pragma once


void pause_thread(int seconds);

template<typename Variable, typename Mtx>
class JKVariableMtx
{
public:
	JKVariableMtx(Variable _var)
		: var(_var)
	{
	};
	~JKVariableMtx()
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


