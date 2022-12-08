#include "DialogDirectory.h"
#include "DialogPECheak.h"
#include "DialogDetail.h"
#include "PETools.h"

HWND gDirDialog = NULL;

int EditRVANumArray[16] =
{
    IDC_EDIT_DIR_EXPORTRVA,
    IDC_EDIT_DIR_IMPORTRVA,
    IDC_EDIT_DIR_RESOURCERVA,
    IDC_EDIT_DIR_EXCEPTIONRVA,
    IDC_EDIT_DIR_SECURITYRVA,
    IDC_EDIT_DIR_RELOCATIONRVA,
    IDC_EDIT_DIR_DEBUGRVA,
    IDC_EDIT_DIR_COPYRIGHTRVA,
    IDC_EDIT_DIR_GLOBALPTRRVA,
    IDC_EDIT_DIR_TLSRVA,
    IDC_EDIT_DIR_LOADCONFIGRVA,
    IDC_EDIT_DIR_BOUNDIMPORTRVA,
    IDC_EDIT_DIR_IATRVA,
    IDC_EDIT_DIR_DELAYIMPORTRVA,
    IDC_EDIT_DIR_COMRVA,
    IDC_EDIT_DIR_RESERVEDRVA
};

int EditSizeNumArray[16] =
{
    IDC_EDIT_DIR_EXPORTSIZE,
    IDC_EDIT_DIR_IMPORTSIZE,
    IDC_EDIT_DIR_RESOURCESIZE,
    IDC_EDIT_DIR_EXCEPTIONSIZE,
    IDC_EDIT_DIR_SECURITYSIZE,
    IDC_EDIT_DIR_RELOCATIONSIZE,
    IDC_EDIT_DIR_DEBUGSIZE,
    IDC_EDIT_DIR_COPYRIGHTSIZE,
    IDC_EDIT_DIR_GLOBALPTRSIZE,
    IDC_EDIT_DIR_TLSSIZE,
    IDC_EDIT_DIR_LOADCONFIGSIZE,
    IDC_EDIT_DIR_BOUNDIMPORTSIZE,
    IDC_EDIT_DIR_IATSIZE,
    IDC_EDIT_DIR_DELAYIMPORTSIZE,
    IDC_EDIT_DIR_COMSIZE,
    IDC_EDIT_DIR_RESERVEDSIZE
};


BOOL CALLBACK DirectoryDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        gDirDialog = hDlg;
        InitDirDialogView();
        return TRUE;
    }
    case WM_CLOSE:
    {
        EndDialog(hDlg, 0);
        return TRUE;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_DIREXIT:
        {
            EndDialog(hDlg, 0);
            return TRUE;
        }
        case IDC_BUTTON_DIR_EXPORT:
        {
            if (!pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress && !pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size)
            {
                MessageBox(hDlg, TEXT("This application does not export table"), TEXT("ERROR"), MB_OK | MB_ICONSTOP);
                return TRUE;
            }
            CreateDialogDetail(0);
            return TRUE;
        }
        case IDC_BUTTON_DIR_IMPORT:
        {
            if (!pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress && !pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size)
            {
                MessageBox(hDlg, TEXT("This application does not import table"), TEXT("ERROR"), MB_OK | MB_ICONSTOP);
                return TRUE;
            }
            CreateDialogDetail(1);
            return TRUE;
        }
        case IDC_BUTTON_DIR_RESOURCE:
        {
            if (!pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress && !pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size)
            {
                MessageBox(hDlg, TEXT("This application does not resource table"), TEXT("ERROR"), MB_OK | MB_ICONSTOP);
                return TRUE;
            }
            CreateDialogDetail(2);
            return TRUE;
        }
        case IDC_BUTTON_DIR_RELOCAL:
        {
            if (!pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress && !pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size)
            {
                MessageBox(hDlg, TEXT("This application does not basereloc table"), TEXT("ERROR"), MB_OK | MB_ICONSTOP);
                return TRUE;
            }
            CreateDialogDetail(3);
            return TRUE;
        }
        case IDC_BUTTON_DIR_BOUNDIMPORT:
        {
            if (!pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress && !pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size)
            {
                MessageBox(hDlg, TEXT("This application does not boundimport table"), TEXT("ERROR"), MB_OK | MB_ICONSTOP);
                return TRUE;
            }
            CreateDialogDetail(4);
            return TRUE;
        }
        case IDC_BUTTON_DIR_IAT:
        {
            if (!pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress && !pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size)
            {
                MessageBox(hDlg, TEXT("This application does not IAT table"), TEXT("ERROR"), MB_OK | MB_ICONSTOP);
                return TRUE;
            }
            CreateDialogDetail(5);
            return TRUE;
        }
        default:
            break;
        }
    }
    default:
        break;
    }

    return FALSE;
}


void CreateDialogDir()
{
	DialogBox(gInstance, MAKEINTRESOURCE(IDD_DIALOG_DIRECTORY), gPECheakDialog, DirectoryDialogProc);
}


void InitDirDialogView()
{
    TCHAR buffer[1024] = { 0 };
    HWND hwndEdit = NULL;

    for (int i = 0; i < pOptionalHeader->NumberOfRvaAndSizes + 1; i++)
    {
        wsprintf(buffer, L"%p", pOptionalHeader->DataDirectory[i].VirtualAddress);
        hwndEdit = GetDlgItem(gDirDialog, EditRVANumArray[i]);
        SetWindowText(hwndEdit, buffer);
        wsprintf(buffer, L"%p", pOptionalHeader->DataDirectory[i].Size);
        hwndEdit = GetDlgItem(gDirDialog, EditSizeNumArray[i]);
        SetWindowText(hwndEdit, buffer);
    }
}