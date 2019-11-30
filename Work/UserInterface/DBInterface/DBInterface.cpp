#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "DBInterface.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved
                      )
{
  return TRUE;
}

void Application::fnDB_InitDatabase(void)
{
  mysql_init(&mysql);
  mysql_options(&mysql,MYSQL_OPT_RECONNECT,&myb_Argu);
}

int Application::fnDB_Connect(char szDatabase[])
{
  if(szDatabase[0])
  {
    if(!mysql_real_connect(&mysql, "localhost", "root", "ignou", szDatabase, 3306, NULL, CLIENT_MULTI_STATEMENTS))
    {
      return 0;
    }
    else 
      return 1;
  }
  return 0;
}

int Application::fnDB_ExecuteQuery(char *szQuery)
{
  int t=mysql_real_query(&mysql,szQuery,(unsigned int) strlen(szQuery));
  if (t)
  {
    return 0;
  }
  return 1;
}

double Application::fnDB_GetValue(char *szQuery, char *szColumn)
{
  if(fnDB_ExecuteQuery(szQuery) == 1)
  {
    res=mysql_store_result(&mysql);
    for(int i=0;i<(int)mysql_field_count(&mysql);i++)
    {
      row=mysql_fetch_row(res);
      if(row<0) break;
      for(int t=0;t<(int)mysql_num_fields(res);t++)
      {
        if(szColumn)
          sprintf(szColumn, "%s", row[t]);
        return (atoi(row[t]));//(int)mysql_num_fields(res);
      }
    }
  }
  return 0;
}

void Application::fnDB_CloseConnection(void)
{
  mysql_close(&mysql);
  mysql_library_end();
}

int Application::fnAPP_GetNoOfAttributes(char szDatabase[], char szTable[])
{
  char szQuery[256];
  sprintf(szQuery, "show columns from %s;", szTable);
  int rows, columns;

  fnDB_InitDatabase();

  if(fnDB_Connect(szDatabase) == 1)
    if(fnDB_ExecuteQuery(szQuery) == 1)
    {
      res=mysql_store_result(&mysql);
      rows = (unsigned long)mysql_num_rows(res);
      columns = (unsigned long)mysql_num_fields(res)-1; 
      fnDB_CloseConnection();
      return rows;
    }
    return 0;
}

int Application::fnAPP_GetNoOfRows(char szDatabase[], char szTable[])
{
  char szQuery[256];
  sprintf(szQuery, "select * from %s;", szTable);
  int rows, columns;

  fnDB_InitDatabase();

  if(fnDB_Connect(szDatabase) == 1)
    if(fnDB_ExecuteQuery(szQuery) == 1)
    {
      res=mysql_store_result(&mysql);
      rows = (unsigned long)mysql_num_rows(res);
      columns = (unsigned long)mysql_num_fields(res)-1; 
      fnDB_CloseConnection();
      return rows;
    }
    return 0;
}

int Application::fnAPP_GetNoOfClasses(char szDatabase[], char szTable[])
{
  char szQuery[256];
  sprintf(szQuery, "select distinct class from  %s;", szTable);
  int rows, columns;

  fnDB_InitDatabase();

  if(fnDB_Connect(szDatabase) == 1)
    if(fnDB_ExecuteQuery(szQuery) == 1)
    {
      res=mysql_store_result(&mysql);
      rows = (unsigned long)mysql_num_rows(res);
      columns = (unsigned long)mysql_num_fields(res)-1; 
      fnDB_CloseConnection();
      return rows;
    }
    return 0;
}

int Application::fnAPP_ListDatabases(char szDatabase[])
{
  char   szQuery[256], db[80];
  int    iCount=0;
  DB    database;
  TBL    table;

  sprintf(szQuery, "show databases;");
  fnDB_InitDatabase();

  if(fnDB_Connect(szDatabase) == 1)
    if(fnDB_ExecuteQuery(szQuery))
    {
      res=mysql_store_result(&mysql);
      for(int i=0;i<(int)mysql_num_rows(res);i++)
      {
        row=mysql_fetch_row(res);
        if(row != NULL)
        {
          iCount++;
          sprintf(db, "%s", row[0]);
          if(_stricmp("test", db) && _stricmp("mysql", db) && _stricmp("information_schema", db))
          {
            database.szDatabase = row[0];
            oDatabases.push_front(database);
          }
        }
      }
      fnDB_CloseConnection();
    }

    iCount=0;
    for(DBItr = oDatabases.begin(); DBItr != oDatabases.end(); DBItr++)
    {
      char   Db[80];
      DbTblRelation relation;
      relation.DBName = DBItr->szDatabase;//->c_str();

      sprintf(szQuery, "show tables;");
      fnDB_InitDatabase();

      sprintf(Db, "%s", DBItr->szDatabase.c_str());
      if(fnDB_Connect(Db) == 1)
        if(fnDB_ExecuteQuery(szQuery))
        {
          res=mysql_store_result(&mysql);
          for(int i=0;i<(int)mysql_num_rows(res);i++)
          {
            row=mysql_fetch_row(res);
            if(row != NULL)
            {
              iCount++;
              table.szTable = row[0];
              //oDBTables.push_front(table);
              relation.tables.push_front(table);
            }
          }
          oRelationship.push_front(relation);
        }
        fnDB_CloseConnection();
    }
    return iCount;
}

int Application::fnAPP_ListTables(char szDatabase[])
{
  char  szQuery[256];
  int   iCount=0;
  TBL   table;

  sprintf(szQuery, "show tables;");
  fnDB_InitDatabase();

  oTables.clear();
  if(fnDB_Connect(szDatabase) == 1)
  {
    if(fnDB_ExecuteQuery(szQuery))
    {
      res=mysql_store_result(&mysql);
      for(int i=0;i<(int)mysql_num_rows(res);i++)
      {
        row=mysql_fetch_row(res);
        if(row != NULL)
        {
          iCount++;
          table.szTable = row[0];
          oTables.push_front(table);
        }
      }
      fnDB_CloseConnection();
    }
  }
  return iCount;
}