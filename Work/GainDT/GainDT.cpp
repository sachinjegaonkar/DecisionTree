#include <iostream>
#include <fstream>
#include <conio.h>
#include "GainDT.hpp"

/*==========================================================================*/
/*
*
*
*
*/
/*--------------------------------------------------------------------------*/

void CollectAllInformation(char szDatabase[], char szTable[], char *szGainValue)
{
  try
  {
    Application GainDT;
    int  iBufferCount=0;
    static char tble[20];
    char szTempTable[20], szLeafNode[20], szValue[20],szQueryBuffer[256];

    strcpy(tble, szTable);

    GainDT.Initialize(szTable);

    GainDT.fnAPP_GetClassesFromTable(szDatabase, szTable);
    GainDT.fnAPP_GetNumFields(szDatabase, szTable);

    GainDT.fnAPP_CalculateExpectedInfo();

    sprintf(szGainValue, "%s", GainDT.fnAPP_CalculateGain(szDatabase));

    if(szGainValue[0])
    {
    }
    else
      return;

    GainDT.fnDB_InitDatabase();
    if(GainDT.fnDB_Connect(szDatabase) == 1)
      //iterate for gain value column
      for(GainDT.ColumnListIterator = GainDT.oColumnList.begin();
        GainDT.ColumnListIterator != GainDT.oColumnList.end();
        GainDT.ColumnListIterator++)
      {
        if(stricmp(GainDT.ColumnListIterator->szColumn.c_str(), szGainValue) == 0)
        {
          sprintf(szBuffer, "<%s>\n", szGainValue);
          WriteToFile(szBuffer);
          //iterate for distinct values for resultant gain value
          sprintf(szValue, "%s", szGainValue);
          for(GainDT.ColumnListIterator->keyValueIterator = GainDT.ColumnListIterator->oKeyValuePair.begin();
            GainDT.ColumnListIterator->keyValueIterator != GainDT.ColumnListIterator->oKeyValuePair.end();
            GainDT.ColumnListIterator->keyValueIterator++)
          {
            szQueryBuffer[0]=0;
            szTempTable[0]=0;
            iBufferCount=0;
            sprintf(szTempTable, "%s%d", szValue, ++iViewCount);//tble, ++iViewCount);//
            iBufferCount += sprintf(szQueryBuffer+iBufferCount, "create view %s as select ", szTempTable);

            //column select iterator
            for(GainDT.posColumns = GainDT.szColumns.begin(); 
              GainDT.posColumns != GainDT.szColumns.end(); 
              GainDT.posColumns++)
            {
              if(stricmp(szValue, GainDT.posColumns->c_str()))
                iBufferCount += sprintf(szQueryBuffer+iBufferCount, "%s, ", GainDT.posColumns->c_str());
            }

            //query build for perticular sub table to create.tble
            iBufferCount += sprintf(szQueryBuffer+iBufferCount-2, " from %s where %s ='%s';", szTable, szValue, GainDT.ColumnListIterator->keyValueIterator->Key.c_str());
            szQueryBuffer[iBufferCount]=0;
            printf("%s\n\n", szQueryBuffer);

            if(GainDT.fnDB_ExecuteQuery(szQueryBuffer))
            {

              sprintf(szBuffer, "<%s>\n", GainDT.ColumnListIterator->keyValueIterator->Key.c_str());
              WriteToFile(szBuffer);

              //subtable created successfully.
              //call same function for this sub table.
              sprintf(szQueryBuffer, "select count(distinct class) from %s;", szTempTable);
              if((GainDT.fnDB_GetValue(szQueryBuffer)) > 1)
              {
                CollectAllInformation(szDatabase, szTempTable, szGainValue);

                sprintf(szQueryBuffer, "drop view %s;", szTempTable);
                GainDT.fnDB_ExecuteQuery(szQueryBuffer);

                sprintf(szBuffer, "</%s>\n", GainDT.ColumnListIterator->keyValueIterator->Key.c_str());
                WriteToFile(szBuffer);
              }
              else
              {
                sprintf(szQueryBuffer, "select distinct class from %s;", szTempTable);
                GainDT.fnDB_GetValue(szQueryBuffer, szLeafNode);

                sprintf(szQueryBuffer, "drop view %s;", szTempTable);
                GainDT.fnDB_ExecuteQuery(szQueryBuffer);

                sprintf(szBuffer, "<class>");
                WriteToFile(szBuffer);
                sprintf(szBuffer, "%s", szLeafNode);
                WriteToFile(szBuffer);
                sprintf(szBuffer, "</class>\n");
                WriteToFile(szBuffer);

                sprintf(szBuffer, "</%s>\n", GainDT.ColumnListIterator->keyValueIterator->Key.c_str());
                WriteToFile(szBuffer);
              }
            }
          }
          sprintf(szBuffer, "</%s>\n", GainDT.ColumnListIterator->szColumn.c_str());;
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
      CollectAllInformation(Database, argv[2], szGainValue);//"age");
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
