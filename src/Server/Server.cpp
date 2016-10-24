//Server.cpp : Defines the entry point for the console application.

#include "stdafx.h"

#pragma comment(lib,"ws2_32.lib")//??
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<WinSock2.h>

#include "Student.h"
#include "sqlite3.h"
#include "Define.h"
#include <iostream>
#include <string>
#include <sstream>


using namespace std;

int rc;
char *error;
sqlite3 *db;

Student student;

SOCKET newConnection;

int Connection() {
	// Open Database
	cout << "Opening MyDb.db ..." << endl;
	rc = sqlite3_open(DATABASE, &db);
	if (rc)
	{
		cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_close(db);
		return 1;
	}
	else
	{
		cout << "Opened MyDb.db." << endl << endl;
	}

	// Execute SQL
	cout << "Creating MyTable ..." << endl;
	const char *sqlCreateTable = "CREATE TABLE tbl_Student (id STRING, name STRING, sex STRING, math INT, physical INT, chemistry INT, sum INT );";
	rc = sqlite3_exec(db, sqlCreateTable, NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 statement1: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Created tbl_Student.." << endl << endl;
	}
	return 0;
}
void check_ID(string id) {
	ostringstream check;
	check << "SELECT id FROM tbl_Student WHERE id = '" << id << "';";
	rc = sqlite3_exec(db, check.str().c_str(), NULL, NULL, &error);
	

}
void socket() {
	//Winsock Starup
	WSAData wsaData;//?
	WORD DllVersion = MAKEWORD(2, 2);//??
									 //if WSAStrartup returns anything other than0, the that means an error has occured in the WinSock Starup
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winsock starup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr);//length of the address (required for accept call)
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");//Address = localhost (this pc)
	addr.sin_port = htons(1111);//Port
	addr.sin_family = AF_INET;//IPv4 Socket

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);//Creatre socket to listen for new connections
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));//Bind the address to the socket
	listen(sListen, SOMAXCONN);//Places sListen socket in a state in which it is listening for an incoming connection. Note:SOMAXCONN = Socket Oustanding Max Connections

							   //SOCKET newConnection;//Socket to hold the client's connection (giu ket noi cac client)
	newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);//Accept a new connection


	if (newConnection == 0) {
		std::cout << "Failed to accept the client's connection" << std::endl;
	}
	else {
		std::cout << "Client Connected!!" << std::endl;
		char MOTD[256] = "==================MENU========================\n1. ADD\n2. XOA\n3. Search By ID\n"
			"4. Search By Subject\n5. Show\n6. Thong ke\n7. Sort_By_Sum\n8. Sort_By_Name\n9. Sort_By_ID\n10. Update\n"
			"==============================================\nChon chuc nang:";//Creadte buffer with message of the day
		send(newConnection, MOTD, sizeof(MOTD), NULL);//Send MOTD buffer
	}
}
void getData() {
	cout << "Retrieving values in tbl_Student ..." << endl;
	const char *sqlSelect = "SELECT * FROM tbl_Student;";
	char **results = NULL;
	int rows, columns;
	rc = sqlite3_get_table(db, sqlSelect, &results, &rows, &columns, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		// Display Table
		for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
		{
			for (int colCtr = 0; colCtr < columns; ++colCtr)
			{
				// Determine Cell Position
				int cellPosition = (rowCtr * columns) + colCtr;

				// Display Cell Value
				cout.width(12);
				cout.setf(ios::left);
				cout << results[cellPosition] << " ";
			}

			// End Line
			cout << endl;

			// Display Separator For Header
			if (0 == rowCtr)
			{
				for (int colCtr = 0; colCtr < columns; ++colCtr)
				{
					cout.width(12);
					cout.setf(ios::left);
					cout << "~~~~~~~~~~~~ ";
				}
				cout << endl;
			}
		}
	}
	sqlite3_free_table(results);

	// Close Database
	//cout << "Closing MyDb.db ..." << endl;
	sqlite3_close(db);
	//cout << "Closed MyDb.db" << endl << endl;
}

