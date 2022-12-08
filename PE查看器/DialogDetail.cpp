#include "DialogDetail.h"
#include "DialogDirectory.h"
#include "PETools.h"

int     TableIndex = 0;
HWND    gDetialDialog = NULL;

WCHAR ResourceTypeArray[32][56] =
{
    L"Unknown",
    L"RT_CURSOR",
    L"RT_BITMAP",
    L"RT_ICON",
    L"RT_MENU",
    L"RT_DIALOG",
    L"RT_STRING",
    L"RT_FONTDIR",
    L"RT_FONT",
    L"RT_ACCELERATOR",
    L"RT_RCDATA",
    L"RT_MESSAGETABLE",
    L"RT_GROUP_CURSRO",
    L"Unknown",
    L"RT_GROUP_ICON",
    L"Unknown",
    L"RT_VERSION",
    L"RT_DLGINCLUDE",
    L"Unknown",
    L"RT_PLUGPLAY",
    L"RT_VXD",
    L"RT_ANICURSOR",
    L"RT_ANIICON"
};


BOOL CALLBACK DetailDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        gDetialDialog = hDlg;
        InitDetailDialogView();
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


void CreateDialogDetail(int Index)
{
    TableIndex = Index;
	DialogBox(gInstance, MAKEINTRESOURCE(IDD_DIALOG_DETAIL), gDirDialog, DetailDialogProc);
}


void InitDetailDialogView()
{
    switch (TableIndex)
    {
    case 0:
        ExportInfoView();
        return;
    case 1:
        ImportInfoView();
        return;
    case 2:
        ResourceInfoView();
        return;
    case 3:
        RelocalInfoView();
        return;
    case 4:
        BoundImportInfoView();
        return;
    case 5:
        IATInfoView();
        return;
    default:
        break;
    }
}


void ExportInfoView()
{
    LPWSTR pWstrInfo = (LPWSTR)malloc(1024 * 1024 * 1);
    memset(pWstrInfo, 0, 1024 * 1024 * 1);
    LPWSTR pTemWstr = pWstrInfo;

    wsprintf(pTemWstr, L"->Export Table\r\n");
    wsprintf(pTemWstr += wcslen(pTemWstr), TEXT("\t%s\t\t0x%p\r\n"), TEXT("Characteristics:"), pExportHeader->Characteristics);
    wsprintf(pTemWstr += wcslen(pTemWstr), TEXT("\t%s\t\t0x%p\r\n"), TEXT("TimeDateStamp:"), pExportHeader->TimeDateStamp);
    wsprintf(pTemWstr += wcslen(pTemWstr), TEXT("\t%s\t\t0x%p\r\n"), TEXT("MajorVersion:"), pExportHeader->MajorVersion);
    wsprintf(pTemWstr += wcslen(pTemWstr), TEXT("\t%s\t\t0x%p\r\n"), TEXT("MinorVersion:"), pExportHeader->MinorVersion);
    wsprintf(pTemWstr += wcslen(pTemWstr), TEXT("\t%s\t\t\t0x%p"), TEXT("Name:"), pExportHeader->Name);

    LPWSTR DllName = CHAR_TO_WCHAR((LPSTR)((DWORD)pDosHeader + RVA_TO_FOA(pDosHeader, pExportHeader->Name)));
    wsprintf(pTemWstr + wcslen(pTemWstr), TEXT("\t(\"%s\")\r\n"), DllName);

    wsprintf(pTemWstr += wcslen(pTemWstr), TEXT("\t%s\t\t\t0x%p\r\n"), TEXT("Base:"), pExportHeader->Base);
    wsprintf(pTemWstr += wcslen(pTemWstr), TEXT("\t%s\t0x%p\r\n"), TEXT("NumberOfFunctions:"), pExportHeader->NumberOfFunctions);
    wsprintf(pTemWstr += wcslen(pTemWstr), TEXT("\t%s\t\t0x%p\r\n"), TEXT("NumberOfNames:"), pExportHeader->NumberOfNames);
    wsprintf(pTemWstr += wcslen(pTemWstr), TEXT("\t%s\t0x%p\r\n"), TEXT("AddressOfFunctions:"), pExportHeader->AddressOfFunctions);
    wsprintf(pTemWstr += wcslen(pTemWstr), TEXT("\t%s\t\t0x%p\r\n"), TEXT("AddressOfNames:"), pExportHeader->AddressOfNames);
    wsprintf(pTemWstr += wcslen(pTemWstr), TEXT("\t%s\t0x%p\r\n"), TEXT("AddressOfNameOrdinals:"), pExportHeader->AddressOfNameOrdinals);

    wsprintf(pTemWstr + wcslen(pTemWstr), TEXT("\r\n"));
    wsprintf(pTemWstr + wcslen(pTemWstr), TEXT("\tOrdinal\tRVA\t\tSymbol Name\r\n"));
    wsprintf(pTemWstr + wcslen(pTemWstr), TEXT("\t---------\t---------------\t----------------\r\n"));

    PDWORD pAddressOfFunctions = (PDWORD)((DWORD)pDosHeader + RVA_TO_FOA(pDosHeader, pExportHeader->AddressOfFunctions));
    for (int i = 0; i < pExportHeader->NumberOfFunctions; i++)
    {
        DWORD TempFuncAddr = *pAddressOfFunctions;
        pAddressOfFunctions++;
        if (TempFuncAddr == 0) continue;
        
        DWORD Ordinal = pExportHeader->Base + i;
        WCHAR FuncName[56] = { 0 };  
        wsprintf(FuncName, TEXT("%s"), CHAR_TO_WCHAR(GetNameByOrdinal(Ordinal)));

        if (FuncName[0] == TEXT('\0'))
        {
            wsprintf(FuncName, TEXT("%s"), TEXT("Unknown"));
        }
        wsprintf(pTemWstr += wcslen(pTemWstr), TEXT("\t0x%04X\t0x%p\t\"%s\"\r\n"), Ordinal, TempFuncAddr, FuncName);
    }

    HWND hwndEdit = GetDlgItem(gDetialDialog, IDC_EDIT_DETAIL);
    SetWindowText(hwndEdit, pWstrInfo);
    free(pWstrInfo);
}


