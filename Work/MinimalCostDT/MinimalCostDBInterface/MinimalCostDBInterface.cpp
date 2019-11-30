// DBInterface.cpp : Defines the entry point for the DLL application.
//

#include "StdAfx.h"
#include <windows.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "MinimalCostDBInterface.hpp"

BOOL APIENTRY DllMain( HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved
                      )
{
  return TRUE;
}

/*==========================================================================*/
/*
*
*
*
*/
/*--------------------------------------------------------------------------*/
void Application::fnDB_InitDatabase(void)
{
  mysql_init(&mysql);
  mysql_options(&mysql,MYSQL_OPT_RECONNECT,&myb_Argu);
}
/*==========================================================================*/

/*
*
*
*
*/
/*--------------------------------------------------------------------------*/
int Application::fnDB_Connect(char *DBase)
{//car medical
  if(!mysql_real_connect(&mysql, "localhost", "root", "ignou", DBase, 3306, NULL, CLIENT_MULTI_STATEMENTS))
  {
    //  cout<<"Error connecting to database "<<endl;
    return 0;
  }
  else 
    return 1;
}
/*==========================================================================*/

/*
*
*
*
*/
/*--------------------------------------------------------------------------*/
int Application::fnDB_ExecuteQuery(char *szQuery)
{
  int t=mysql_real_query(&mysql,szQuery,(unsigned int) strlen(szQuery));
  if (t)
  {
    //  cout<<"Error making query: "<<endl;
    return 0;
  }
  return 1;
}
/*==========================================================================*/

/*
*
*
*
*/
/*--------------------------------------------------------------------------*/
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
/*==========================================================================*/

/*
*
*
*
*/
/*--------------------------------------------------------------------------*/
void Application::fnDB_CloseConnection(void)
{
  mysql_close(&mysql);
  mysql_library_end();
}
/*==========================================================================*/
/*
*
*
*
*/
/*--------------------------------------------------------------------------*/
int Application::fnAPP_GetClassesFromTable(char *szDb, char *szTable1)
{
  char  szQuery[256];
  int   iCount=0;
  ClassType NodeClass;

  sprintf(szQuery, "select distinct class from %s;", szTable1);
  fnDB_InitDatabase();

  if(fnDB_Connect(szDb) == 1)
    if(fnDB_ExecuteQuery(szQuery))
    {
      res=mysql_store_result(&mysql);
      for(int i=0;i<(int)mysql_num_rows(res);i++)
      {
        row=mysql_fetch_row(res);
        if(row != NULL)
        {
          iCount++;
          szClasses.push_front(row[0]);
        }
      }
      //cout<<endl<<"Number of Class Available: "<<iCount<<endl;
      for(posClasses = szClasses.begin(); posClasses != szClasses.end(); posClasses++)
      {
        NodeClass.szClassName = posClasses->c_str();
        //cout<<endl<<"Class: "<<NodeClass.szClassName.c_str();Table
        sprintf(szQuery, "select count(*) from %s where class = '%s'", szTable1, posClasses->c_str());
        NodeClass.iCount = fnDB_GetValue(szQuery);
        //cout<<"\tInstances: "<<NodeClass.iCount<<endl;
        oClasses.push_front(NodeClass);
      }
      fnDB_CloseConnection();
    }
    return iCount;
}
/*==========================================================================*/

/*
*
*
*
*/
/*--------------------------------------------------------------------------*/
int Application::fnAPP_GetNumFields(char *Database, char *szTable11)
{
  char szQuery[256];
  sprintf(szQuery, "select * from %s", szTable11);
  int num;

  fnDB_InitDatabase();

  if(fnDB_Connect(Database) == 1)
    if(fnDB_ExecuteQuery(szQuery) == 1)
    {
      //res=mysql_use_result(&mysql);
      res=mysql_store_result(&mysql);
      rows = (unsigned long)mysql_num_rows(res);
      columns = (unsigned long)mysql_num_fields(res)-1; 
      //cout<<"Number of Columns: "<<columns<<endl<<endl;
      //cout<<"Number of Rows: "<<rows<<endl<<endl;

      num=mysql_num_fields(res);
      for(int i=0;i<mysql_num_fields(res);i++)
      {
        fields=mysql_fetch_field_direct(res,i);
        szColumns.push_front(fields->name);
      }
      for(posColumns = szColumns.begin(); posColumns != szColumns.end(); posColumns++);
      //cout<<"\tAttribute "<<(i++)+1 <<" is: "<<posColumns->c_str()<<endl;
      fnDB_CloseConnection();
    }
    return num+1;
}
/*==========================================================================*/

/*
*
*
*
*/
/*--------------------------------------------------------------------------*/
void Application::fnAPP_GetCostInfo(char *Database)
{
  char szQuery[500];

  fnDB_InitDatabase();
  if(fnDB_Connect(Database) == 1)
  {
    sprintf(szQuery, "select * from %s_cost", Database);
    if(fnDB_ExecuteQuery(szQuery))
    {
      CostInfo c_Info;//cost_res
      res1 = mysql_store_result(&mysql);
      oCostInfo.clear();
      for(int i=0;i<(int)mysql_num_rows(res1);i++)
      {
        row1 = mysql_fetch_row(res1);
        c_Info.szColumn = row1[0];
        c_Info.iCost = (double)atoi(row1[1]);
        oCostInfo.push_front(c_Info);
      }
    }
    fnDB_CloseConnection();
  }
}
/*==========================================================================*/

