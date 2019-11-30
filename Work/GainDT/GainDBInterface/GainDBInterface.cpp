// DBInterface.cpp : Defines the entry point for the DLL application.
//

#include "StdAfx.h"
#include <windows.h>
#include    <iostream>
#include    <math.h>
#include <stdlib.h>
#include "GainDBInterface.hpp"

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
      //  cout<<endl<<"Number of Class Available: "<<iCount<<endl;
      for(posClasses = szClasses.begin(); posClasses != szClasses.end(); posClasses++)
      {
        NodeClass.szClassName = posClasses->c_str();
        //   cout<<endl<<"Class: "<<NodeClass.szClassName.c_str();Table
        sprintf(szQuery, "select count(*) from %s where class = '%s'", szTable1, posClasses->c_str());
        NodeClass.iCount = fnDB_GetValue(szQuery);
        //   cout<<"\tInstances: "<<NodeClass.iCount<<endl;
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
      //  cout<<"Number of Columns: "<<columns<<endl<<endl;
      //  cout<<"Number of Rows: "<<rows<<endl<<endl;

      num=mysql_num_fields(res);
      for(int i=0;i<mysql_num_fields(res);i++)
      {
        fields=mysql_fetch_field_direct(res,i);
        szColumns.push_front(fields->name);
      }
      for(posColumns = szColumns.begin(); posColumns != szColumns.end(); posColumns++);
      //   cout<<"\tAttribute "<<(i++)+1 <<" is: "<<posColumns->c_str()<<endl;
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
void Application::fnAPP_CalculateExpectedInfo()
{
  for(ClassIterator = oClasses.begin(); ClassIterator != oClasses.end(); ClassIterator++)
    expt += (-((ClassIterator->iCount)/rows)*(log10((ClassIterator->iCount)/rows)/r));
  // cout<<endl<<"Expected Information Needed To Classify Is = " <<expt <<" bits/message"<<endl<<endl<<endl;
}
/*==========================================================================*/

/*
*
*
*
*/
/*--------------------------------------------------------------------------*/
char* Application::fnAPP_CalculateGain(char *DB)
{
  char szQuery[500];

  fnDB_InitDatabase();
  if(fnDB_Connect(DB) == 1)
  {
    for(posColumns = szColumns.begin(); posColumns != szColumns.end(); posColumns++)
    {
      ColumnList  columnInfo;
      if(stricmp("class", posColumns->c_str()) == 0)
        continue;
      columnInfo.szColumn = posColumns->c_str();

      // cout<<endl<<endl<<"--------------------------------------------------------------"<<endl;

      // cout<<endl<<"\t--------------------Atrributes:Start---------------------"<<endl;
      //   cout<<"\tFor Attribute: "<<posColumns->c_str()<<endl;

      //"select distinct column from Table;
      sprintf(szQuery, "select distinct %s from %s;", posColumns->c_str(), Table);
      if(fnDB_ExecuteQuery(szQuery))
      {
        res1=mysql_store_result(&mysql);
        //    cout<<endl<<"\t---------------Values:Start----------------"<<endl;
        //    cout<<endl<<"\t\tValues are:"<<endl;
        for(int i=0;i<(int)mysql_num_rows(res1);i++)
        {
          double   sTemp=0;
          row1=mysql_fetch_row(res1);
          if(row1 != NULL)
          {
            KeyValuePair keyValue;
            ClassType  NodeClass;
            zSumOrder  ozSumOrder;
            double   zValue; 
            double   aValue;

            keyValue.Key = row1[0];
            string szBuffer = row1[0];
            //      cout<<"\t\t"<<row1[0];

            //"select COUNT(*) from table where columnName = value;
            sprintf(szQuery, "select COUNT(*) from %s where %s= '%s'", Table, posColumns->c_str(),row1[0]);
            zValue = fnDB_GetValue(szQuery);
            keyValue.Value = zValue;
            ozSumOrder.zOrder = zValue;
            columnInfo.oKeyValuePair.push_front(keyValue);
            //      cout<<"\tInstances: "<<zValue<<endl;//fnDB_GetValue(szQuery)

            //      cout<<endl<<"\t----------Classes:Start-----------"<<endl;
            //      cout<<"\t\t\tClasses are: "<<endl;
            for(posClasses = szClasses.begin(); posClasses != szClasses.end(); ++posClasses)
            {
              //"select COUNT(*) from table where columnName = value and class = className;
              sprintf(szQuery, "select count(*) from %s where %s='%s' and class='%s';", Table, posColumns->c_str(),szBuffer.c_str(), posClasses->c_str());
              //       cout<<"\t\t\t\t"<<posClasses->c_str();
              aValue = fnDB_GetValue(szQuery);
              columnInfo.classKeyCount.push_front(aValue);
              //        cout<<"\tClassInstances: "<<aValue<<endl;
              //sum1=(-(a1/z1)*(log10(a1/z1)/r))+(-(a2/z1)*(log10(a2/z1)/r));
              if(aValue) 
                sTemp += (-(aValue/zValue)*(log10(aValue/zValue)/r));
            }
            //      cout<<endl<<"\t----------Classes:End-----------"<<endl;
            ozSumOrder.sumOrder = sTemp;
            columnInfo.zSumOrderList.push_front(ozSumOrder);
          }
        }
        //    cout<<endl<<"\t---------------Values:End----------------"<<endl;
      }
      oColumnList.push_front(columnInfo);

      // cout<<endl<<"\t--------------------Atrributes:End---------------------"<<endl;
    }
    fnDB_CloseConnection();
    double greater=0;
    for( posColumns = szColumns.begin(); posColumns != szColumns.end(); posColumns++)
    {
      double entropy=0;
      if(strcmp(posColumns->c_str(),"class") == 0)
        continue;
      for( ColumnListIterator = oColumnList.begin(); ColumnListIterator != oColumnList.end(); ColumnListIterator++ )
      {
        //cout<<endl<<posColumns->c_str()<<" ~~~~ "<<ColumnListIterator->szColumn.c_str()<<endl;
        if(strcmp(posColumns->c_str(), ColumnListIterator->szColumn.c_str()) == 0)
        {
          for( ColumnListIterator->zSumOrderListIterator = ColumnListIterator->zSumOrderList.begin(); ColumnListIterator->zSumOrderListIterator != ColumnListIterator->zSumOrderList.end(); ColumnListIterator->zSumOrderListIterator++ )
          {
            //      cout<<endl<<"Sum:"<<ColumnListIterator->zSumOrderListIterator->sumOrder<<endl<<"Z:"<<ColumnListIterator->zSumOrderListIterator->zOrder<<endl;
            entropy += ((ColumnListIterator->zSumOrderListIterator->zOrder)/rows)*(ColumnListIterator->zSumOrderListIterator->sumOrder);
          }
          ColumnListIterator->entropy = entropy;
          ColumnListIterator->gain = expt-entropy;
          //     cout<<endl<<"Entropy of Class \""<<posColumns->c_str()<<"\" is => "<<entropy<<endl;
          //     cout<<endl<<"Gain for the Class \""<<posColumns->c_str()<<"\" is => "<<(expt-entropy)<<endl;
          if(greater < (expt-entropy))
          {
            greater = (expt-entropy);
            sprintf(szMaxGainClmName, "%s", posColumns->c_str());
          }
        }
      }
    }
    //  cout<<endl<<"\""<<szMaxGainClmName<<"\""<<" column is having Maximum Gain: "<<greater<<endl;
  }
  return szMaxGainClmName;
}
/*==========================================================================*/