void insert() {
	char id[256], name[256], sex[256], math[256], physical[256], chemistry[256];
	char sendId[256] = "Nhap id: ";
	char sendName[256] = "Nhap ten: ";
	char sendSex[256] = "Nhap gioi tinh: ";
	char sendMath[256] = "Nhap diem toan: ";
	char sendPhysical[256] = "Nhap diem li: ";
	char sendChemistry[256] = "Nhap diem hoa: ";


	send(newConnection, sendId, sizeof(sendId), NULL);
	recv(newConnection, id, sizeof(id), NULL);
	string ID = (string)id;
	check_ID(ID);

	send(newConnection, sendName, sizeof(sendName), NULL);
	recv(newConnection, name, sizeof(name), NULL);
	string NAME = (string)name;
	//student.setName(name);

	send(newConnection, sendSex, sizeof(sendSex), NULL);
	recv(newConnection, sex, sizeof(sex), NULL);
	string SEX = (string)sex;
	//student.setSex(sex);

	send(newConnection, sendMath, sizeof(sendMath), NULL);
	recv(newConnection, math, sizeof(math), NULL);
	int m = atoi(math);
	//student.setMath(atoi(math));

	send(newConnection, sendPhysical, sizeof(sendPhysical), NULL);
	recv(newConnection, physical, sizeof(physical), NULL);
	int p = atoi(physical);
	//student.setPhysical(atoi(physical));

	send(newConnection, sendChemistry, sizeof(sendChemistry), NULL);
	recv(newConnection, chemistry, sizeof(chemistry), NULL);
	int c = atoi(chemistry);
	//student.setChemistry(atoi(physical));
	//student.setStudent(ID, NAME, SEX, m, p, c);
	int sum = m + p + c;
	ostringstream insertQuery;
	cout << "Inserting a value into tbl_Student ..." << endl;
	insertQuery << "INSERT INTO " << TBL_Student << "(" << St_ID << "," << St_NAME << "," << St_SEX << "," << St_MATH << "," << St_PHYSICAL << "," << St_CHEMISTRY"," << St_SUM << ")" << "VALUES ('" << ID << "','" << NAME << "','" << SEX << "','" << m << "','"
		<< p << "','" << c << "','" << sum << "');";

	rc = sqlite3_exec(db, insertQuery.str().c_str(), NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 statement2: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Inserted a value into tbl_Student." << endl << endl;
	}
	getData();
}