/*
*
*
*
*/
/*--------------------------------------------------------------------------*/
char* Application::fnAPP_CalculateLowestValue(char *DB)
{
  char szQuery[500];

  fnDB_InitDatabase();
  if(fnDB_Connect(DB) == 1)
  {
    for(posColumns = szColumns.begin(); posColumns != szColumns.end(); posColumns++)
    {
      ColumnList  columnInfo;
      double   TotalMissCCost=0;
      double   TestCost=0;

      if(stricmp("class", posColumns->c_str()) == 0)
        continue;

      for(CostInfoItr = oCostInfo.begin(); CostInfoItr != oCostInfo.end(); CostInfoItr++)
        if(stricmp(posColumns->c_str(), CostInfoItr->szColumn.c_str()) == 0)
          C = CostInfoItr->iCost;

      columnInfo.szColumn = posColumns->c_str(); //ColumnName

      //"select distinct columnValue from Table;
      sprintf(szQuery, "select distinct %s from %s;", posColumns->c_str(), Table);
      if(fnDB_ExecuteQuery(szQuery))
      {
        res1=mysql_store_result(&mysql);
        //cout<<endl<<"\t\tValues are:"<<endl;
        for(int i=0;i<(int)mysql_num_rows(res1);i++)
        {
          double   sTemp=0;
          row1=mysql_fetch_row(res1);
          if(row1 != NULL)
          {
            KeyValuePair keyValue;
            ValueDetails valueDetails;
            PNJunction  pnjunction;
            string   szBuffer;

            keyValue.Key = row1[0];
            szBuffer = row1[0]; //value
            valueDetails.szValue = szBuffer;//row1[0];

            sprintf(szQuery, "select COUNT(*) from %s where %s= '%s'", Table, posColumns->c_str(),row1[0]);
            keyValue.Value = fnDB_GetValue(szQuery);

            columnInfo.oKeyValuePair.push_front(keyValue);
            int i;
            for(i=0, posClasses = szClasses.begin(); (i < 2) && (posClasses != szClasses.end()); ++posClasses, i++)
            {
              //"select COUNT(*) from table where columnName = value and class = className;
              sprintf(szQuery, "select count(*) from %s where %s='%s' and class='%s';", Table, posColumns->c_str(),szBuffer.c_str(), posClasses->c_str());

              if(i == 0)
                pnjunction.P1 = fnDB_GetValue(szQuery);
              else if(i == 1)
                pnjunction.N1 = fnDB_GetValue(szQuery);
            }
            valueDetails.pnValue.P1 = pnjunction.P1;
            valueDetails.pnValue.N1 = pnjunction.N1;

            if(!_stricmp(valueDetails.szValue.c_str(), "?") || ((valueDetails.pnValue.P1)*FN > (valueDetails.pnValue.N1)*FP))
              valueDetails.iMissClassificationCost = FP*(valueDetails.pnValue.N1);
            else if((valueDetails.pnValue.P1)*FN < (valueDetails.pnValue.N1)*FP)
              valueDetails.iMissClassificationCost = FN*(valueDetails.pnValue.P1);

            TotalMissCCost += valueDetails.iMissClassificationCost;

            columnInfo.oDetails.push_front(valueDetails);
          }
        }
        columnInfo.iTotalMissCCost = TotalMissCCost;

        for(columnInfo.Iter = columnInfo.oDetails.begin();
          columnInfo.Iter != columnInfo.oDetails.end();
          columnInfo.Iter++)
        {
          //TotalMissCCost += columnInfo.Iter->iMissClassificationCost;
          TestCost += (columnInfo.Iter->pnValue.P1) + (columnInfo.Iter->pnValue.N1);
        }
        columnInfo.iTestCost = TestCost*C;
        columnInfo.iTotalCost = (TestCost*C) + TotalMissCCost;
      }
      oColumnList.push_front(columnInfo);
    }
    fnDB_CloseConnection();
    double lower=0;
    for( posColumns = szColumns.begin(); posColumns != szColumns.end(); posColumns++)
    {
      if(strcmp(posColumns->c_str(),"class") == 0)
        continue;

      for( ColumnListIterator = oColumnList.begin(); ColumnListIterator != oColumnList.end(); ColumnListIterator++ )
      {
        //cout<<endl<<posColumns->c_str()<<" ~~~~ "<<ColumnListIterator->szColumn.c_str()<<endl;
        if(strcmp(posColumns->c_str(), ColumnListIterator->szColumn.c_str()) == 0)
        {
          if(lower < (ColumnListIterator->iTotalCost))
          {
            lower = ColumnListIterator->iTotalCost;
            sprintf(szLowestValueClmName, "%s", posColumns->c_str());
          }
        }
      }
    }
    //cout<<endl<<"\""<<szLowestValueClmName<<"\""<<" column is having Lowest Total Cost: "<<lower<<endl;
  }
  return szLowestValueClmName;
}
/*==========================================================================*/
