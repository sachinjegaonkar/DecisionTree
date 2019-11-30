#include <windows.h>
#include "MinimalCostDBInterface.hpp"

FILE *fdXML = NULL;
char szFile[50];
char TBL[20];
char szBuffer[256];
char szMinimumCostValue[50], root[50], Database[80];
static int iViewCount = 0;
static bool bOnce = FALSE;
static bool bConnected = FALSE;

/*==========================================================================*/
void CollectAllInformation(char szDatabase[], char szTable[], char *szGainValue);
void WriteToFile(char szBuffer[], char szMode[]="a");
/*==========================================================================*/