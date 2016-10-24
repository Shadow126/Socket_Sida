#include "stdafx.h"
#include "Student.h"


Student::Student()
{
}


Student::~Student()
{
}
void Student::setStudent(string id, string name, string sex, int math, int physical, int chemistry, int sum) {
	id = id;
	name = name;
	sex = sex;
	math = math;
	physical = physical;
	chemistry = chemistry;
	sum = sum;
}
void Student::setId(string ID) {
	id = ID;
}

string Student::getId() {
	return id;
}

void Student::setName(string NAME) {
	name = NAME;
}

string Student::getName() {
	return name;
}

void Student::setSex(string SEX) {
	sex = SEX;
}

string Student::getSex() {
	return sex;
}

void Student::setMath(int MATH) {
	math = MATH;
}

int Student::getMath() {
	return math;
}

void Student::setChemistry(int CHEMISTRY) {
	chemistry = CHEMISTRY;
}

int Student::getChemistry() {
	return chemistry;
}

void Student::setSum(int SUM){
	sum = SUM;
}

int Student::getSum() {
	return sum;
}
void Student::setPhysical(int PHYSICAL) {
	physical = PHYSICAL;
}

int Student::getPhysical() {
	return physical;
}