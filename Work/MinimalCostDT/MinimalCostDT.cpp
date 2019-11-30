#include <iostream>
#include <fstream>
#include <conio.h>
#include "MinimalCostDT.hpp"

extern void fn_GetCostInfo(char *Database);


/*==========================================================================*/
/*
*
*
*
*/
/*--------------------------------------------------------------------------*/

void CollectAllInformation(char szDatabase[], char szTable[], char *szMinimumCost)
{
  try
  {
    Application DecisionTree;
    int  iBufferCount=0;
    static char tble[20];
    char szTempTable[20], szLeafNode[20], szValue[20],szQueryBuffer[256];
    int PValue=0, NValue=0;
    string P,N;

    strcpy(tble, szTable);

    DecisionTree.Initialize(szTable);
    DecisionTree.fnAPP_GetCostInfo(Database);

    DecisionTree.fnAPP_GetClassesFromTable(szDatabase, szTable);
    DecisionTree.fnAPP_GetNumFields(szDatabase, szTable);

    sprintf(szMinimumCost, "%s", DecisionTree.fnAPP_CalculateLowestValue(szDatabase));

    DecisionTree.ClassIterator = DecisionTree.oClasses.begin();
    NValue = DecisionTree.ClassIterator->iCount;
    N  = DecisionTree.ClassIterator->szClassName;
    DecisionTree.ClassIterator++;
    PValue = DecisionTree.ClassIterator->iCount;
    P  = DecisionTree.ClassIterator->szClassName;

    if(szMinimumCost[0])
    {
    }
    else
      return;

    DecisionTree.fnDB_InitDatabase();
    if(DecisionTree.fnDB_Connect(szDatabase) == 1)
      //iterate for MinimumCost Column value column
      for(DecisionTree.ColumnListIterator = DecisionTree.oColumnList.begin();
        DecisionTree.ColumnListIterator != DecisionTree.oColumnList.end();
        DecisionTree.ColumnListIterator++)
      {
        if(stricmp(DecisionTree.ColumnListIterator->szColumn.c_str(), szMinimumCost) == 0)
        {
          sprintf(szBuffer, "<%s::P_%d:N_%d>\n", szMinimumCost, PValue, NValue);
          WriteToFile(szBuffer);
          //iterate for distinct values for resultant gain value
          sprintf(szValue, "%s", szMinimumCost);
          if(stricmp(szValue, "?"))
            for(DecisionTree.ColumnListIterator->keyValueIterator = DecisionTree.ColumnListIterator->oKeyValuePair.begin();
              DecisionTree.ColumnListIterator->keyValueIterator != DecisionTree.ColumnListIterator->oKeyValuePair.end();
              DecisionTree.ColumnListIterator->keyValueIterator++)
            {
              szQueryBuffer[0]=0;
              szTempTable[0]=0;
              iBufferCount=0;
              sprintf(szTempTable, "%s%d", szValue, ++iViewCount);//tble, ++iViewCount);//
              iBufferCount += sprintf(szQueryBuffer+iBufferCount, "create view %s as select ", szTempTable);

              //column select iterator
              for(DecisionTree.posColumns = DecisionTree.szColumns.begin(); 
                DecisionTree.posColumns != DecisionTree.szColumns.end(); 
                DecisionTree.posColumns++)
              {
                if(stricmp(szValue, DecisionTree.posColumns->c_str()))
                  iBufferCount += sprintf(szQueryBuffer+iBufferCount, "%s, ", DecisionTree.posColumns->c_str());
              }

              //query build for perticular sub table to create.tble
              iBufferCount += sprintf(szQueryBuffer+iBufferCount-2, " from %s where %s ='%s';", szTable, szValue, DecisionTree.ColumnListIterator->keyValueIterator->Key.c_str());
              szQueryBuffer[iBufferCount]=0;
              printf("%s\n\n", szQueryBuffer);


              if(stricmp(DecisionTree.ColumnListIterator->keyValueIterator->Key.c_str(), "?"))
              {
                if(DecisionTree.fnDB_ExecuteQuery(szQueryBuffer))
                {
                  sprintf(szBuffer, "<%s>\n", DecisionTree.ColumnListIterator->keyValueIterator->Key.c_str());
                  //sprintf(szBuffer, "<%s::P_%d:N_%d>\n", DecisionTree.ColumnListIterator->keyValueIterator->Key.c_str(), PValue, NValue);
                  WriteToFile(szBuffer);

                  sprintf(szQueryBuffer, "select count(class) from %s;", szTempTable);
                  int instances=0;
                  instances = (int)DecisionTree.fnDB_GetValue(szQueryBuffer);

                  //subtable created successfully.
                  //call same function for this sub table.
                  sprintf(szQueryBuffer, "select count(distinct class) from %s;", szTempTable);
                  if((DecisionTree.fnDB_GetValue(szQueryBuffer)) > 1)
                  {
                    CollectAllInformation(szDatabase, szTempTable, szMinimumCost);

                    sprintf(szQueryBuffer, "drop view %s;", szTempTable);
                    DecisionTree.fnDB_ExecuteQuery(szQueryBuffer);
                    sprintf(szBuffer, "</%s>\n", DecisionTree.ColumnListIterator->keyValueIterator->Key.c_str());
                    //sprintf(szBuffer, "</%s::P_%d:N_%d>\n", DecisionTree.ColumnListIterator->keyValueIterator->Key.c_str(), PValue, NValue);
                    WriteToFile(szBuffer);
                  }
                  else
                  {
                    sprintf(szQueryBuffer, "select distinct class from %s;", szTempTable);
                    DecisionTree.fnDB_GetValue(szQueryBuffer, szLeafNode);

                    sprintf(szQueryBuffer, "drop view %s;", szTempTable);
                    DecisionTree.fnDB_ExecuteQuery(szQueryBuffer);

                    if(!stricmp(P.c_str(), szLeafNode))
                    {
                      sprintf(szBuffer, "<P::P_%d:N_0>", instances);
                      WriteToFile(szBuffer);
                      sprintf(szBuffer, "</P::P_%d:N_0>\n", instances);
                      WriteToFile(szBuffer);
                    }
                    else if(!stricmp(N.c_str(), szLeafNode))
                    {
                      sprintf(szBuffer, "<N::P_0:N_%d>", instances);
                      WriteToFile(szBuffer);
                      sprintf(szBuffer, "</N::P_0:N_%d>\n", instances);
                      WriteToFile(szBuffer);
                    }
                    sprintf(szBuffer, "</%s>\n", DecisionTree.ColumnListIterator->keyValueIterator->Key.c_str());
                    //sprintf(szBuffer, "</%s::P_%d:N_%d>\n", DecisionTree.ColumnListIterator->keyValueIterator->Key.c_str(), PValue, NValue);
                    WriteToFile(szBuffer);
                  }
                }
              }
              else
              {
                sprintf(szBuffer, "<%s>\n", DecisionTree.ColumnListIterator->keyValueIterator->Key.c_str());
                WriteToFile(szBuffer);
                if(PValue > NValue)
                {
                  sprintf(szBuffer, "<P::P_%d:N_%d>\n", PValue, NValue);
                  WriteToFile(szBuffer);
                  sprintf(szBuffer, "</P::P_%d:N_%d>\n", PValue, NValue);
                  WriteToFile(szBuffer);
                }
                else
                {
                  sprintf(szBuffer, "<N::P_%d:N_%d>\n", PValue, NValue);
                  WriteToFile(szBuffer);
                  sprintf(szBuffer, "</N::P_%d:N_%d>\n", PValue, NValue);
                  WriteToFile(szBuffer);
                }
                sprintf(szBuffer, "</%s>\n", DecisionTree.ColumnListIterator->keyValueIterator->Key.c_str());
                WriteToFile(szBuffer);
              }
            }
            sprintf(szBuffer, "</%s::P_%d:N_%d>\n", DecisionTree.ColumnListIterator->szColumn.c_str(), PValue, NValue);
            WriteToFile(szBuffer);
            return;
        }
      }
  }
  catch(...)
  {
    printf("Too many connctions to the Database.\n");
  }
}
/*--------------------------------------------------------------------------*/
void WriteToFile(char szBuffer[], char szMode[])
{
  sprintf(szFile, "%s.xml", TBL);
  fdXML = fopen(szFile, szMode);

  if(fdXML)
  {
    fwrite(szBuffer, 1, strlen(szBuffer),fdXML);
    fflush(fdXML);
    fclose(fdXML);
  }
}
/*--------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
  char szCurrentDir[256];
  if(argv[1])
  {
    sprintf(Database, "%s", argv[1]);
    if(argv[2])
    {
      sprintf(TBL, "%s", argv[2]);
      WriteToFile("", "w");
      CollectAllInformation(Database, argv[2], szMinimumCostValue);
    }
  }

  printf("\nPlease wait for a while, this operation may take saveral minutes.....\n");

  GetCurrentDirectory(80, szCurrentDir);

  ShellExecute(NULL, "open", "PARSER.bat", szFile, szCurrentDir, 0); 
  sprintf(szBuffer, "%s", TBL);
  Sleep(10000);
  ShellExecute(NULL, "open", "DRAWTAG.bat", szBuffer, szCurrentDir, 0); 
  Sleep(10000);
  sprintf(szBuffer, "%s.jpg", TBL);
  ShellExecute(NULL, "open", szBuffer, NULL, szCurrentDir, 1);

  return 0;
}
/*--------------------------------------------------------------------------*/
