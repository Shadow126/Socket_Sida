#pragma once
#include<iostream>
using namespace std;
class Student{
private:
	string id;
	string name;
	string sex;
	int math;
	int physical;
	int chemistry;
	int sum;
public:
	void setStudent(string id, string name, string sex, int math, int physical, int chemistry, int sum);
	void setId(string);
	string getId();

	void setName(string);
	string getName();

	void setSex(string);
	string getSex();

	void setMath(int);
	int getMath();

	void setPhysical(int);
	int getPhysical();

	void setChemistry(int);
	int getChemistry();

	void setSum(int);
	int getSum();
	Student();
	~Student();
};

