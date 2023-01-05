/****************************************************************************************************
	
				PE�鿴��:
	
				�汾:1.0

				����:�ȷ��೤1997

����ʱ��:2023-01-04 11:04

*****************************************************************************************************/
#include "DialogMain.h"
#include "DialogAbout.h"
#include "DialogPECheak.h"
#include "DialogAddShell.h"
#include "DialogDelShell.h"

HWND      ghwndListProcess;
HWND      ghwndListModule;
HWND      ghDlg;


BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        SetIcon(hDlg);
        InitListViewProcess(hDlg);
        InitListViewModules(hDlg);
        ghDlg = hDlg;
        return TRUE;
    }
    case WM_CLOSE:
    {
        int UserChoice = MessageBox(ghDlg, TEXT("Do you want to Exit?"), TEXT("Exit Warning"), MB_OKCANCEL | MB_ICONQUESTION | MB_APPLMODAL);
        if (UserChoice == 1) EndDialog(hDlg, 0);
        return TRUE;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_EXIT:
        {
            int UserChoice = MessageBox(ghDlg, TEXT("Do you want to Exit?"), TEXT("Exit Warning"), MB_OKCANCEL | MB_ICONQUESTION);
            if (UserChoice == 1) EndDialog(hDlg, 0);
            return TRUE;
        }
        case IDC_BUTTON_ABOUT:
            CreateDialogAbout();
            return TRUE;
            break;
        case IDC_BUTTON_PECHEAK:
            CreateDialogPECheak();
            return TRUE;
            break;
        case IDC_BUTTON_ADDSHELL:
            CreateDialogAddShell();
            return TRUE;
            break;
        case IDC_BUTTON_DELSHELL:
            CreateDialogDelShell();
            return TRUE;
            break;
        default:
            break;
        }
    }
    case WM_NOTIFY:
    {
        NMHDR* pNmhdr = (NMHDR*)lParam;
        if (LOWORD(wParam) == IDC_LIST_PROCESS && pNmhdr->code == NM_CLICK)
        {
            ShowProcessAllModule();
        }

        return TRUE;
    }
    default:
        break;
    }
    return FALSE;
}


BOOL GetMainModuleInfo(DWORD dwPid, MODULEINFO* moduleInfo)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    if (hProcess == NULL)
    {
        return FALSE;
    }

    HMODULE hModules[1];
    DWORD cbNeeded;
    if (EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded) == FALSE)
    {
        CloseHandle(hProcess);
        return FALSE;
    }

    MODULEINFO info;
    if (GetModuleInformation(hProcess, hModules[0], &info, sizeof(info)) == FALSE)
    {
        CloseHandle(hProcess);
        return FALSE;
    }

    *moduleInfo = info;
    CloseHandle(hProcess);
    return TRUE;
}


BOOL GetAllModules_(DWORD dwPid, HMODULE* pModuel, int nSize, DWORD* dwNumOfModules)
{

    HANDLE hProcess;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    if (hProcess == NULL)
    {
        return FALSE;
    }

    DWORD cbNeeded;
    if (EnumProcessModules(hProcess, pModuel, nSize, &cbNeeded) == FALSE)
    {
        CloseHandle(hProcess);
        return FALSE;
    }

    *dwNumOfModules = cbNeeded / sizeof(DWORD);
    return TRUE;
}


void ShowAllProcessInfo(HWND hwndList)
{
    HANDLE lpSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    int i = 0;

    if (lpSnapshot == INVALID_HANDLE_VALUE)
    {
        MessageBox(ghDlg, TEXT("��������ʧ��"), TEXT("ERROR"), MB_OK);
        return;
    }

    // �������н���
    BOOL bMore = ::Process32First(lpSnapshot, &pe32);
    while (bMore)
    {
        LV_ITEM item = { 0 };
        item.mask = LVIF_TEXT;
        item.iItem = i;
        // ����ID
        DWORD dwPid = pe32.th32ProcessID;
        // ������
        LPTSTR name = pe32.szExeFile;
        // ��ģ����Ϣ
        MODULEINFO moduleInfo = { 0 };
        BOOL miResult = FALSE;
        miResult = GetMainModuleInfo(dwPid, &moduleInfo);

        // ��һ��(������)
        item.pszText = name;
        item.iSubItem = 0;
        ListView_InsertItem(hwndList, &item);

        // �ڶ���(PID)
        TCHAR buffer[16];
        wsprintf(buffer, TEXT("%d"), dwPid);
        item.pszText = buffer;
        item.iSubItem = 1;
        ListView_SetItem(hwndList, &item);

        if (miResult)
        {
            // ������(��ģ�����ַ)
            wsprintf(buffer, TEXT("%p"), moduleInfo.lpBaseOfDll);
            item.pszText = buffer;
            item.iSubItem = 2;
            ListView_SetItem(hwndList, &item);

            // ������(�����С)
            wsprintf(buffer, TEXT("%p"), moduleInfo.SizeOfImage);
            item.pszText = buffer;
            item.iSubItem = 3;
            ListView_SetItem(hwndList, &item);
        }
        else
        {
            item.pszText = (LPWSTR)L"00000000";
            item.iSubItem = 2;
            ListView_SetItem(hwndList, &item);

            item.pszText = (LPWSTR)L"00000000";
            item.iSubItem = 3;
            ListView_SetItem(hwndList, &item);
        }

        bMore = ::Process32Next(lpSnapshot, &pe32);
        i++;
    }
}


