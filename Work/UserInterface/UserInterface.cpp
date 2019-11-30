#include "stdafx.h"
#include "resource.h"
#include "DBInterface.h"
#include <shellapi.h>

#define  MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;        // current instance
HWND hWindow;
TCHAR  szTitle[MAX_LOADSTRING];   // The title bar text
TCHAR  szWindowClass[MAX_LOADSTRING];  // The title bar text

// Foward declarations of functions included in this code module:
ATOM    MyRegisterClass(HINSTANCE hInstance);
BOOL    InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
  // TODO: Place code here.
  MSG msg;
  HACCEL hAccelTable;

  // Initialize global strings
  LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadString(hInstance, IDC_USERINTERFACE, szWindowClass, MAX_LOADSTRING);

  MyRegisterClass(hInstance);

  HWND hDecisionTree = FindWindow("#32770", "Decision Tree");
  if (hDecisionTree) 
  {
    MessageBox(NULL, "One Instance is already running!", "Decision Tree", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  // Perform application initialization:
  if (!InitInstance (hInstance, nCmdShow)) 
  {
    return FALSE;
  }

  hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_USERINTERFACE);

  // Main message loop:
  while (GetMessage(&msg, NULL, 0, 0)) 
  {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
  WNDCLASSEX wcex;

  wcex.cbSize = sizeof(WNDCLASSEX); 

  wcex.style   = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = (WNDPROC)WndProc;
  wcex.cbClsExtra  = 0;
  wcex.cbWndExtra  = 0;
  wcex.hInstance  = hInstance;
  wcex.hIcon   = LoadIcon(hInstance, (LPCTSTR)IDI_USERINTERFACE);
  wcex.hCursor  = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName = (LPCSTR)IDC_USERINTERFACE;
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm  = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

  return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  HWND hWnd;

  hInst = hInstance; // Store instance handle in our global variable

  hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_BORDER | SS_CENTER,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

  /*if (!hWnd)
  {
  return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);*/

  hWindow = hWnd;
  DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
  DestroyWindow(hWnd);

  return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND - process the application menu
//  WM_PAINT - Paint the main window
//  WM_DESTROY - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  int wmId, wmEvent;

  PAINTSTRUCT ps;
  HDC hdc;
  TCHAR szHello[MAX_LOADSTRING];
  LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

  switch (message) 
  {
  case WM_COMMAND:
    wmId    = LOWORD(wParam); 
    wmEvent = HIWORD(wParam); 
    // Parse the menu selections:
    switch (wmId)
    {
    case IDM_ABOUT:
      DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
      DestroyWindow(hWnd);
      break;
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
    break;
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    // TODO: Add any drawing code here...
    RECT rt;
    GetClientRect(hWnd, &rt);
    DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
    EndPaint(hWnd, &ps);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

void ErrorExit(LPTSTR lpszFunction) 
{ 
  TCHAR szBuf[80]; 
  LPVOID lpMsgBuf;
  DWORD dw = GetLastError(); 

  FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
    FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    dw,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR) &lpMsgBuf,
    0, NULL );

  wsprintf(szBuf, 
    "%s failed with error %d: %s", 
    lpszFunction, dw, lpMsgBuf); 

  MessageBox(NULL, szBuf, "Error", MB_OK); 

  LocalFree(lpMsgBuf);
  ExitProcess(dw); 
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  int   dlgId, dlgEvent;
  char  database[80],table[80], szCurrentDir[80], szCurrentDir1[80], szBuffer[200];
  int   iCount = 0, count = 0, radioGain, radioMCost;
  char  string[80];
  HWND  hRadioGain = GetDlgItem(hDlg, IDC_RADIOGAIN);
  HWND  hRadioMCost = GetDlgItem(hDlg, IDC_RADIOMINCOST);
  HWND  hDBCombo = GetDlgItem(hDlg, IDC_COMBO1);
  HWND  hDBCombo1 = GetDlgItem(hDlg, IDC_COMBO1);

  HWND  hDBList  = GetDlgItem(hDlg, IDC_DBLIST);
  HWND  hAttributes = GetDlgItem(hDlg, IDC_ATTRIBUTES);
  HWND  hInstances = GetDlgItem(hDlg, IDC_INSTANCES);
  HWND  hClasses = GetDlgItem(hDlg, IDC_CLASSES);
  HWND  hShow  = GetDlgItem(hDlg, IDC_SHOW);
  HWND  hCancel  = GetDlgItem(hDlg, IDC_CANCEL);
  Application ui;

  switch (message)
  {
  case WM_INITDIALOG:
    SendMessage(hDBCombo, CB_LIMITTEXT, 80, 0);

    ui.fnAPP_ListDatabases("baloon");

    //for(ui.RelationItr = ui.oRelationship.begin(); ui.RelationItr != ui.oRelationship.end(); ui.RelationItr++)
    //  SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (long)ui.RelationItr->DBName.c_str());

    for(ui.RelationItr = ui.oRelationship.begin(); ui.RelationItr != ui.oRelationship.end(); ui.RelationItr++)
    {
      //SendDlgItemMessage(hDlg, IDC_COMBO1, CB_INSERTSTRING, 0, (long)ui.RelationItr->DBName.c_str());
      //SendMessageA(hDBCombo1, CB_ADDSTRING, -1, (LPARAM)ui.RelationItr->DBName.c_str());
      SendMessageA(hDBCombo, CB_ADDSTRING, -1, (LPARAM)ui.RelationItr->DBName.c_str());
    }

    InvalidateRect(hDlg, NULL, 1);
    return TRUE;

  case WM_COMMAND:
    dlgId    = LOWORD(wParam); 
    dlgEvent = HIWORD(wParam); 

    if(HIWORD(wParam) == CBN_SELCHANGE)
    {
      /*iCount = SendMessage(hDBList, LB_GETCOUNT, 0, 0);
      while(iCount>=0)
      SendMessage(hDBList, LB_DELETESTRING, (long)iCount--, 0);*/

      //GetDlgItemText(hDlg, IDC_COMBO1, database, 80);
    }

    if(HIWORD(wParam) == CBN_SELENDOK)
    {
      long index=SendMessageA(hDBCombo, CB_GETCURSEL, 0, 0);
      SendMessageA(hDBCombo, CB_GETLBTEXT, index, (LPARAM)database);
      //if (GetDlgItemText(hDlg, IDC_COMBO1, database, 100))
      //{
      //  ErrorExit("About()");
      //}

      ui.fnAPP_ListTables(database);

      iCount = SendMessage(hDBList, LB_GETCOUNT, 0, 0);
      while(iCount>=0)
        SendMessage(hDBList, LB_DELETESTRING, (long)iCount--, 0);

      for(ui.TBLItr = ui.oTables.begin(); ui.TBLItr != ui.oTables.end(); ui.TBLItr++)
      {
        SendDlgItemMessage(hDlg, IDC_DBLIST, LB_ADDSTRING, 0, (long)ui.TBLItr->szTable.c_str());
      }
      //ui.oTables.clear();

      SendMessage(hDBList, LB_SETCURSEL, 0, 0);
      SendMessage(hRadioGain, BM_SETCHECK, 1, 0);
      SendMessage(hRadioMCost, BM_SETCHECK, 0, 0);

      count = SendMessage(hDBList, LB_GETCURSEL, 0, 0);
      SendMessage(hDBList, LB_GETTEXT, count, (long)table);
      if(table[0])
      {
        count = ui.fnAPP_GetNoOfAttributes(database, table);
        if(count)
        {
          sprintf(szBuffer, "%d", count);
          SetWindowText(hAttributes, szBuffer);
        }
        count = ui.fnAPP_GetNoOfRows(database, table);
        if(count)
        {
          sprintf(szBuffer, "%d", count);
          SetWindowText(hInstances, szBuffer);
        }
        count = ui.fnAPP_GetNoOfClasses(database, table);
        if(count)
        {
          sprintf(szBuffer, "%d", count);
          SetWindowText(hClasses, szBuffer);
        }
      }
    }
    if(HIWORD(wParam) == CBN_SELENDOK)
    {
    }
    if(HIWORD(wParam) == LBN_SELCHANGE)
    {
      count = SendMessage(hDBList, LB_GETCURSEL, 0, 0);
      SendMessage(hDBList, LB_GETTEXT, count, (long)table);
      if(table[0])
      {
        GetDlgItemText(hDlg, IDC_COMBO1, database, 80);
        count = ui.fnAPP_GetNoOfAttributes(database, table);
        if(count)
        {
          sprintf(szBuffer, "%d", count);
          SetWindowText(hAttributes, szBuffer);
        }
        count = ui.fnAPP_GetNoOfRows(database, table);
        if(count)
        {
          sprintf(szBuffer, "%d", count);
          SetWindowText(hInstances, szBuffer);
        }
        count = ui.fnAPP_GetNoOfClasses(database, table);
        if(count)
        {
          sprintf(szBuffer, "%d", count);
          SetWindowText(hClasses, szBuffer);
        }
      }
    }
    if(LOWORD(wParam) == IDC_RADIOGAIN)
    {
      SendMessage(hRadioGain, BM_SETCHECK, 1, 0);
      SendMessage(hRadioMCost, BM_SETCHECK, 0, 0);
    }
    if(LOWORD(wParam) == IDC_RADIOMINCOST)
    {
      SendMessage(hRadioMCost, BM_SETCHECK, 1, 0);
      SendMessage(hRadioGain, BM_SETCHECK, 0, 0);
    }
    if(LOWORD(wParam) == IDC_OK)//dlgId)
    {
    }
    if(LOWORD(wParam) == IDC_SHOW)//dlgId)
    {
      /*--------------------------------------------------------------------------*/
      /*GetDlgItemText(hDlg, IDC_COMBO1, database, 80);

      ui.fnAPP_ListTables(database);

      iCount = SendMessage(hDBList, LB_GETCOUNT, 0, 0);
      while(iCount>=0)
      SendMessage(hDBList, LB_DELETESTRING, (long)iCount--, 0);

      for(ui.TBLItr = ui.oTables.begin(); ui.TBLItr != ui.oTables.end(); ui.TBLItr++)
      {
      SendDlgItemMessage(hDlg, IDC_DBLIST, LB_ADDSTRING, 0, (long)ui.TBLItr->szTable.c_str());
      }
      ui.oTables.clear();

      SendMessage(hDBList, LB_SETCURSEL, 0, 0);
      SendMessage(hRadioGain, BM_SETCHECK, 1, 0);


      //int index = SendMessage(hDBList, LB_GETTOPINDEX, 0, 0);
      //sprintf(table, "%d", index);
      //MessageBox(NULL, table, "Index", MB_OK);
      */
      /*--------------------------------------------------------------------------*/

      EnableWindow(hShow, FALSE);
      EnableWindow(hCancel, FALSE);
      GetDlgItemText(hDlg, IDC_COMBO1, database, 80);
      radioGain = SendMessage(hRadioGain, BM_GETCHECK, 0, 0);
      radioMCost = SendMessage(hRadioMCost, BM_GETCHECK, 0, 0);

      int index = SendMessage(hDBList, LB_GETCURSEL, 0, 0);

      SendMessage(hDBList, LB_GETTEXT, index, (long)table);
      GetCurrentDirectory(80, szCurrentDir);

      count = ui.fnAPP_GetNoOfClasses(database, table);
      if(count == 0)
        MessageBox(NULL, "Not a valid table!, Please select another table.", "No Class Values", MB_OK | MB_ICONEXCLAMATION);
      else if(count == 1)
        MessageBox(NULL, "Not a valid table!, Please select another table.", "Same Class Values", MB_OK | MB_ICONEXCLAMATION);
      else if (count > 1 && database[0] && table[0])
      {
        if(radioGain)
        {
          //sprintf(string, "Database: %s\nTable: %s", database, table);
          //MessageBox(NULL, string, szCurrentDir, MB_OK);

          sprintf(szBuffer, "%s %s", database, table);
          sprintf(szCurrentDir1, "%s\\Debug", szCurrentDir);
          ShellExecute(NULL, "open", "GainDT.exe", szBuffer, szCurrentDir, SW_SHOW); 
          EnableWindow(hShow, TRUE);
          EnableWindow(hCancel, TRUE);
        }
        else if(radioMCost)
        {
          if(count != 2)
            MessageBox(NULL, "Table must have binary classification!", "MinimumCost method", MB_OK | MB_ICONEXCLAMATION);
          else
          {
            sprintf(szBuffer, "%s %s", database, table);
            sprintf(szCurrentDir1, "%s\\Debug", szCurrentDir);
            ShellExecute(NULL, "open", "MCostDecisionTree.exe", szBuffer, szCurrentDir, SW_SHOW); 
            EnableWindow(hShow, TRUE);
            EnableWindow(hCancel, TRUE);
          }
        }
      }

    }
    if(LOWORD(wParam) == IDC_CANCEL)//dlgId)
    {
      EndDialog(hDlg, LOWORD(wParam));
    }
    return TRUE;
  }
  return FALSE;
}
