#include "DialogPECheak.h"
#include "PETools.h"
#include "DialogSection.h"
#include "DialogDirectory.h"


HWND gPECheakDialog = NULL;
TCHAR wstrFileName[256] = { 0 };


BOOL CALLBACK PECheakDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        gPECheakDialog = hDlg;
        PEBaseInfoView();
        return TRUE;
    }
    case WM_CLOSE:
    {
        EndDialog(hDlg, 0);
        return TRUE;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_PEEXIT:
        {
            EndDialog(hDlg, 0);
            return TRUE;
        }
        case IDC_BUTTON_SECTIONS:
        {
            CreateDialogSection();
            return TRUE;
        }
        case IDC_BUTTON_DIRECTORY:
        {
            CreateDialogDir();
            return TRUE;
        }
        default:
            break;
        }
        break;
    default:
        break;
    }

    return FALSE;
}


void TipStaticInfo(LPCTSTR str)
{
    HWND hwndInfo = GetDlgItem(ghDlg, IDC_STATIC_TIP);
    SetWindowText(hwndInfo, str);
}


void CreateDialogPECheak()
{
	// 弹出文件选择器
	TCHAR strFeFileExt[128] = TEXT("PE File(*.exe,*.dll,*.sys)\0*.exe;*.dll*;.sys\0") \
		TEXT("All File(*.*)\0*.*\0\0");
    OPENFILENAMEW st = { 0 };
    ZeroMemory(&st, sizeof(OPENFILENAME));
    st.lStructSize = sizeof(OPENFILENAME);
    st.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    st.hwndOwner = ghDlg;
    st.lpstrFilter = strFeFileExt;
    st.lpstrFile = wstrFileName;
    st.nMaxFile = MAX_PATH;

    if (GetOpenFileName(&st) == FALSE)
    {
        TipStaticInfo(TEXT("未选择文件!"));
        return;
    }

    LPSTR strFileName = (LPSTR)malloc(1024);
    strcpy(strFileName, WCHAR_TO_CHAR(wstrFileName));
    if (!ReadPEFile(strFileName))
    {
        TipStaticInfo(TEXT("加载PE文件失败!"));
        return;
    }
    free(strFileName);

    TCHAR buf[1024];
    wsprintf(buf, L"已打开文件 %s", wstrFileName);
    TipStaticInfo(buf);
    DialogBox(gInstance, MAKEINTRESOURCE(IDD_DIALOG_PECHEAK), ghDlg, PECheakDialogProc);
}


void PEBaseInfoView()
{
    if (!InitPEBuffer())
    {
        TipStaticInfo(TEXT("初始化PE文件失败"));
        return;
    }

    TCHAR buffer[1024];
    wsprintf(buffer, L"[ PEVIEW ] %s", wstrFileName);
    SetWindowText(gPECheakDialog, buffer);

    wsprintf(buffer, L"%p", pOptionalHeader->AddressOfEntryPoint);
    HWND hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_EOP);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pOptionalHeader->ImageBase);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_IMAGEBASE);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pOptionalHeader->SizeOfImage);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_IMAGESIZE);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pOptionalHeader->BaseOfCode);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_BASEOFCODE);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pOptionalHeader->BaseOfData);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_BASEOFDATA);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pOptionalHeader->SectionAlignment);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_SECTIONALIGNMENT);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pOptionalHeader->FileAlignment);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_FILEALIGNMENT);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pOptionalHeader->Magic);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_MAGIC);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pOptionalHeader->Subsystem);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_SUBSYSTEM);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pFileHeader->NumberOfSections);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_NUMBEROFSECTIONS);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pFileHeader->TimeDateStamp);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_TIMEDATESTAMP);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pOptionalHeader->SizeOfHeaders);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_SIZEOFHEADERS);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pFileHeader->Characteristics);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_CHARACTERISTICS);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pOptionalHeader->CheckSum);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_CHECKSUM);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pFileHeader->SizeOfOptionalHeader);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_SIZEOFOPTIONHEADER);
    SetWindowText(hwndEdit, buffer);

    wsprintf(buffer, L"%p", pOptionalHeader->NumberOfRvaAndSizes);
    hwndEdit = GetDlgItem(gPECheakDialog, IDC_EDIT_NUMOFRVASIZE);
    SetWindowText(hwndEdit, buffer);
}