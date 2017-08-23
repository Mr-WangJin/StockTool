#pragma once

#include <JKFramework\JKSingleton.h>

#include <hiberlite.h>


class JKDatabase
{
public:
	JKDatabase();
	~JKDatabase();

public:
	hiberlite::Database db;

};




