/****************************************************************************************************
	
				PE查看器:
	
				版本:1.0

				作者:热饭班长1997

创建时间:2023-01-04 11:04

*****************************************************************************************************/
#include "PETools.h"

CONTEXT	context;
HANDLE	ProcessHandle = NULL;

LPVOID						pShellFile = NULL;
LPVOID						pNewShellFile = NULL;
LPVOID						pOriginalFile = NULL;
LPVOID						pFileBuffer = NULL;
DWORD						ShellSize = 0;
DWORD						NewShellSize = 0;
DWORD						FileSize = 0;

PIMAGE_DOS_HEADER				pDosHeader = NULL;
PIMAGE_NT_HEADERS				pNTHeader = NULL;
PIMAGE_FILE_HEADER				pFileHeader = NULL;
PIMAGE_OPTIONAL_HEADER32		pOptionalHeader = NULL;
PIMAGE_SECTION_HEADER			pSectionHeader = NULL;
PIMAGE_EXPORT_DIRECTORY			pExportHeader = NULL;
PIMAGE_IMPORT_DESCRIPTOR		pImportDescriptor = NULL;
PIMAGE_RESOURCE_DIRECTORY	    pResourceHeader = NULL;
PIMAGE_BASE_RELOCATION			pRelocationHeader = NULL;
PIMAGE_BOUND_IMPORT_DESCRIPTOR	pBoundImportDescriptor = NULL;
PIMAGE_BOUND_FORWARDER_REF		pBoundForwarderRef = NULL;
PIMAGE_THUNK_DATA				pIATDirectory = NULL;


DWORD Align(int x, DWORD Alignment)
{
	if (x % Alignment == 0)
	{
		return x;
	}
	else
	{
		return (1 + (x / Alignment)) * Alignment;
	}
}


bool ReadPEFile(const char* FileName)
{
	FILE* pFile = NULL;
	LPVOID FileBuffer = NULL;

	pFile = fopen(FileName, "rb");
	if (!pFile)
	{
		return false;
	}

	fseek(pFile, 0, SEEK_END);
	FileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	FileBuffer = malloc(FileSize);

	if (!FileBuffer)
	{
		fclose(pFile);
		return false;
	}

	size_t ReadSize = fread(FileBuffer, FileSize, 1, pFile);
	if (!ReadSize)
	{
		free(FileBuffer);
		FileBuffer = NULL;
		fclose(pFile);
		return false;
	}

	pFileBuffer = FileBuffer;
	fclose(pFile);
	return true;
}


bool ReadPEFile(_In_ const char* FileName, _Out_ LPVOID& FileBuffer_, _Out_ DWORD& OutFileSize)
{
	FILE* pFile = NULL;
	LPVOID FileBuffer = NULL;

	pFile = fopen(FileName, "rb");
	if (!pFile)
	{
		return false;
	}

	fseek(pFile, 0, SEEK_END);
	OutFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	FileBuffer = malloc(OutFileSize);

	if (!FileBuffer)
	{
		fclose(pFile);
		return false;
	}

	size_t ReadSize = fread(FileBuffer, OutFileSize, 1, pFile);
	if (!ReadSize)
	{
		free(FileBuffer);
		FileBuffer = NULL;
		fclose(pFile);
		return false;
	}

	FileBuffer_ = FileBuffer;
	fclose(pFile);
	return true;
}


bool MemeryToFile(IN LPVOID pMemBuffer, LPCSTR FileName, IN size_t size)
{
	FILE* fpw = fopen(FileName, "wb");
	if (fpw == NULL)
	{
		return false;
	}
	if (!fwrite(pMemBuffer, 1, size, fpw))
	{
		fclose(fpw);
		return false;
	}

	fclose(fpw);
	fpw = NULL;
	return true;
}


