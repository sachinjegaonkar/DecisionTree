#include <windows.h>
#include "GainDBInterface.hpp"

FILE *fdXML = NULL;
char szFile[50];
char TBL[20];
char szBuffer[256];
char szGainValue[50], root[50], Database[80];
static int iViewCount = 0;

/*==========================================================================*/
void CollectAllInformation(char szDatabase[], char szTable[], char *szGainValue);
void WriteToFile(char szBuffer[], char szMode[]="a");
/*==========================================================================*/