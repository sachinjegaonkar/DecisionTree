#ifndef		_DBINTERFACE_H
#define		_DBINTERFACE_H

#pragma		warning(disable:4786)
#pragma		warning(disable:4251)
/*--------------------------------------------------------------------------*/

#include	"StdAfx.h"
#include	<windows.h>
#include	<string>
#include	<list>
#include	<winsock.h>
#include	"MySQL\include\mysql.h"
#include	"MySQL\include\my_alloc.h"
#include	"MySQL\include\my_list.h"
#include	"MySQL\include\mysql_time.h"
#include	"MySQL\include\mysql_version.h"
#include	"MySQL\include\typelib.h"

/*==========================================================================*/

#ifdef	DBINTERFACE_EXPORTS
#define DBINTERFACE_API __declspec(dllexport)
#else
#define DBINTERFACE_API __declspec(dllimport)
#endif

/*--------------------------------------------------------------------------*/

using namespace std;

/*--------------------------------------------------------------------------*/

MYSQL		mysql;						//Mysql Connector
MYSQL_RES	*res, *res1;				//Mysql ResultSet
MYSQL_ROW	row, row1;					//Mysql Rows from Table
MYSQL_FIELD *fields;					//ResultSet metadata
my_bool		myb_Argu=TRUE;

/*--------------------------------------------------------------------------*/

typedef struct 
{
	string	szDatabase;
}DB;
/*--------------------------------------------------------------------------*/
typedef struct
{
	string	szTable;
}TBL;
/*--------------------------------------------------------------------------*/
typedef struct
{
	string		DBName;
	list<TBL>	tables;
}DbTblRelation;
/*--------------------------------------------------------------------------*/

class DBINTERFACE_API Application //: public CDBInterface
{
public:

/*==========================================================================*/

	int		fnAPP_ListDatabases(char szDatabase[]);
	int		fnAPP_ListTables(char szDatabase[]);
	int		fnAPP_GetNoOfAttributes(char szDatabase[], char szTable[]);
	int		fnAPP_GetNoOfRows(char szDatabase[], char szTable[]);
	int		fnAPP_GetNoOfClasses(char szDatabase[], char szTable[]);

/*==========================================================================*/

	void	fnDB_InitDatabase(void);
	int		fnDB_Connect(char szDatabase[]);
	int		fnDB_ExecuteQuery(char *szQuery);
	double	fnDB_GetValue(char *szQuery, char *szColumn=NULL);
	void	fnDB_CloseConnection(void);

/*==========================================================================*/

public:
	double rows,columns;//static
/*--------------------------------------------------------------------------*/
	list<DB>				oDatabases;
	list<DB>::iterator		DBItr;
/*--------------------------------------------------------------------------*/
	list<TBL>				oDBTables, oTables;
	list<TBL>::iterator		TBLItr;
/*--------------------------------------------------------------------------*/
	list<DbTblRelation>					oRelationship;
	list<DbTblRelation>::iterator		RelationItr;
/*--------------------------------------------------------------------------*/
};

#endif

