#include "DialogSection.h"
#include "DialogPECHeak.h"
#include "PETools.h"


HWND gSectionDialog = NULL;
HWND hwndList = NULL;


BOOL CALLBACK SectionDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        gSectionDialog = hDlg;
        InitSectionsListView();
        ShowSectionListInfo();
        return TRUE;
    }
    case WM_CLOSE:
    {
        EndDialog(hDlg, 0);
        return TRUE;
    }
    default:
        break;
    }

    return FALSE;
}


void ShowSectionListInfo()
{
    LV_ITEM item = { 0 };
    item.mask = LVIF_TEXT;
    
    CHAR SectionName[16] = { 0 };
    WCHAR wSectionName[16] = { 0 };
    WCHAR buffer[52] = { 0 };

    PIMAGE_SECTION_HEADER pTempSectionH = pSectionHeader;
    for (int i = 0; i < pFileHeader->NumberOfSections; i++, pTempSectionH++)
    {
        wsprintfA(SectionName, "%8s", pTempSectionH->Name);
        wsprintfW(wSectionName, L"%s", CHAR_TO_WCHAR(SectionName));
        
        item.iItem = i;
        item.iSubItem = 0;
        item.pszText = wSectionName;
        ListView_InsertItem(hwndList, &item);

        item.iSubItem = 1;
        wsprintf(buffer, L"%p", pTempSectionH->VirtualAddress);
        item.pszText = buffer;
        ListView_SetItem(hwndList, &item);

        item.iSubItem = 2;
        wsprintf(buffer, L"%p", pTempSectionH->Misc.VirtualSize);
        item.pszText = buffer;
        ListView_SetItem(hwndList, &item);

        item.iSubItem = 3;
        wsprintf(buffer, L"%p", pTempSectionH->PointerToRawData);
        item.pszText = buffer;
        ListView_SetItem(hwndList, &item);

        item.iSubItem = 4;
        wsprintf(buffer, L"%p", pTempSectionH->SizeOfRawData);
        item.pszText = buffer;
        ListView_SetItem(hwndList, &item);

        item.iSubItem = 5;
        wsprintf(buffer, L"%p", pTempSectionH->Characteristics);
        item.pszText = buffer;
        ListView_SetItem(hwndList, &item);
    }
}


void InitSectionsListView()
{
    LV_COLUMN lv = { 0 };

    hwndList = GetDlgItem(gSectionDialog, IDC_LIST_SECTIONS);
    SendMessage(hwndList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

    lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

    // 第一列
    lv.pszText = (LPWSTR)L"名称";
    lv.cx = 90;
    lv.iSubItem = 0;
    ListView_InsertColumn(hwndList, 0, &lv);

    // 第二列
    lv.pszText = (LPWSTR)L"VOffset";
    lv.cx = 80;
    lv.iSubItem = 1;
    ListView_InsertColumn(hwndList, 1, &lv);

    // 第三列
    lv.pszText = (LPWSTR)L"VSize";
    lv.cx = 80;
    lv.iSubItem = 2;
    ListView_InsertColumn(hwndList, 2, &lv);

    // 第四列
    lv.pszText = (LPWSTR)L"ROffset";
    lv.cx = 80;
    lv.iSubItem = 3;
    ListView_InsertColumn(hwndList, 3, &lv);

    // 第五列
    lv.pszText = (LPWSTR)L"RSize";
    lv.cx = 80;
    lv.iSubItem = 4;
    ListView_InsertColumn(hwndList, 4, &lv);

    // 第六列
    lv.pszText = (LPWSTR)L"标志";
    lv.cx = 70;
    lv.iSubItem = 5;
    ListView_InsertColumn(hwndList, 5, &lv);
}


void CreateDialogSection()
{
    DialogBox(gInstance, MAKEINTRESOURCE(IDD_DIALOG_SECTIONS), gPECheakDialog, SectionDialogProc);
}