void ImportInfoView()
{
    PIMAGE_IMPORT_DESCRIPTOR pTempImportDes = pImportDescriptor;
    
    BYTE Zeros[sizeof(IMAGE_IMPORT_DESCRIPTOR)] = { 0 };
    DWORD Count = 0;

    LPWSTR pWstrInfo = (LPWSTR)malloc(1024 * 1024 * 1);
    memset(pWstrInfo, 0, 1024 * 1024 * 1);
    LPWSTR pTemWstr = pWstrInfo;

    wsprintf(pTemWstr, L"->Import Table\r\n");
    while (memcmp(pTempImportDes, Zeros, sizeof(IMAGE_IMPORT_DESCRIPTOR)))
    {
        wsprintf(pTemWstr += wcslen(pTemWstr), L"%7d.  ImageImportDescriptor:\r\n", Count + 1);
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\tOriginalFirstThunk:\t0x%p\r\n", pTempImportDes->OriginalFirstThunk);
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\tTimeDateStamp:\t0x%p   ", pTempImportDes->TimeDateStamp);

        WCHAR TimeBuffer[100] = { 0 };
        wcscpy(TimeBuffer, TimeStampToTime(pTempImportDes->TimeDateStamp));
        wsprintf(pTemWstr += wcslen(pTemWstr), L"(%s)\r\n", TimeBuffer);
        
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\tForwarderChain:\t0x%p\r\n", pTempImportDes->ForwarderChain);
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\tName:\t\t0x%p   ", pTempImportDes->Name);

        LPWSTR DllName = CHAR_TO_WCHAR((LPSTR)((DWORD)pDosHeader + RVA_TO_FOA(pDosHeader, pTempImportDes->Name)));
        wsprintf(pTemWstr + wcslen(pTemWstr), TEXT("(\"%s\")\r\n"), DllName);
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\tFirstThunk:\t0x%p\r\n", pTempImportDes->FirstThunk);

        wsprintf(pTemWstr += wcslen(pTemWstr), L"\r\n");
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\tOrdinal/Hint\tAPI Name\r\n");
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t--------------\t---------------------------------\r\n");
        

        PDWORD pOriginalFirstThunk = (PDWORD)((DWORD)pDosHeader + RVA_TO_FOA(pDosHeader, pTempImportDes->OriginalFirstThunk));
        PDWORD pTempOriginalThunk = pOriginalFirstThunk;
        while (*pTempOriginalThunk != 0)
        {
            if ((*pTempOriginalThunk) & 0x80000000)
            {
                DWORD Original = (*pTempOriginalThunk) & 0x7FFFFFFF;
                wsprintf(pTemWstr += wcslen(pTemWstr), L"\t0x%04X\r\n", Original);
            }
            else
            {
                PIMAGE_IMPORT_BY_NAME pImportByName = (PIMAGE_IMPORT_BY_NAME)((DWORD)pDosHeader + RVA_TO_FOA(pDosHeader, (*pTempOriginalThunk)));
                DWORD dHint = pImportByName->Hint;
                char* pName = pImportByName->Name;
                wsprintf(pTemWstr += wcslen(pTemWstr), L"\t0x%04X\t\t\"%s\"\r\n", dHint, CHAR_TO_WCHAR(pName));
            }
            pTempOriginalThunk++;
        }

        wsprintf(pTemWstr += wcslen(pTemWstr), L"\r\n\r\n");
        pTempImportDes++;
        Count++;
    }

    HWND hwndEdit = GetDlgItem(gDetialDialog, IDC_EDIT_DETAIL);
    SetWindowText(hwndEdit, pWstrInfo);
    free(pWstrInfo);
}


