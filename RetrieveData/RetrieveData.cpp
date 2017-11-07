// RetrieveData.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sqlite3.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <ios>
#include <vector>
#include <fstream>

#define DB "roomStore.db"

using namespace std;

bool isDBOpen = false;
sqlite3 *db;
int rc;
char *error;

bool ConnectDB();
void DisconnectDB();
int addDataRow();
void getTableData();
int updateRow();
int deleteRow();
bool doesTableExist();

class Room
{
public:
	Room()
	{
		roomID = 0;
	}

	Room(int i, string name, string descrip)
	{
		roomID = i;
		roomName = name;
		description = descrip;
	}

	void setID(int i)
	{
		roomID = i;
	}

	int getID()
	{
		return roomID;
	}

	void setName(string name)
	{
		roomName = name;
	}

	string getName()
	{
		return roomName;
	}

	void setDescription(string descrip)
	{
		description = descrip;
	}

	string getDescription()
	{
		return description;
	}
private:
	int roomID;
	string roomName;
	string description;
};

int main()
{
	cout << "Opening roomStore.db" << endl;
	isDBOpen = ConnectDB();

	if (isDBOpen)
	{
		cout << "Connection Successful" << endl;
	}
	else
	{
		cout << "Connection Failed" << endl;
	}

	//cout << "Creating table" << endl;
	

	Room rm[10];

	while (1)
	{
		int choice;
		cout << "Select the operation you wish to carry out." << endl;
		cout << "1. Add New Room" << endl;
		cout << "2. Display list of rooms" << endl;
		cout << "3. Update room description" << endl;
		cout << "4. Delete Room" << endl;
		cout << "5. Exit" << endl;

		cout << "Your choice: ";
		cin >> choice;
		cout << endl;

		if (choice == 1)
		{
			int count = addDataRow();

			if (count == SQLITE_DONE)
			{
				cout << "Input successful" << endl;
			}
		}
		else if (choice == 2)
		{
			getTableData();
		}
		else if (choice == 3)
		{
			updateRow();
		}
		else if (choice == 4)
		{
			deleteRow();
		}
		else if (choice == 5)
		{
			break;
		}
		
	}

	DisconnectDB();


    return 0;
}

Room getRoom()
{
	Room rm;

	int id;
	string name;
	string descript;

	cout << "Enter room id: ";
	cin >> id;
	cout << endl;

	cout << "Enter name of room: ";
	cin >> name;
	cout << endl;

	cout << "Enter description of room: ";
	cin >> descript;
	cout << endl;

	rm.setID(id);
	rm.setName(name);
	rm.setDescription(descript);

	return rm;
}

int addDataRow()
{
	Room rm = getRoom();

	std::stringstream strm;
	strm << "Inserting values into room database: ID: " << rm.getID() << ", Name:" << rm.getName() << ", Description: " << rm.getDescription();

	string s = strm.str();
	char *str = &s[0];

	sqlite3_stmt *statement;
	int result = 1;

	char *query = str;
	{
		if (sqlite3_prepare(db, query, -1, &statement, 0) == SQLITE_OK)
		{
			int res = sqlite3_step(statement);
			result = res;
			sqlite3_finalize(statement);
		}
		return result;
	}


	return 0;
}

int updateRow()
{
	int roomNo;
	string newDescrip;

	cout << "Enter room ID number: ";
	cin >> roomNo;
	cout << endl;

	cout << "Enter updated description: ";
	cin >> newDescrip;
	cout << endl;

	std::stringstream strm;
	strm << "Updated Description: " << newDescrip << "for room number: " << roomNo;

	string s = strm.str();
	char *str = &s[0];

	sqlite3_stmt *statement;
	int result;

	char *query = str;
	{
		if (sqlite3_prepare(db, query, -1, &statement, 0) == SQLITE_OK)
		{
			int res = sqlite3_step(statement);
			result = res;
			sqlite3_finalize(statement);
		}
		return result;
	}

	
	return 0;
}

int deleteRow()
{
	int roomNo;

	cout << "Enter room ID number: ";
	cin >> roomNo;
	cout << endl;

	std::stringstream strm;
	strm << "Deleting room with ID number " << roomNo << "from room.";

	string s = strm.str();
	char *str = &s[0];

	sqlite3_stmt *statement;
	int result;
	char *query = str;
	{
		if (sqlite3_prepare(db, query, -1, &statement, 0) == SQLITE_OK)
		{
			int res = sqlite3_step(statement);
			result = res;
			sqlite3_finalize(statement);
		}
		return result;
	}
	
	return 0;
}

void getTableData()
{
	sqlite3_stmt *statement;

	char *query = "Select * from list";
	char **results = NULL;
	int rows, columns;

	if (sqlite3_prepare(db, query, -1, &statement, 0) == SQLITE_OK)
	{
		int ctotal = sqlite3_column_count(statement);
		int res = 0;

		while (1)
		{
			res = sqlite3_step(statement);

			if (res == SQLITE_ROW)
			{
				for (int i = 0; i < ctotal; i++)
				{
					string s = (char*)sqlite3_column_text(statement, i);
					cout << s << " ";
				}
				cout << endl;

				if (res == SQLITE_DONE)
				{
					cout << "DONE" << endl;
					break;
				}
			}
		}
	}
	else
	{
		cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		
	}
}

bool ConnectDB()
{
	if (sqlite3_open(DB, &db) == SQLITE_OK)
	{
		isDBOpen = true;
		return true;
	}
	return false;
}

void DisconnectDB()
{
	if (isDBOpen == true)
	{
		sqlite3_close(db);
	}
}

bool doesTableExist()
{

	return false;
}