LPSTR GetNameByOrdinal(DWORD Ordinal)
{
	if (!pExportHeader) return NULL;

	LPSTR pMultiChar = (LPSTR)malloc(256);
	memset(pMultiChar, 0, 256);
	int Index = -1;

	PWORD pAddressOfNameOrdinals = (PWORD)((DWORD)pDosHeader + RVA_TO_FOA(pDosHeader, pExportHeader->AddressOfNameOrdinals));
	for (int i = 0; i < pExportHeader->NumberOfNames; i++)
	{
		WORD TempNameIndex = *pAddressOfNameOrdinals;
		pAddressOfNameOrdinals++;
		if ((TempNameIndex + pExportHeader->Base) == Ordinal)
		{
			Index = i;
		}
	}
	if (Index == -1) return NULL;


	PDWORD pAddressOfNames = (PDWORD)((DWORD)pDosHeader + RVA_TO_FOA(pDosHeader, pExportHeader->AddressOfNames));
	for (int i = 0; i < pExportHeader->NumberOfNames; i++)
	{
		DWORD TempNameAddr = *pAddressOfNames;
		pAddressOfNames++;
		if (TempNameAddr == 0) continue;
		if (i == Index)
		{
			pMultiChar = (LPSTR)((DWORD)pDosHeader + RVA_TO_FOA(pDosHeader, TempNameAddr));
		}
	}

	return pMultiChar;
}


DWORD RVA_TO_FOA(PIMAGE_DOS_HEADER pDosHeader, DWORD dwRva)
{
	DWORD dwFov = 0;

	pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
	pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 4);
	pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);

	if (dwRva < pOptionalHeader->SizeOfHeaders && dwRva != 0)
	{
		dwFov = dwRva;
		return dwFov;
	}

	PIMAGE_SECTION_HEADER TempSectionHeader = pSectionHeader;
	for (int i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		if (dwRva >= TempSectionHeader->VirtualAddress && dwRva <= (TempSectionHeader->VirtualAddress + TempSectionHeader->SizeOfRawData))
		{
			dwFov = TempSectionHeader->PointerToRawData + dwRva - TempSectionHeader->VirtualAddress;
			return dwFov;
		}
		TempSectionHeader++;
	}

	return 0;
}


DWORD FOA_TO_RVA(PIMAGE_DOS_HEADER pDosHeader, DWORD dwFoa)
{
	DWORD dwRva = 0;

	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
	PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 4);
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);

	if (dwFoa < pOptionalHeader->SizeOfHeaders)
	{
		dwRva = dwFoa;
		return dwRva;
	}

	PIMAGE_SECTION_HEADER TempSectionHeader = pSectionHeader;
	for (int i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		if (dwFoa >= TempSectionHeader->PointerToRawData && dwFoa <= (TempSectionHeader->PointerToRawData + TempSectionHeader->SizeOfRawData))
		{
			dwRva = TempSectionHeader->VirtualAddress + dwFoa - TempSectionHeader->PointerToRawData;
			return dwRva;
		}
		TempSectionHeader++;
	}

	return 0;
}


DWORD RVA_TO_FOA(LPVOID FileBuffer, DWORD dwRva)
{
	DWORD dwFov = 0;

	PIMAGE_DOS_HEADER pDosHead = (PIMAGE_DOS_HEADER)FileBuffer;
	PIMAGE_NT_HEADERS pNTHead = (PIMAGE_NT_HEADERS)((DWORD)FileBuffer + pDosHead->e_lfanew);
	PIMAGE_FILE_HEADER pFileHead = (PIMAGE_FILE_HEADER)((DWORD)pNTHead + 4);
	PIMAGE_OPTIONAL_HEADER pOptionalHead = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHead + IMAGE_SIZEOF_FILE_HEADER);

	if (dwRva < pOptionalHead->SizeOfHeaders && dwRva != 0)
	{
		dwFov = dwRva;
		return dwFov;
	}

	PIMAGE_SECTION_HEADER TempSectionHead = (PIMAGE_SECTION_HEADER)((DWORD)pOptionalHead + pFileHead->SizeOfOptionalHeader);;
	for (int i = 0; i < pFileHead->NumberOfSections; i++)
	{
		if (dwRva >= TempSectionHead->VirtualAddress && dwRva <= (TempSectionHead->VirtualAddress + TempSectionHead->SizeOfRawData))
		{
			dwFov = TempSectionHead->PointerToRawData + dwRva - TempSectionHead->VirtualAddress;
			return dwFov;
		}
		TempSectionHead++;
	}

	return 0;
}


