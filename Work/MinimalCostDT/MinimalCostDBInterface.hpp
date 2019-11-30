#ifndef  _DBINTERFACE_H
#define  _DBINTERFACE_H

#pragma  warning(disable:4786)
#pragma  warning(disable:4251)
#include <string>
#include <list>
#include <winsock.h>
#include "MySQL\include\mysql.h"
#include "MySQL\include\my_alloc.h"
#include "MySQL\include\my_list.h"
#include "MySQL\include\mysql_time.h"
#include "MySQL\include\mysql_version.h"
#include "MySQL\include\typelib.h"


/*--------------------------------------------------------------------------*/

#ifdef DBINTERFACE_EXPORTS
#define DBINTERFACE_API __declspec(dllexport)
#else
#define DBINTERFACE_API __declspec(dllimport)
#endif

/*--------------------------------------------------------------------------*/
MYSQL  mysql;      //Mysql Connector
MYSQL_RES *res, *res1, *cost_res;  //Mysql ResultSet
MYSQL_ROW row, row1, cost_row;  //Mysql Rows from Table
MYSQL_FIELD *fields;     //ResultSet metadata
my_bool  myb_Argu=TRUE;

/*--------------------------------------------------------------------------*/

/*==========================================================================*/
#define  FP 200
#define  FN 600
/*==========================================================================*/

using namespace std;

/*==========================================================================*/

double DBINTERFACE_API expt;

/*--------------------------------------------------------------------------*/
typedef struct 
{
  string szClassName;
  int  iCount;
}ClassType;
/*--------------------------------------------------------------------------*/
typedef struct
{
  string Key;
  double Value;
}KeyValuePair;
/*--------------------------------------------------------------------------*/
typedef struct
{
  double P1;
  double N1;
}PNJunction;
/*--------------------------------------------------------------------------*/
typedef struct
{
  string    szValue;  //Column Value
  PNJunction   pnValue;
  double    iMissClassificationCost;
}ValueDetails;
/*--------------------------------------------------------------------------*/
typedef struct
{
  string    szColumn;     //ColumnName
  list<KeyValuePair> oKeyValuePair;    //Distinct Values in Column,
  list<KeyValuePair>::iterator keyValueIterator; //Iterator for above

  list<ValueDetails> oDetails;     //ColumnName:(ClassName::Instaces):MissClnCost
  list<ValueDetails>::iterator Iter;
  double    iTotalMissCCost;
  double    iTestCost;
  double    iTotalCost; 
}ColumnList;
/*--------------------------------------------------------------------------*/
typedef struct
{
  string szColumn;
  double iCost;
}CostInfo;
/*--------------------------------------------------------------------------*/

class DBINTERFACE_API Application //: public CDBInterface
{
public:

  /*==========================================================================*/

  void Initialize(char *szTable)
  {
    C = 20;
    if(szTable[0])
      sprintf(Table, "%s", szTable);
    else
      sprintf(Table, "%s", "shop");
  }
  int  fnAPP_GetClassesFromTable(char *szDb, char *szTable);
  int  fnAPP_GetNumFields(char *Database, char *szTable);
  char* fnAPP_CalculateLowestValue(char *DB);
  void fnAPP_GetCostInfo(char *Database);
  /*==========================================================================*/

  void fnDB_InitDatabase(void);
  int  fnDB_Connect(char *DBase);
  int  fnDB_ExecuteQuery(char *szQuery);
  double fnDB_GetValue(char *szQuery, char *szColumn=NULL);
  void fnDB_ReadCostInfo(char *szTable);
  void fnDB_CloseConnection(void);

  /*==========================================================================*/

public:
  char Table[10];
  double C;
  double rows,columns;//static
  char szLowestValueClmName[20];
  /*--------------------------------------------------------------------------*/
  list<ClassType>    oClasses;
  list<ClassType>::iterator ClassIterator;
  /*--------------------------------------------------------------------------*/
  list<string>     szClasses;
  list<string>     szColumns;
  list<string>::const_iterator posClasses;
  list<string>::const_iterator posColumns;
  /*--------------------------------------------------------------------------*/
  list<ColumnList>    oColumnList;
  list<ColumnList>::iterator  ColumnListIterator;
  /*--------------------------------------------------------------------------*/
  list<CostInfo>     oCostInfo;
  list<CostInfo>::iterator  CostInfoItr;
  /*--------------------------------------------------------------------------*/
};

#endif