void ResourceInfoView()
{
    LPWSTR pWstrInfo = (LPWSTR)malloc(1024 * 1024 * 1);
    memset(pWstrInfo, 0, 1024 * 1024 * 1);
    LPWSTR pTemWstr = pWstrInfo;

    PDWORD pActiveAddr = (PDWORD)((DWORD)pResourceHeader + sizeof(IMAGE_RESOURCE_DIRECTORY));
    DWORD NumEntry = pResourceHeader->NumberOfNamedEntries + pResourceHeader->NumberOfIdEntries;
    PIMAGE_RESOURCE_DIRECTORY_ENTRY pResDirEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)pActiveAddr;

    // -------

    wsprintf(pTemWstr, L"->Resource Tree (detailed dump)\r\n");
    wsprintf(pTemWstr += wcslen(pTemWstr), L"\t[Resource Directory (0) ]\r\n");
    wsprintf(pTemWstr += wcslen(pTemWstr), L"\tCharacteristics:\t\t0x%p\r\n", pResourceHeader->Characteristics);
    wsprintf(pTemWstr += wcslen(pTemWstr), L"\tTimeDateStamp:\t\t0x%p", pResourceHeader->TimeDateStamp);

    WCHAR TimeBuffer[100] = { 0 };
    wcscpy(TimeBuffer, TimeStampToTime(pResourceHeader->TimeDateStamp));
    wsprintf(pTemWstr += wcslen(pTemWstr), L"\t(%s)\r\n", TimeBuffer);
      
    wsprintf(pTemWstr += wcslen(pTemWstr), L"\tMajorVersion:\t\t0x%p\r\n", pResourceHeader->MajorVersion);
    wsprintf(pTemWstr += wcslen(pTemWstr), L"\tMinorVersion:\t\t0x%p\r\n", pResourceHeader->MinorVersion);
    wsprintf(pTemWstr += wcslen(pTemWstr), L"\tNumberOfNamedEntries:\t0x%p\r\n", pResourceHeader->NumberOfNamedEntries);
    wsprintf(pTemWstr += wcslen(pTemWstr), L"\tNumberOfIdEntries:\t0x%p\r\n", pResourceHeader->NumberOfIdEntries);

    // -------

    for (int i = 0; i < NumEntry; i++)
    {
        wsprintf(pTemWstr += wcslen(pTemWstr), L"------------------------------------------------------------------------------------------------------------------------\r\n");
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t[ResourceEntry]:\r\n");
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t----------------------------------------\r\n");
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\tName/Id:\t\t0x%p", pResDirEntry->Name);
        int EntryChoice = pResDirEntry->NameIsString;
        if (EntryChoice)
        {
            PIMAGE_RESOURCE_DIR_STRING_U pString = (PIMAGE_RESOURCE_DIR_STRING_U)((DWORD)pResourceHeader + pResDirEntry->NameOffset);
            WCHAR Namestr[56] = { 0 };
            memcpy(Namestr, pString->NameString, pString->Length * 2);
            wsprintf(pTemWstr + wcslen(pTemWstr), L"\t(\"%s\")\r\n", Namestr);
        }
        else
        {
            if(pResDirEntry->Id > 23) wsprintf(pTemWstr + wcslen(pTemWstr), L"\t(\"unknown\")\r\n");
            else wsprintf(pTemWstr + wcslen(pTemWstr), L"\t(\"%s\")\r\n", ResourceTypeArray[pResDirEntry->Id]);
        }
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\tOffsetToData:\t0x%p\r\n", pResDirEntry->OffsetToData);
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t----------------------------------------\r\n");

        PIMAGE_RESOURCE_DIRECTORY pResourceHeader2 = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)pResourceHeader + pResDirEntry->OffsetToDirectory);
        pActiveAddr = (PDWORD)((DWORD)pResourceHeader2 + sizeof(IMAGE_RESOURCE_DIRECTORY));
        DWORD NumEntry2 = pResourceHeader2->NumberOfNamedEntries + pResourceHeader2->NumberOfIdEntries;
        PIMAGE_RESOURCE_DIRECTORY_ENTRY pResDirEntry2 = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)pActiveAddr;

        // -------

        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t[Resource Directory (%d) ]:\r\n", i);
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t[Characteristics:\t\t0x%p\r\n", pResourceHeader2->Characteristics);
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t[TimeDateStamp:\t\t0x%p", pResourceHeader2->TimeDateStamp);

        WCHAR TimeBuffer[100] = { 0 };
        wcscpy(TimeBuffer, TimeStampToTime(pResourceHeader2->TimeDateStamp));
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t(%s)\r\n", TimeBuffer);

        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t[MajorVersion:\t\t0x%p\r\n", pResourceHeader2->MajorVersion);
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t[MinorVersion:\t\t0x%p\r\n", pResourceHeader2->MinorVersion);
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t[NumberOfNamedEntries:\t0x%p\r\n", pResourceHeader2->NumberOfNamedEntries);
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t[NumberOfIdEntries:\t0x%p\r\n", pResourceHeader2->NumberOfIdEntries);

        // -------

        for (int j = 0; j < NumEntry2; j++)
        {
            int OffsetChoice2 = pResDirEntry2->DataIsDirectory;
            int EntryChoice2 = pResDirEntry2->NameIsString;

            wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t\t----------------------------------------\r\n");
            wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t\tName/Id:\t\t0x%p", pResDirEntry2->Name);
            if (EntryChoice2)
            {
                PIMAGE_RESOURCE_DIR_STRING_U pString = (PIMAGE_RESOURCE_DIR_STRING_U)((DWORD)pResourceHeader + pResDirEntry2->NameOffset);
                WCHAR Namestr[20] = { 0 };
                memcpy(Namestr, pString->NameString, pString->Length * 2);
                wsprintf(pTemWstr += wcslen(pTemWstr), L"\t(\"%s\")\r\n", Namestr);
            }
            else
            {
                wsprintf(pTemWstr += wcslen(pTemWstr), L"\t(\"%d\")\r\n", pResDirEntry2->Id);
            }
            wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t\tOffsetToData:\t0x%p\r\n", pResDirEntry2->OffsetToData);
            wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t\t----------------------------------------\r\n");

            if (OffsetChoice2)
            {
                PIMAGE_RESOURCE_DIRECTORY pResourceHeader3 = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)pResourceHeader + pResDirEntry2->OffsetToDirectory);
                pActiveAddr = (PDWORD)((DWORD)pResourceHeader3 + sizeof(IMAGE_RESOURCE_DIRECTORY));
                DWORD NumEntry3 = pResourceHeader3->NumberOfNamedEntries + pResourceHeader3->NumberOfIdEntries;
                PIMAGE_RESOURCE_DIRECTORY_ENTRY pResDirEntry3 = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)pActiveAddr;


                for (int k = 0; k < NumEntry3; k++)
                {
                    int OffsetChoice3 = pResDirEntry3->DataIsDirectory;
                    int EntryChoice3 = pResDirEntry3->NameIsString;

                    wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t\t\t----------------------------------------\r\n");
                    wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t\t\tName/Id:\t\t0x%p", pResDirEntry3->Name);
                    if (EntryChoice2)
                    {
                        PIMAGE_RESOURCE_DIR_STRING_U pString = (PIMAGE_RESOURCE_DIR_STRING_U)((DWORD)pResourceHeader + pResDirEntry3->NameOffset);
                        WCHAR Namestr[20] = { 0 };
                        memcpy(Namestr, pString->NameString, pString->Length * 2);
                        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t(\"%s\")\r\n", Namestr);
                    }
                    else
                    {
                        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t(\"%d\")\r\n", pResDirEntry3->Id);
                        //printf("\t\tID:%d", pResDirEntry3->Id);
                    }
                    wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t\t\tOffsetToData:\t0x%p\r\n", pResDirEntry3->OffsetToData);
                    wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t\t\t----------------------------------------\r\n");

                    PIMAGE_DATA_DIRECTORY pDirectroy = (PIMAGE_DATA_DIRECTORY)((DWORD)pResourceHeader + pResDirEntry3->OffsetToDirectory);

                    wsprintf(pTemWstr += wcslen(pTemWstr), L"\t\t\t\tRVA:0x%p", pDirectroy->VirtualAddress);
                    wsprintf(pTemWstr += wcslen(pTemWstr), L"\tSIZE:0x%p\r\n", pDirectroy->Size);

                    pResDirEntry3++;
                    pActiveAddr = (PDWORD)pResDirEntry3;
                }
            }
            pResDirEntry2++;
            pActiveAddr = (PDWORD)pResDirEntry2;
        }

        pResDirEntry++;
        pActiveAddr = (PDWORD)pResDirEntry;
    }



    HWND hwndEdit = GetDlgItem(gDetialDialog, IDC_EDIT_DETAIL);
    SetWindowText(hwndEdit, pWstrInfo);
    free(pWstrInfo);
}


void RelocalInfoView()
{
    LPWSTR pWstrInfo = (LPWSTR)malloc(1024 * 1024 * 1);
    memset(pWstrInfo, 0, 1024 * 1024 * 1);
    LPWSTR pTemWstr = pWstrInfo;

    wsprintf(pTemWstr, L"->Relocation Directory\r\n");
    PIMAGE_BASE_RELOCATION pTemRelocHead = pRelocationHeader;
    for (int Index = 1; pTemRelocHead->VirtualAddress != 0 && pTemRelocHead->VirtualAddress != 0; Index++)
    {
        int Count = (pTemRelocHead->SizeOfBlock - 0x8) / 0x2;

        wsprintf(pTemWstr += wcslen(pTemWstr), L"       %d.Relocation Block:\r\n", Index);
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\tVirtualAddress:\t%p\r\n", pRelocationHeader->VirtualAddress);
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\tSizeOfBlock:\t%p\r\n", pRelocationHeader->SizeOfBlock);

        wsprintf(pTemWstr += wcslen(pTemWstr), L"\r\n\tRVA\t\tType\r\n");
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t----------------\t---------\r\n");

        PWORD AddressStart = (PWORD)((DWORD)pTemRelocHead + 0x8);
        for (int i = 0; i < Count; i++)
        {
            WORD Charater = (*AddressStart) >> 12;
            DWORD AddrRVA = pTemRelocHead->VirtualAddress + (*(AddressStart) & 0X0FFF);
            wsprintf(pTemWstr += wcslen(pTemWstr), L"\t0x%p\t0x%04x\r\n", AddrRVA, Charater);
            AddressStart++;
        }
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\r\n");
        pTemRelocHead = (PIMAGE_BASE_RELOCATION)((DWORD)pTemRelocHead + pTemRelocHead->SizeOfBlock);
    }


    HWND hwndEdit = GetDlgItem(gDetialDialog, IDC_EDIT_DETAIL);
    SetWindowText(hwndEdit, pWstrInfo);
    free(pWstrInfo);
}


void BoundImportInfoView()
{
    LPWSTR pWstrInfo = (LPWSTR)malloc(1024 * 1024 * 1);
    memset(pWstrInfo, 0, 1024 * 1024 * 1);
    LPWSTR pTemWstr = pWstrInfo;

    PIMAGE_BOUND_IMPORT_DESCRIPTOR	pTempBoundImportDes = pBoundImportDescriptor;
    PIMAGE_BOUND_FORWARDER_REF		pTempBoundFRef = pBoundForwarderRef;
    BYTE ZerosBound[sizeof(IMAGE_BOUND_IMPORT_DESCRIPTOR)] = { 0 };
    DWORD BoundImportRVA = pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress;
    DWORD Count = 0;

    wsprintf(pTemWstr, L"->Bound Import Directory\r\n");
    while (memcmp(pTempBoundImportDes, ZerosBound, sizeof(IMAGE_BOUND_IMPORT_DESCRIPTOR)))
    {
        char* ModuleName = (char*)((DWORD)pDosHeader + RVA_TO_FOA(pDosHeader, pTempBoundImportDes->OffsetModuleName + BoundImportRVA));
        
        wsprintf(pTemWstr += wcslen(pTemWstr), L"    %d. BoundImportDescriptor:\r\n", Count + 1);      
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\tTimeDateStamp:\t\t\t0x%p", pTempBoundImportDes->TimeDateStamp);      
        
        WCHAR TimeBuffer[100] = { 0 };
        wcscpy(TimeBuffer, TimeStampToTime(pTempBoundImportDes->TimeDateStamp));
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t(\"%s\")\r\n", TimeBuffer);
        
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\tOffsetModuleName:\t\t0x%p", pTempBoundImportDes->OffsetModuleName);
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\t(\"%s\")\r\n", CHAR_TO_WCHAR(ModuleName));
        wsprintf(pTemWstr += wcslen(pTemWstr), L"\tNumberOfModuleForwarderRefs:\t0x%p\r\n", pTempBoundImportDes->NumberOfModuleForwarderRefs);
        
        for (int i = 0; i < pTempBoundImportDes->NumberOfModuleForwarderRefs; i++)
        {
            char* SecModuleName = (char*)((DWORD)pDosHeader + RVA_TO_FOA(pDosHeader, pTempBoundFRef->OffsetModuleName + BoundImportRVA));

            wsprintf(pTemWstr += wcslen(pTemWstr), L"\t    %d. ModuleForwarderReference:\r\n", i + 1);
            wsprintf(pTemWstr += wcslen(pTemWstr), L"\t            TimeDateStamp:\t\t            0x%p", pTempBoundFRef->TimeDateStamp);
            
            WCHAR TimeBuffer[100] = { 0 };
            wcscpy(TimeBuffer, TimeStampToTime(pTempBoundFRef->TimeDateStamp));
            wsprintf(pTemWstr += wcslen(pTemWstr), L"\t(\"%s\")\r\n", TimeBuffer);
            
            wsprintf(pTemWstr += wcslen(pTemWstr), L"\t            OffsetModuleName:\t\t            0x%p", pTempBoundFRef->OffsetModuleName);
            wsprintf(pTemWstr += wcslen(pTemWstr), L"\t(\"%s\")\r\n", CHAR_TO_WCHAR(SecModuleName));
            wsprintf(pTemWstr += wcslen(pTemWstr), L"\t            Reserved:\t\t\t            0x%p\r\n", pTempBoundFRef->Reserved);


            pTempBoundFRef = (PIMAGE_BOUND_FORWARDER_REF)((DWORD)pTempBoundFRef + sizeof(IMAGE_BOUND_FORWARDER_REF));
        }

        wsprintf(pTemWstr += wcslen(pTemWstr), L"\r\n");
        pTempBoundImportDes = (PIMAGE_BOUND_IMPORT_DESCRIPTOR)((DWORD)pTempBoundImportDes + sizeof(IMAGE_BOUND_FORWARDER_REF) * pTempBoundImportDes->NumberOfModuleForwarderRefs + sizeof(IMAGE_BOUND_IMPORT_DESCRIPTOR));
        pTempBoundFRef = (PIMAGE_BOUND_FORWARDER_REF)((DWORD)pTempBoundFRef + sizeof(IMAGE_BOUND_IMPORT_DESCRIPTOR));
        Count++;
    }


    HWND hwndEdit = GetDlgItem(gDetialDialog, IDC_EDIT_DETAIL);
    SetWindowText(hwndEdit, pWstrInfo);
    free(pWstrInfo);
}


void IATInfoView()
{
    LPWSTR pWstrInfo = (LPWSTR)malloc(1024 * 1024 * 1);
    memset(pWstrInfo, 0, 1024 * 1024 * 1);
    LPWSTR pTemWstr = pWstrInfo;

    wsprintf(pTemWstr, L"->IAT\r\n");
    int Count = 1;
    int loopNum = pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size / sizeof(IMAGE_THUNK_DATA);
    PIMAGE_THUNK_DATA pTempThunk = pIATDirectory;
    for (int i = 0; i < loopNum; i++)
    {
        if (pTempThunk->u1.Ordinal)
        {
            wsprintf(pTemWstr += wcslen(pTemWstr), L"[%04d]   0x%p\r\n", Count, pTempThunk->u1.Ordinal);
            Count++;
        }
        pTempThunk++;
    }

    HWND hwndEdit = GetDlgItem(gDetialDialog, IDC_EDIT_DETAIL);
    SetWindowText(hwndEdit, pWstrInfo);
    free(pWstrInfo);
}