void ShowProcessAllModule()
{
    // 1. ��ȡ�����̵�ID
    // 2. ����ģ��
    // 3. �������. ���ݻ�ȡʧ��. ����б�
    DWORD dwRowId;
    TCHAR szPid[0x20] = { 0 };

    dwRowId = SendMessage(ghwndListProcess, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
    if (dwRowId == -1)
    {
        return;
    }
    // �������������
    ListView_DeleteAllItems(ghwndListModule);



    // ��ȡPID
    LV_ITEM lv = { 0 };
    lv.iSubItem = 1;
    lv.pszText = szPid;
    lv.cchTextMax = 0x20;
    SendMessage(ghwndListProcess, LVM_GETITEMTEXT, dwRowId, (DWORD)&lv);

    DWORD dwPid = StrToInt(szPid);


    // ����ģ��
    DWORD dwNumOfModules = 0;
    BOOL bResult = FALSE;
    HMODULE hModules[1024];
    HANDLE hProcess;

    bResult = GetAllModules_(dwPid, hModules, 1024, &dwNumOfModules);
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    if (bResult == FALSE || hProcess == NULL || dwNumOfModules == 0)
    {
        return;
    }


    // �������������
    for (int row = 0; row < dwNumOfModules; row++)
    {
        // ��ȡģ������: ģ������,ģ���ַ
        TCHAR strBaseName[1024] = { 0 };
        TCHAR strFileName[1024] = { 0 };

        GetModuleBaseName(hProcess, hModules[row], strBaseName, 1024);
        GetModuleFileNameEx(hProcess, hModules[row], strFileName, 1024);

        LV_ITEM vitem = { 0 };
        vitem.mask = LVIF_TEXT;
        vitem.iItem = row;

        // ��һ��(ģ������)
        vitem.pszText = strBaseName;
        vitem.iSubItem = 0;
        ListView_InsertItem(ghwndListModule, &vitem);

        // �ڶ���(ģ��λ��)
        vitem.pszText = strFileName;
        vitem.iSubItem = 1;
        ListView_SetItem(ghwndListModule, &vitem);


        MODULEINFO mi;
        if (GetModuleInformation(hProcess, hModules[row], &mi, sizeof(MODULEINFO)))
        {

            mi.lpBaseOfDll;
            vitem.pszText = strFileName;
            vitem.iSubItem = 1;
            ListView_SetItem(ghwndListModule, &vitem);

        }
    }

    CloseHandle(hProcess);
}


void SetIcon(HWND hDlg)
{
    // ͼ��
    HICON icon1 = LoadIcon(gInstance, MAKEINTRESOURCE(IDI_ICON1));
    SendMessage(hDlg, WM_SETICON, ICON_BIG, (DWORD)icon1);
    SendMessage(hDlg, WM_SETICON, ICON_SMALL, (DWORD)icon1);
}


void InitListViewProcess(HWND hDlg)
{
    LV_COLUMN lv = { 0 };
    HWND hwndList;

    // ��ȡ���
    hwndList = GetDlgItem(hDlg, IDC_LIST_PROCESS);
    // ��������ѡ��
    SendMessage(hwndList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

    lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

    // ��һ��
    lv.pszText = (LPWSTR)L"������";
    lv.cx = 140;
    lv.iSubItem = 0;
    ListView_InsertColumn(hwndList, 0, &lv);

    // �ڶ���
    lv.pszText = (LPWSTR)L"PID";
    lv.cx = 50;
    lv.iSubItem = 1;
    ListView_InsertColumn(hwndList, 1, &lv);

    // ������
    lv.pszText = (LPWSTR)L"�����ַ";
    lv.cx = 80;
    lv.iSubItem = 2;
    ListView_InsertColumn(hwndList, 2, &lv);

    // ������
    lv.pszText = (LPWSTR)L"�����С";
    lv.cx = 80;
    lv.iSubItem = 3;
    ListView_InsertColumn(hwndList, 3, &lv);

    ghwndListProcess = hwndList;
    // �������н���
    ShowAllProcessInfo(hwndList);
}


void InitListViewModules(HWND hDlg)
{
    LV_COLUMN lv = { 0 };
    HWND hwndList;

    // ��ȡ���
    hwndList = GetDlgItem(hDlg, IDC_LIST_MODULE);
    // ��������ѡ��
    SendMessage(hwndList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

    lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

    // ��һ��
    lv.pszText = (LPWSTR)L"ģ������";
    lv.cx = 100;
    lv.iSubItem = 0;
    ListView_InsertColumn(hwndList, 0, &lv);

    // �ڶ���
    lv.pszText = (LPWSTR)L"ģ��λ��";
    lv.cx = 300;
    lv.iSubItem = 1;
    ListView_InsertColumn(hwndList, 1, &lv);

    ghwndListModule = hwndList;
}