void delete_student() {
	char mess[256] = "Nhap id: ";
	char id[256];
	send(newConnection, mess, sizeof(mess), NULL);
	recv(newConnection, id, sizeof(id), NULL);

	string ID = (string)id;
	ostringstream deleteQuery;
	cout << "Deleting a value into tbl_Student ..." << endl;
	deleteQuery << "DELETE FROM tbl_Student WHERE id ='" << ID << "';";

	rc = sqlite3_exec(db, deleteQuery.str().c_str(), NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 statement2: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Deleted a value into tbl_Student." << endl << endl;
	}
	getData();
}
void search_by_id() {
	char search_id[256] = "Nhap id can tim: ";
	char id[256];
	send(newConnection, search_id, sizeof(search_id), NULL);
	recv(newConnection, id, sizeof(id), NULL);
	string ID = string(id);

	ostringstream search_ID;
	cout << "Searching values in tbl_Student ..." << endl;
	search_ID << "SELECT * FROM tbl_Student WHERE id = '" << ID << "';";
	char **results = NULL;
	int rows, columns;
	rc = sqlite3_get_table(db, search_ID.str().c_str(), &results, &rows, &columns, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		// Display Table
		for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
		{
			for (int colCtr = 0; colCtr < columns; ++colCtr)
			{
				// Determine Cell Position
				int cellPosition = (rowCtr * columns) + colCtr;

				// Display Cell Value
				cout.width(12);
				cout.setf(ios::left);
				cout << results[cellPosition] << " ";
			}

			// End Line
			cout << endl;

			// Display Separator For Header
			if (0 == rowCtr)
			{
				for (int colCtr = 0; colCtr < columns; ++colCtr)
				{
					cout.width(12);
					cout.setf(ios::left);
					cout << "~~~~~~~~~~~~ ";
				}
				cout << endl;
			}
		}
	}
	sqlite3_free_table(results);

	// Close Database
	//cout << "Closing MyDb.db ..." << endl;
	sqlite3_close(db);
}
void search_scores_subject() {
	char mess[256] = "\nSearch By Subject\n1. Math\n2. Physical\n3. Chemistry\n";
	char getInfo[256];
	char choose_scores[256] = "Nhap so diem: ";
	char get_scores[256];
	send(newConnection, mess, sizeof(mess), NULL);
	recv(newConnection, getInfo, sizeof(getInfo), NULL);
	string subject = string(getInfo);
	if (subject == "1") {
		send(newConnection, choose_scores, sizeof(choose_scores), NULL);
		recv(newConnection, get_scores, sizeof(get_scores), NULL);
		int scores = atoi(get_scores);

		ostringstream scores_math;
		scores_math << "SELECT *FROM tbl_Student WHERE math = '" << scores << "';";
		char **results = NULL;
		int rows, columns;
		rc = sqlite3_get_table(db, scores_math.str().c_str(), &results, &rows, &columns, &error);
		if (rc)
		{
			cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
			sqlite3_free(error);
		}
		else
		{
			// Display Table
			for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
			{
				for (int colCtr = 0; colCtr < columns; ++colCtr)
				{
					// Determine Cell Position
					int cellPosition = (rowCtr * columns) + colCtr;

					// Display Cell Value
					cout.width(12);
					cout.setf(ios::left);
					cout << results[cellPosition] << " ";
				}

				// End Line
				cout << endl;

				// Display Separator For Header
				if (0 == rowCtr)
				{
					for (int colCtr = 0; colCtr < columns; ++colCtr)
					{
						cout.width(12);
						cout.setf(ios::left);
						cout << "~~~~~~~~~~~~ ";
					}
					cout << endl;
				}
			}
		}
		sqlite3_free_table(results);

		// Close Database
		//cout << "Closing MyDb.db ..." << endl;
		sqlite3_close(db);
	}
	if (subject == "2") {
		send(newConnection, choose_scores, sizeof(choose_scores), NULL);
		recv(newConnection, get_scores, sizeof(get_scores), NULL);
		string scores = (string)get_scores;

		ostringstream scores_physical;
		scores_physical << "SELECT *FROM tbl_Student WHERE physical = '" << scores << "';";
		char **results = NULL;
		int rows, columns;
		rc = sqlite3_get_table(db, scores_physical.str().c_str(), &results, &rows, &columns, &error);
		if (rc)
		{
			cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
			sqlite3_free(error);
		}
		else
		{
			// Display Table
			for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
			{
				for (int colCtr = 0; colCtr < columns; ++colCtr)
				{
					// Determine Cell Position
					int cellPosition = (rowCtr * columns) + colCtr;

					// Display Cell Value
					cout.width(12);
					cout.setf(ios::left);
					cout << results[cellPosition] << " ";
				}

				// End Line
				cout << endl;

				// Display Separator For Header
				if (0 == rowCtr)
				{
					for (int colCtr = 0; colCtr < columns; ++colCtr)
					{
						cout.width(12);
						cout.setf(ios::left);
						cout << "~~~~~~~~~~~~ ";
					}
					cout << endl;
				}
			}
		}
		sqlite3_free_table(results);
		sqlite3_close(db);
	}
	if (subject == "3") {
		send(newConnection, choose_scores, sizeof(choose_scores), NULL);
		recv(newConnection, get_scores, sizeof(get_scores), NULL);
		string scores = (string)get_scores;

		ostringstream scores_chemistry;
		scores_chemistry << "SELECT *FROM tbl_Student WHERE chemistry = '" << scores << "';";
		char **results = NULL;
		int rows, columns;
		rc = sqlite3_get_table(db, scores_chemistry.str().c_str(), &results, &rows, &columns, &error);
		if (rc)
		{
			cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
			sqlite3_free(error);
		}
		else
		{
			// Display Table
			for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
			{
				for (int colCtr = 0; colCtr < columns; ++colCtr)
				{
					// Determine Cell Position
					int cellPosition = (rowCtr * columns) + colCtr;

					// Display Cell Value
					cout.width(12);
					cout.setf(ios::left);
					cout << results[cellPosition] << " ";
				}

				// End Line
				cout << endl;

				// Display Separator For Header
				if (0 == rowCtr)
				{
					for (int colCtr = 0; colCtr < columns; ++colCtr)
					{
						cout.width(12);
						cout.setf(ios::left);
						cout << "~~~~~~~~~~~~ ";
					}
					cout << endl;
				}
			}
		}
		sqlite3_free_table(results);
		sqlite3_close(db);
	}
}
extern "C" int callback_count(void* data, int count, char** rows, char**)
{
	if (count == 1 && rows) {
		*static_cast<int*>(data) = atoi(rows[0]);
		return 0;
	}
	return 1;
}

