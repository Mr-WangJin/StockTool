#pragma once

#include "JKBaseModel.h"

class JKProjectVersionModel : public JKBaseModel
{
	//friend class JKProjectVersionBLL;
	friend class hiberlite::Database;
	friend class hiberlite::access;
	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(version);
	}


public:
	int version;

};

