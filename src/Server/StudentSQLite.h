#pragma once
#include "Student.h"
#include "sqlite3.h"
#include "Define.h"
#include <sstream>

class StudentSQLite{
protected:
	sqlite3 * dbAccess;
	int result;
	char *errMsg;
public:

	int addStudent();
	//void getListStudent();
	StudentSQLite();
	~StudentSQLite();
};