void thongke() {
	int count_St = 0;
	ostringstream count_student;
	count_student << "SELECT count(*) FROM tbl_Student;";
	rc = sqlite3_exec(db, count_student.str().c_str(), callback_count, &count_St, &error);
	if (rc) {
		cerr << "Error executing SQLite3 statement2: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else {
		cout << "So sinh vien: " << count_St << endl << endl;
	}

	int count_sum1 = 0;
	count_student << "SELECT count(*) FROM tbl_Student WHERE sum <= 15;";
	int count1 = sqlite3_exec(db, count_student.str().c_str(), callback_count, &count_sum1, &error);
	if (count1) {
		cerr << "Error executing SQLite3 statement2: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else {
		cout << "So sinh vien co tong diem <= 15: " << count_sum1 << endl << endl;
	}

	int count_sum2 = 0;
	count_student << "SELECT count(*) FROM tbl_Student WHERE sum >15 AND sum <= 20;";
	int count2 = sqlite3_exec(db, count_student.str().c_str(), callback_count, &count_sum2, &error);
	if (count2) {
		cerr << "Error executing SQLite3 statement2: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else {
		cout << "So sinh vien co tong diem > 15 && <=20: " << count_sum2 << endl << endl;
	}

	int count_sum3 = 0;
	count_student << "SELECT count(*) FROM tbl_Student WHERE sum >20 AND sum <= 25;";
	int count3 = sqlite3_exec(db, count_student.str().c_str(), callback_count, &count_sum3, &error);
	if (count3) {
		cerr << "Error executing SQLite3 statement2: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else {
		cout << "So sinh vien co tong diem >20 && <=25: " << count_sum3 << endl << endl;
	}

	int count_sum4 = 0;
	count_student << "SELECT count(*) FROM tbl_Student WHERE sum > 25;";
	int count4 = sqlite3_exec(db, count_student.str().c_str(), callback_count, &count_sum4, &error);
	if (count4) {
		cerr << "Error executing SQLite3 statement2: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else {
		cout << "So sinh vien co tong diem > 25: " << count_sum4 << endl << endl;
	}
}

void sort_by_sum() {
	ostringstream sort_scorse_sum;
	sort_scorse_sum << "SELECT *FROM tbl_Student ORDER BY sum ASC";
	char **results = NULL;
	int rows, columns;
	rc = sqlite3_get_table(db, sort_scorse_sum.str().c_str(), &results, &rows, &columns, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		// Display Table
		for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
		{
			for (int colCtr = 0; colCtr < columns; ++colCtr)
			{
				// Determine Cell Position
				int cellPosition = (rowCtr * columns) + colCtr;

				// Display Cell Value
				cout.width(12);
				cout.setf(ios::left);
				cout << results[cellPosition] << " ";
			}

			// End Line
			cout << endl;

			// Display Separator For Header
			if (0 == rowCtr)
			{
				for (int colCtr = 0; colCtr < columns; ++colCtr)
				{
					cout.width(12);
					cout.setf(ios::left);
					cout << "~~~~~~~~~~~~ ";
				}
				cout << endl;
			}
		}
	}
	sqlite3_free_table(results);
	sqlite3_close(db);
}
void sort_by_name() {
	ostringstream sort_scorse_name;
	sort_scorse_name << "SELECT *FROM tbl_Student ORDER BY name ASC";
	char **results = NULL;
	int rows, columns;
	rc = sqlite3_get_table(db, sort_scorse_name.str().c_str(), &results, &rows, &columns, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		// Display Table
		for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
		{
			for (int colCtr = 0; colCtr < columns; ++colCtr)
			{
				// Determine Cell Position
				int cellPosition = (rowCtr * columns) + colCtr;

				// Display Cell Value
				cout.width(12);
				cout.setf(ios::left);
				cout << results[cellPosition] << " ";
			}

			// End Line
			cout << endl;

			// Display Separator For Header
			if (0 == rowCtr)
			{
				for (int colCtr = 0; colCtr < columns; ++colCtr)
				{
					cout.width(12);
					cout.setf(ios::left);
					cout << "~~~~~~~~~~~~ ";
				}
				cout << endl;
			}
		}
	}
	sqlite3_free_table(results);
	sqlite3_close(db);
}