bool FixRelocationDirectory(DWORD NewImageBase)
{
	// 更改入口点---> 尝试自己修复重定位表
	DWORD AddItem = NewImageBase - pOptionalHeader->ImageBase;
	// pOptionalHeader->ImageBase = NewImageBase;

	PIMAGE_BASE_RELOCATION pTempRelocationDir = pRelocationHeader;
	while (pTempRelocationDir->VirtualAddress != 0 && pTempRelocationDir->SizeOfBlock != 0)
	{
		DWORD ChunkNum = (pTempRelocationDir->SizeOfBlock - 0x8) / 0x2;

		for (int i = 0; i < ChunkNum; i++)
		{
			PWORD pItem = (PWORD)(((DWORD)pTempRelocationDir + 0x8) + 0x2 * i);
			PDWORD pFixAddress = (PDWORD)((DWORD)pDosHeader + RVA_TO_FOA(pFileBuffer, pTempRelocationDir->VirtualAddress + (*pItem & 0x0FFF)));
			*pFixAddress += AddItem;
		}

		pTempRelocationDir = (PIMAGE_BASE_RELOCATION)((DWORD)pTempRelocationDir + pTempRelocationDir->SizeOfBlock);
	}

	return true;
}


bool CopyFileBufferToImageBuffer(_In_ LPVOID pFilebuffer_, _Out_ LPVOID& pImageBuffer_)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFilebuffer_;
	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pFilebuffer_ + pDosHeader->e_lfanew);

	PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 0x4);
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
	PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOptionalHeader + pFileHeader->SizeOfOptionalHeader);

	pImageBuffer_ = malloc(Align(pOptionalHeader->SizeOfImage, pOptionalHeader->SectionAlignment));
	memset(pImageBuffer_, 0, Align(pOptionalHeader->SizeOfImage, pOptionalHeader->SectionAlignment));
	memcpy(pImageBuffer_, pFilebuffer_, pOptionalHeader->SizeOfHeaders);

	PIMAGE_SECTION_HEADER TempSectionHeader = pSectionHeader;

	for (int i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		memcpy((LPVOID)((DWORD)pImageBuffer_ + TempSectionHeader->VirtualAddress), (LPVOID)((DWORD)pDosHeader + TempSectionHeader->PointerToRawData), TempSectionHeader->SizeOfRawData);
		TempSectionHeader++;
	}

	return true;
}


void ClearAllBuffer()
{
	if (pShellFile != NULL)
	{
		free(pShellFile);
		pShellFile = NULL;
	}

	if (pNewShellFile != NULL)
	{
		free(pNewShellFile);
		pNewShellFile = NULL;
	}

	if (pOriginalFile != NULL)
	{
		free(pOriginalFile);
		pOriginalFile = NULL;
	}

	if (pFileBuffer != NULL)
	{
		free(pFileBuffer);
		pFileBuffer = NULL;
	}
}


bool ExpendNewSection(char* ProcessFile)
{
	DWORD LeaveSize = 0;
	PIMAGE_SECTION_HEADER pEndSectionHeader = pSectionHeader + (pFileHeader->NumberOfSections - 1);

	// 添加节
	LeaveSize = (pOptionalHeader->SizeOfHeaders + (DWORD)pDosHeader) - (DWORD)pEndSectionHeader;
	if (LeaveSize < 0x50)
	{
		free(pShellFile);
		pShellFile = NULL;
		return false;
	}

	// 读取程序大小
	FILE* pFile = fopen(ProcessFile, "rb");
	if (!pFile)
	{
		return false;
	}
	fseek(pFile, 0, SEEK_END);
	FileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	fclose(pFile);

	PIMAGE_SECTION_HEADER pNewSectionHeader = (PIMAGE_SECTION_HEADER)malloc(0x28);
	memset(pNewSectionHeader, 0, 0x28);

	strcpy((char*)pNewSectionHeader->Name, ".NewSec");
	pNewSectionHeader->Misc.VirtualSize = FileSize;
	pNewSectionHeader->Characteristics = 0x60000020;
	pNewSectionHeader->VirtualAddress = Align(pOptionalHeader->SizeOfImage, pOptionalHeader->SectionAlignment);
	pNewSectionHeader->PointerToRawData = pEndSectionHeader->PointerToRawData + pEndSectionHeader->SizeOfRawData;
	pNewSectionHeader->NumberOfLinenumbers = 0;
	pNewSectionHeader->NumberOfRelocations = 0;
	pNewSectionHeader->PointerToLinenumbers = 0;
	pNewSectionHeader->PointerToRelocations = 0;
	pNewSectionHeader->SizeOfRawData = Align(FileSize, pOptionalHeader->FileAlignment);

	pFileHeader->NumberOfSections += 1;
	pOptionalHeader->SizeOfImage = Align(pOptionalHeader->SizeOfImage, pOptionalHeader->SectionAlignment) + pNewSectionHeader->SizeOfRawData;

	memcpy((PVOID)((DWORD)pEndSectionHeader + 0x28), pNewSectionHeader, 0x28);
	memset((PVOID)((DWORD)pEndSectionHeader + 0x50), 0, 0x28);

	NewShellSize = ShellSize + pNewSectionHeader->SizeOfRawData;
	pNewShellFile = malloc(NewShellSize);
	memset(pNewShellFile, 0, NewShellSize);
	memcpy(pNewShellFile, pShellFile, ShellSize);

	// 再次更新头
	pDosHeader = (PIMAGE_DOS_HEADER)pNewShellFile;
	pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pNewShellFile + pDosHeader->e_lfanew);
	pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 4);
	pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
	pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOptionalHeader + pFileHeader->SizeOfOptionalHeader);

	free(pNewSectionHeader);
	pNewSectionHeader = NULL;
	free(pShellFile);
	pShellFile = NULL;
	return true;
	return TRUE;
}


