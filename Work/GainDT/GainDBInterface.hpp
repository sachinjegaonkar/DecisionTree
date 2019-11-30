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
MYSQL_RES *res, *res1;    //Mysql ResultSet
MYSQL_ROW row, row1;     //Mysql Rows from Table
MYSQL_FIELD *fields;     //ResultSet metadata
my_bool  myb_Argu=TRUE;

/*--------------------------------------------------------------------------*/

/*==========================================================================*/
#define  r 0.301029
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
  double zOrder;
  double sumOrder;
}zSumOrder;
/*--------------------------------------------------------------------------*/
typedef struct
{
  string    szColumn;   //Column Name
  list<KeyValuePair> oKeyValuePair;  //Distinct Values in Column,

  list<KeyValuePair>::iterator keyValueIterator; //Iterator for above

  list<double>  classKeyCount;  //For each Column value: instances

  list<double>::iterator classKeyCountIterator; //Iterator for above

  //sum1=(-(a1/z1)*(log10(a1/z1)/r))+(-(a2/z1)*(log10(a2/z1)/r));
  list<zSumOrder>  zSumOrderList;  
  list<zSumOrder>::iterator zSumOrderListIterator;//Iterator for above

  double    entropy;   //Entropy value for the Column
  double    gain;    //Gain value for the Column
}ColumnList;
/*--------------------------------------------------------------------------*/

class DBINTERFACE_API Application //: public CDBInterface
{
public:

  /*==========================================================================*/

  void Initialize(char *szTable)
  {
    if(szTable[0])
      sprintf(Table, "%s", szTable);
    else
      sprintf(Table, "%s", "shop");
  }
  int  fnAPP_GetClassesFromTable(char *szDb, char *szTable);
  int  fnAPP_GetNumFields(char *Database, char *szTable);
  void fnAPP_CalculateExpectedInfo(void);
  char* fnAPP_CalculateGain(char *DB);

  /*==========================================================================*/

  void fnDB_InitDatabase(void);
  int  fnDB_Connect(char *DBase);
  int  fnDB_ExecuteQuery(char *szQuery);
  double fnDB_GetValue(char *szQuery, char *szColumn=NULL);
  void fnDB_CloseConnection(void);

  /*==========================================================================*/

public:
  char Table[10];
  double rows,columns;//static
  char szMaxGainClmName[20];
  /*--------------------------------------------------------------------------*/
  list<ClassType>    oClasses;
  list<ClassType>::iterator ClassIterator;
  /*--------------------------------------------------------------------------*/
  list<string>     szClasses;
  list<string>     szColumns;
  list<string>::const_iterator posClasses;
  list<string>::const_iterator posColumns;
  /*--------------------------------------------------------------------------*/
  list<ColumnList>   oColumnList;
  list<ColumnList>::iterator ColumnListIterator;
  /*--------------------------------------------------------------------------*/
};

#endif