void sort_by_id() {
	ostringstream sort_scorse_id;
	sort_scorse_id << "SELECT *FROM tbl_Student ORDER BY id ASC";
	char **results = NULL;
	int rows, columns;
	rc = sqlite3_get_table(db, sort_scorse_id.str().c_str(), &results, &rows, &columns, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		// Display Table
		for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
		{
			for (int colCtr = 0; colCtr < columns; ++colCtr)
			{
				// Determine Cell Position
				int cellPosition = (rowCtr * columns) + colCtr;

				// Display Cell Value
				cout.width(12);
				cout.setf(ios::left);
				cout << results[cellPosition] << " ";
			}

			// End Line
			cout << endl;

			// Display Separator For Header
			if (0 == rowCtr)
			{
				for (int colCtr = 0; colCtr < columns; ++colCtr)
				{
					cout.width(12);
					cout.setf(ios::left);
					cout << "~~~~~~~~~~~~ ";
				}
				cout << endl;
			}
		}
	}
	sqlite3_free_table(results);
	sqlite3_close(db);
}
int main() {
	Student student;
	Connection();
	socket();

	char mess[256];
	char choose[256];

	while (true) {
		recv(newConnection, choose, sizeof(choose), NULL);
		string str = string(choose);
		if (str == "1") {
			insert();
		}
		if (str == "2") {
			delete_student();
		}
		if (str == "3") {
			search_by_id();
		}
		if (str == "4") {
			search_scores_subject();
		}
		if (str == "5") {
			rc = sqlite3_open(DATABASE, &db);
			getData();
		}
		if (str == "6") {
			thongke();
		}
		if (str == "7") {
			sort_by_sum();
		}
		if (str == "8") {
			sort_by_name();
		}
		if (str == "9") {
			sort_by_id();
		}
		if (str == "10") {
			char id[256] = "Nhap id:";
			char getId[256];
			send(newConnection, id, sizeof(id), NULL);
			recv(newConnection, getId, sizeof(getId), NULL);
			string ID = (string)getId;

			char name[256] = "Nhap ten:";
			char get_name[256];
			send(newConnection, name, sizeof(name), NULL);
			recv(newConnection, get_name, sizeof(get_name), NULL);
			string NAME = (string)get_name;

			char sex[256] = "Nhap gioi tinh: ";
			char get_sex[256];
			send(newConnection, sex, sizeof(sex), NULL);
			recv(newConnection, get_sex, sizeof(get_sex), NULL);
			string SEX = (string)get_sex;

			char math[256] = "Nhap diem math: ";
			char getMath[256];
			send(newConnection, math, sizeof(math), NULL);
			recv(newConnection, getMath, sizeof(getMath), NULL);
			int mat = atoi(getMath);


			char physical[256] = "Nhap diem physical: ";
			char get_physical[256];
			send(newConnection, physical, sizeof(physical), NULL);
			recv(newConnection, get_physical, sizeof(get_physical), NULL);
			int phy = atoi(get_physical);

			char chemistry[256] = "Nhap diem chemistry: ";
			char get_chemistry[256];
			send(newConnection, chemistry, sizeof(chemistry), NULL);
			recv(newConnection, get_chemistry, sizeof(get_chemistry), NULL);
			int chem = atoi(get_chemistry);

			int sum = mat + phy + chem;
			ostringstream update;
			update << "UPDATE tbl_Student SET name ='" << NAME << "', math = '" << mat << "',sex ='" << SEX << "', physical = '" << phy << "', chemistry = '" << chem <<
				"', sum = '" << sum << "' WHERE id = '" << ID << "';";
			rc = sqlite3_exec(db, update.str().c_str(), NULL, NULL, &error);
			if (rc) {
				cerr << "Error executing SQLite3 statement2: " << sqlite3_errmsg(db) << endl << endl;
				sqlite3_free(error);
			}
			else {
				cout << "OK!!" << endl << endl;
				getData();
			}
		}
	}
	system("pause");
	return 0;
}
