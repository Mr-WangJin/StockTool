#pragma once

#include <JKFramework\JKSingleton.h>

#include <hiberlite.h>


class JKDatabase
{
public:
	JKDatabase();
	~JKDatabase();

	void dropAllTable();
	void checkModel();

public:
	hiberlite::Database db;

};