bool WriteFileToNewSection(char* ProcessFile, char* NewFileName)
{
	PIMAGE_SECTION_HEADER pEndSectionHeader = pSectionHeader + (pFileHeader->NumberOfSections - 1);
	DWORD WriteAddr = (DWORD)pDosHeader + pEndSectionHeader->PointerToRawData;

	ReadPEFile(ProcessFile, pOriginalFile, FileSize);
	PBYTE pTempOriginFile = (PBYTE)pOriginalFile;

	// 取反
	for (int i = 0; i < FileSize; i++)
	{
		*pTempOriginFile = ~(*pTempOriginFile);
		pTempOriginFile++;
	}

	memcpy((LPDWORD)WriteAddr, pOriginalFile, FileSize);

	if (!MemeryToFile(pNewShellFile, NewFileName, NewShellSize))
	{
		free(pNewShellFile);
		pNewShellFile = NULL;
		return false;
	}

	free(pNewShellFile);
	pNewShellFile = NULL;
	return true;
	return TRUE;
}


bool ProgramShelling(char* ShellFile, char* ProcessFile, char* NewFileName)
{
	if (!InitShellPEInfo(ShellFile))
	{
		return false;
	}

	if (!ExpendNewSection(ProcessFile))
	{
		return false;
	}

	if (!WriteFileToNewSection(ProcessFile, NewFileName))
	{
		return false;
	}

	return TRUE;
}


bool InitShellPEInfo(char* ShellFile)
{
	if (!ReadPEFile(ShellFile, pShellFile, ShellSize))
	{
		return false;
	}

	pDosHeader = (PIMAGE_DOS_HEADER)pShellFile;

	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		free(pShellFile);
		pShellFile = NULL;
		return false;
	}

	pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pShellFile + pDosHeader->e_lfanew);

	if (pNTHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		free(pShellFile);
		pShellFile = NULL;
		return false;
	}

	pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 0x4);
	pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
	pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOptionalHeader + pFileHeader->SizeOfOptionalHeader);

	return TRUE;
	return TRUE;
}


bool InitPEBuffer()
{
	pDosHeader = (PIMAGE_DOS_HEADER)pFileBuffer;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		free(pFileBuffer);
		pFileBuffer = NULL;
		return false;
	}

	pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pFileBuffer + pDosHeader->e_lfanew);
	if (pNTHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		free(pFileBuffer);
		pFileBuffer = NULL;
		return false;
	}

	pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 4);
	pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
	pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOptionalHeader + pFileHeader->SizeOfOptionalHeader);

	DWORD FOA_addr = RVA_TO_FOA(pDosHeader, pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	if(FOA_addr) pExportHeader = (PIMAGE_EXPORT_DIRECTORY)((DWORD)pDosHeader + FOA_addr);

	FOA_addr = RVA_TO_FOA(pDosHeader, pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	if (FOA_addr) pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)pDosHeader + FOA_addr);

	FOA_addr = RVA_TO_FOA(pDosHeader, pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress);
	if (FOA_addr) pResourceHeader = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)pDosHeader + FOA_addr);

	FOA_addr = RVA_TO_FOA(pDosHeader, pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
	if (FOA_addr) pRelocationHeader = (PIMAGE_BASE_RELOCATION)((DWORD)pDosHeader + FOA_addr);

	FOA_addr = RVA_TO_FOA(pDosHeader, pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress);
	if (FOA_addr)
	{
		pBoundImportDescriptor = (PIMAGE_BOUND_IMPORT_DESCRIPTOR)((DWORD)pDosHeader + FOA_addr);
		pBoundForwarderRef = (PIMAGE_BOUND_FORWARDER_REF)((DWORD)pBoundImportDescriptor + sizeof(IMAGE_BOUND_IMPORT_DESCRIPTOR));
	}

	FOA_addr = RVA_TO_FOA(pDosHeader, pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress);
	if (FOA_addr) pIATDirectory = (PIMAGE_THUNK_DATA)((DWORD)pDosHeader + FOA_addr);

	return true;
}


bool InitPEAllHeaderInfo(char* FilePath)
{
	if (!ReadPEFile(FilePath, pFileBuffer, FileSize))
	{
		return false;
	}
	if (!UpDateGlobalPEInfo(pFileBuffer)) return FALSE;

	return TRUE;
}


bool UpDateGlobalPEInfo(LPVOID pFileBuffer_)
{
	pDosHeader = (PIMAGE_DOS_HEADER)pFileBuffer_;

	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		free(pFileBuffer_);
		pFileBuffer_ = NULL;
		return false;
	}

	pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pFileBuffer_ + pDosHeader->e_lfanew);

	if (pNTHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		free(pFileBuffer_);
		pFileBuffer_ = NULL;
		return false;
	}

	pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 0x4);
	pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
	pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOptionalHeader + pFileHeader->SizeOfOptionalHeader);

	DWORD pReloacFOA = RVA_TO_FOA(pFileBuffer_, pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
	if (pReloacFOA != 0)
	{
		pRelocationHeader = (PIMAGE_BASE_RELOCATION)((DWORD)pDosHeader + pReloacFOA);
	}
	else
	{
		pRelocationHeader = NULL;
	}

	return TRUE;
}


bool DeleteShellAndSave(char* FilePath, char* SaveFilePath)
{
	if (!InitPEAllHeaderInfo(FilePath))
	{
		// MessageBox(NULL, L"初始化失败!", L"Debug", MB_OK);
		return FALSE;
	}

	PIMAGE_SECTION_HEADER pEndSectionHeader = pSectionHeader + (pFileHeader->NumberOfSections - 1);

	// 解密
	BYTE* pDecryptAddr = (BYTE*)((DWORD)pDosHeader + pEndSectionHeader->PointerToRawData);
	for (int i = 0; i < pEndSectionHeader->SizeOfRawData; i++)
	{
		*pDecryptAddr = ~(*pDecryptAddr);
		pDecryptAddr++;
	}

	DWORD OriginFileSize = pEndSectionHeader->SizeOfRawData;
	pOriginalFile = malloc(OriginFileSize);
	memset(pOriginalFile, 0, OriginFileSize);
	memcpy(pOriginalFile, (LPVOID)((DWORD)pDosHeader + pEndSectionHeader->PointerToRawData), OriginFileSize);

	// 验证
	PIMAGE_DOS_HEADER pTempDosHeader = (PIMAGE_DOS_HEADER)pOriginalFile;
	if (pTempDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		// MessageBox(NULL, L"验证失败!", L"Debug", MB_OK);
		return false;
	}

	char szNewFileName[512] = { 0 };
	strcpy(szNewFileName, SaveFilePath);
	if (!MemeryToFile(pOriginalFile, szNewFileName, OriginFileSize)) return FALSE;

	return TRUE;
}


bool DeleteShellAndRun(char* FilePath)
{
	if (!InitPEAllHeaderInfo(FilePath))
	{
		return FALSE;
	}

	PIMAGE_SECTION_HEADER pEndSectionHeader = pSectionHeader + (pFileHeader->NumberOfSections - 1);

	// 解密
	BYTE* pDecryptAddr = (BYTE*)((DWORD)pDosHeader + pEndSectionHeader->PointerToRawData);
	for (int i = 0; i < pEndSectionHeader->SizeOfRawData; i++)
	{
		*pDecryptAddr = ~(*pDecryptAddr);
		pDecryptAddr++;
	}

	// 将解密后的文件放pOriginalFile
	DWORD OriginFileSize = pEndSectionHeader->SizeOfRawData;
	pOriginalFile = malloc(OriginFileSize);
	memset(pOriginalFile, 0, OriginFileSize);
	memcpy(pOriginalFile, (LPVOID)((DWORD)pDosHeader + pEndSectionHeader->PointerToRawData), OriginFileSize);

	// 验证
	PIMAGE_DOS_HEADER pTempDosHeader = (PIMAGE_DOS_HEADER)pOriginalFile;
	if (pTempDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}

	// 以挂起的方式创建外壳进程
	TCHAR wstrCommandLine[256] = { 0 };
	CHAR strTemp[256] = { 0 };
	strcpy(strTemp, FilePath);
	wcscpy(wstrCommandLine, CHAR_TO_WCHAR(strTemp));
	
	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;
	BOOL bRet = CreateProcess(
		NULL,
		wstrCommandLine,
		NULL,
		NULL,
		FALSE,
		CREATE_SUSPENDED,
		NULL,
		NULL,
		&si, &pi);

	if (!bRet)
	{
		return FALSE;
	}

	//获取context
	context.ContextFlags = CONTEXT_FULL;
	GetThreadContext(pi.hThread, &context);
	DWORD dwbuffer = NULL;
	DWORD baseAddress = context.Ebx + 8;
	ReadProcessMemory(pi.hProcess, (LPVOID)baseAddress, &dwbuffer, 4, NULL);
	ProcessHandle = pi.hProcess;


	//卸载外壳程序
	typedef DWORD(WINAPI* pfnZwUnmapViewOfSection)(HANDLE, PVOID);
	pfnZwUnmapViewOfSection ZwUnmapViewOfSection = NULL;
	HMODULE hModule = LoadLibrary(L"ntdll.dll");
	if (hModule)
	{
		ZwUnmapViewOfSection = (pfnZwUnmapViewOfSection)GetProcAddress(hModule, "ZwUnmapViewOfSection");
		if (ZwUnmapViewOfSection)
		{
			if (ZwUnmapViewOfSection(pi.hProcess, (PVOID)dwbuffer) != 0)
			{
				free(pFileBuffer);
				pFileBuffer = NULL;
				return FALSE;
			}
		}
		else
		{
			free(pFileBuffer);
			pFileBuffer = NULL;
			return FALSE;
		}
	}
	else
	{
		free(pFileBuffer);
		pFileBuffer = NULL;
		return FALSE;
	}

	if (!UpDateGlobalPEInfo(pOriginalFile)) return FALSE;

	// ImageBase位置申请空间,拉伸解密后的文件放到申请的空间(若申请失败在随意位置申请，并修复重定向表)
	LPVOID AllocAddr = VirtualAllocEx(pi.hProcess, (LPVOID)pOptionalHeader->ImageBase, pOptionalHeader->SizeOfImage, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (AllocAddr == NULL)
	{
		AllocAddr = VirtualAllocEx(pi.hProcess, NULL, pOptionalHeader->SizeOfImage, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (pRelocationHeader != NULL) FixRelocationDirectory((DWORD)AllocAddr);
	}

	LPVOID pImageBuffer = NULL;
	CopyFileBufferToImageBuffer(pOriginalFile, pImageBuffer);
	if (!WriteProcessMemory(pi.hProcess, (LPVOID)AllocAddr, pImageBuffer, pOptionalHeader->SizeOfImage, NULL))
	{
		free(pImageBuffer);
		pImageBuffer = NULL;
		free(pOriginalFile);
		pOriginalFile = NULL;
		return FALSE;
	}
	free(pImageBuffer);
	pImageBuffer = NULL;

	//修改外壳Context的OEP(Eax), ImageBase(Ebx + 8)
	context.Eax = pOptionalHeader->AddressOfEntryPoint + (DWORD)AllocAddr;
	DWORD ImagebaseAddress = context.Ebx + 8;
	WriteProcessMemory(pi.hProcess, (LPVOID)ImagebaseAddress, &AllocAddr, 4, NULL);
	SetThreadContext(pi.hThread, &context);

	// 恢复线程
	ResumeThread(pi.hThread);
	free(pOriginalFile);
	pOriginalFile = NULL;
	return TRUE;
}