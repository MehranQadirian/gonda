#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <shlobj.h>
#include <direct.h>
#include <io.h>
#include <sys/stat.h>
#include <fstream>
#include <conio.h>
#include <winioctl.h>

using namespace std;

struct TableColumnSizes
{
    int nameWidth;
    int labelWidth;
    int spaceWidth;
};

struct FileTableColumnSizes
{
    int nameWidth;
    int sizeWidth;
    int dateWidth;
};

void SetupConsole()
{
    SetConsoleCP(437);
    SetConsoleOutputCP(437);
    system("chcp 437 > nul");
}

TableColumnSizes CalculateThisPCColumns(int consoleWidth)
{
    TableColumnSizes sizes;
    int fixedWidth = 10;
    int usableWidth = max(45, consoleWidth - fixedWidth - 2);

    sizes.nameWidth = max(10, usableWidth * 20 / 100);
    sizes.labelWidth = max(15, usableWidth * 35 / 100);
    sizes.spaceWidth = usableWidth - sizes.nameWidth - sizes.labelWidth;

    return sizes;
}

FileTableColumnSizes CalculateFileColumns(int consoleWidth)
{
    FileTableColumnSizes sizes;
    int fixedWidth = 10;
    int usableWidth = max(50, consoleWidth - fixedWidth - 2);

    sizes.dateWidth = 17;
    sizes.sizeWidth = max(12, usableWidth * 25 / 100);
    sizes.nameWidth = usableWidth - sizes.sizeWidth - sizes.dateWidth;

    return sizes;
}

class FileSystemItem
{
public:
    wstring name;
    wstring fullPath;
    bool isDirectory;
    bool isRemovable;
    long long size;
    wstring extension;
    time_t lastModified;
    wstring volumeLabel;
    long long totalSize;
    long long freeSpace;

    FileSystemItem()
    {
        size = 0;
        isDirectory = false;
        isRemovable = false;
        lastModified = 0;
        totalSize = 0;
        freeSpace = 0;
    }
};

string WStringToString(const wstring &wstr)
{
    if (wstr.empty())
        return string();
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    string str(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size, NULL, NULL);
    return str;
}

wstring StringToWString(const string &str)
{
    if (str.empty())
        return wstring();
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wstring wstr(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);
    return wstr;
}

bool CompareFileSystemItems(const FileSystemItem &a, const FileSystemItem &b)
{
    if (a.isDirectory != b.isDirectory)
        return a.isDirectory;
    return a.name < b.name;
}

string FormatFileSize(long long bytes)
{
    if (bytes == 0)
        return "0 B";
    const char *sizes[] = {"B", "KB", "MB", "GB", "TB"};
    double len = (double)bytes;
    int order = 0;
    while (len >= 1024 && order < 4)
    {
        order++;
        len = len / 1024.0;
    }
    char buffer[64];
    sprintf(buffer, "%.2f %s", len, sizes[order]);
    return string(buffer);
}

string TruncateText(const string &text, int maxLength)
{
    if (text.length() <= maxLength)
        return text;
    if (maxLength <= 3)
        return "...";
    return text.substr(0, maxLength - 3) + "...";
}
string PadText(const string &text, int width)
{
    if (text.length() >= width)
        return text.substr(0, width);
    return text + string(width - text.length(), ' ');
}
string CenterText(const string &text, int width)
{
    int padding = (width - text.length()) / 2;
    return string(padding, ' ') + text + string(width - padding - text.length(), ' ');
}

void ClearScreen()
{
    system("cls");
}

void SetColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

class FileManagerTab
{
private:
    wstring currentPath;
    vector<FileSystemItem> currentItems;
    int selectedIndex;
    int scrollOffset;
    bool isInThisPCView;
    vector<wstring> navigationHistory;
    wstring clipboard;
    bool clipboardIsCut;

    bool pathInputMode;
    wstring pathInput;
    vector<wstring> pathSuggestions;
    int suggestionIndex;

public:
    FileManagerTab()
    {
        isInThisPCView = true;
        currentPath = L"This PC";
        selectedIndex = 0;
        scrollOffset = 0;
        pathInputMode = false;
        clipboardIsCut = false;
        suggestionIndex = 0;
        RefreshItems();
    }
    bool IsInThisPCView() const
    {
        return isInThisPCView;
    }
    void RefreshItems()
    {
        if (isInThisPCView)
            currentItems = GetDrives();
        else
            currentItems = GetDirectoryItems(currentPath);

        if (selectedIndex >= currentItems.size())
            selectedIndex = max(0, (int)currentItems.size() - 1);

        AdjustScrollOffset();
    }

    vector<FileSystemItem> GetDrives()
    {
        vector<FileSystemItem> items;
        DWORD drives = GetLogicalDrives();

        for (int i = 0; i < 26; i++)
        {
            if (drives & (1 << i))
            {
                wchar_t driveLetter[4];
                driveLetter[0] = L'A' + i;
                driveLetter[1] = L':';
                driveLetter[2] = L'\\';
                driveLetter[3] = L'\0';

                FileSystemItem item;
                item.name = driveLetter;
                item.fullPath = driveLetter;
                item.isDirectory = true;
                STORAGE_PROPERTY_QUERY query;
                query.PropertyId = StorageAccessAlignmentProperty;
                query.QueryType = PropertyStandardQuery;

                HANDLE hDevice = CreateFileW((item.fullPath + L"\\").c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
                if (hDevice != INVALID_HANDLE_VALUE)
                {
                    STORAGE_DEVICE_DESCRIPTOR descriptor;
                    DWORD bytesReturned;
                    if (DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(query), &descriptor, sizeof(descriptor), &bytesReturned, NULL))
                    {
                        item.isRemovable = (descriptor.BusType == BusTypeUsb);
                    }
                    CloseHandle(hDevice);
                }
                ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
                if (GetDiskFreeSpaceExW(driveLetter, &freeBytesAvailable, &totalBytes, &totalFreeBytes))
                {
                    item.totalSize = totalBytes.QuadPart;
                    item.freeSpace = freeBytesAvailable.QuadPart;
                }

                wchar_t volumeName[MAX_PATH];
                if (GetVolumeInformationW(driveLetter, volumeName, MAX_PATH, NULL, NULL, NULL, NULL, 0))
                {
                    item.volumeLabel = volumeName;
                }

                items.push_back(item);
            }
        }

        return items;
    }

    vector<FileSystemItem> GetDirectoryItems(const wstring &path)
    {
        vector<FileSystemItem> items;
        WIN32_FIND_DATAW findData;
        wstring searchPath = path + L"\\*";
        HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);

        if (hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                wstring name = findData.cFileName;
                if (name != L"." && name != L"..")
                {
                    FileSystemItem item;
                    item.name = name;
                    item.fullPath = path + L"\\" + name;
                    item.isDirectory = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

                    if (!item.isDirectory)
                    {
                        LARGE_INTEGER fileSize;
                        fileSize.LowPart = findData.nFileSizeLow;
                        fileSize.HighPart = findData.nFileSizeHigh;
                        item.size = fileSize.QuadPart;
                    }

                    FILETIME ft = findData.ftLastWriteTime;
                    SYSTEMTIME st;
                    FileTimeToSystemTime(&ft, &st);
                    struct tm timeinfo;
                    memset(&timeinfo, 0, sizeof(timeinfo));
                    timeinfo.tm_year = st.wYear - 1900;
                    timeinfo.tm_mon = st.wMonth - 1;
                    timeinfo.tm_mday = st.wDay;
                    timeinfo.tm_hour = st.wHour;
                    timeinfo.tm_min = st.wMinute;
                    timeinfo.tm_sec = st.wSecond;
                    item.lastModified = mktime(&timeinfo);

                    items.push_back(item);
                }
            } while (FindNextFileW(hFind, &findData));
            FindClose(hFind);
        }

        sort(items.begin(), items.end(), CompareFileSystemItems);

        return items;
    }

    void AdjustScrollOffset()
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        int availableLines = max(1, height - 12);

        if (selectedIndex < scrollOffset)
            scrollOffset = selectedIndex;
        else if (selectedIndex >= scrollOffset + availableLines)
            scrollOffset = selectedIndex - availableLines + 1;
    }

    void MoveUp()
    {
        if (currentItems.empty())
            return;

        if (selectedIndex > 0)
        {
            selectedIndex--;
        }
        else
        {
            selectedIndex = currentItems.size() - 1;
        }
        AdjustScrollOffset();
    }

    void MoveDown()
    {
        if (currentItems.empty())
            return;

        if (selectedIndex < currentItems.size() - 1)
        {
            selectedIndex++;
        }
        else
        {
            selectedIndex = 0;
        }
        AdjustScrollOffset();
    }

    void OpenSelected()
    {
        if (currentItems.empty())
            return;
        FileSystemItem &selected = currentItems[selectedIndex];

        if (isInThisPCView)
        {
            navigationHistory.push_back(L"This PC");
            isInThisPCView = false;
            currentPath = selected.fullPath;
            selectedIndex = 0;
            scrollOffset = 0;
            RefreshItems();
        }
        else if (selected.isDirectory)
        {
            navigationHistory.push_back(currentPath);
            currentPath = selected.fullPath;
            selectedIndex = 0;
            scrollOffset = 0;
            RefreshItems();
        }
        else
        {
            ShellExecuteW(NULL, L"open", selected.fullPath.c_str(), NULL, NULL, SW_SHOW);
        }
    }

    void GoBack()
    {
        if (!navigationHistory.empty())
        {
            wstring previousPath = navigationHistory.back();
            navigationHistory.pop_back();

            if (previousPath == L"This PC")
            {
                isInThisPCView = true;
                currentPath = L"This PC";
            }
            else
            {
                currentPath = previousPath;
            }
            selectedIndex = 0;
            scrollOffset = 0;
            RefreshItems();
        }
        else if (!isInThisPCView)
        {
            size_t pos = currentPath.find_last_of(L"\\");
            if (pos != wstring::npos && pos > 2)
            {
                currentPath = currentPath.substr(0, pos);
                selectedIndex = 0;
                scrollOffset = 0;
                RefreshItems();
            }
            else
            {
                isInThisPCView = true;
                currentPath = L"This PC";
                selectedIndex = 0;
                scrollOffset = 0;
                RefreshItems();
            }
        }
    }

    void DeleteSelected()
    {
        if (currentItems.empty() || isInThisPCView)
            return;
        FileSystemItem &selected = currentItems[selectedIndex];

        COORD coord;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        coord.X = 0;
        coord.Y = csbi.srWindow.Bottom - 2;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        SetColor(12);
        wcout << L"Delete '" << selected.name << L"'? (Y/N): ";
        SetColor(7);

        int key = getch();
        if (key == 'y' || key == 'Y')
        {
            if (selected.isDirectory)
            {
                RemoveDirectoryW(selected.fullPath.c_str());
            }
            else
            {
                DeleteFileW(selected.fullPath.c_str());
            }
            RefreshItems();
        }
    }

    void ShowHelp()
    {
        ClearScreen();
        SetColor(10);
        cout << "===============================================================\n";
        cout << CenterText("GONDA FILE MANAGER HELP", 63) << "\n";
        cout << "===============================================================\n";
        SetColor(7);

        cout << "\n[TABS]\n";
        cout << "  	1-4          	 - Switch between tabs\n";
        cout << "  	+            	 - Create new tab (up to 5)\n";

        cout << "\n[NAVIGATION]\n";
        cout << "  	Arrow Up/Down    - Move selection\n";
        cout << "  	Enter/Right      - Open folder/file\n";
        cout << "  	Left/Backspace   - Go back\n";

        cout << "\n[FILE OPERATIONS]\n";
        cout << "  	Ctrl+C       	 - Copy\n";
        cout << "  	Ctrl+X       	 - Cut\n";
        cout << "  	Ctrl+V       	 - Paste\n";
        cout << "  	Delete       	 - Delete file/folder\n";
        cout << "  	F2           	 - Rename\n";

        cout << "\n[OTHER]\n";
        cout << "  	U       	 - Help\n";
        cout << "  	F5           	 - Refresh\n";
        cout << "  	Esc          	 - Exit\n";

        cout << "\n\nPress any key to continue...";
        getch();
        ClearScreen();
    }

    void Draw(int tabIndex, int totalTabs)
    {
        ClearScreen();
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        DrawTabs(tabIndex, totalTabs, width);
        DrawPathBar(width);

        if (isInThisPCView)
        {
            DrawThisPCView(width, height);
        }
        else
        {
            DrawFileView(width, height);
        }

        DrawStatusBar(width, height);
    }

    void DrawTabs(int activeIndex, int totalTabs, int width)
    {
        SetColor(8);
        cout << (char)218;

        for (int i = 0; i < totalTabs; i++)
        {
            if (i == activeIndex)
            {
                SetColor(10);
                cout << " [" << (i + 1) << "] ";
            }
            else
            {
                SetColor(15);
                cout << " " << (i + 1) << " ";
            }
            SetColor(8);
            if (i < totalTabs - 1)
                cout << (char)179;
        }
        SetColor(8);
        for (int i = 0; i < width - (totalTabs * 5 - 1); i++)
            cout << (char)196;
        cout << (char)191 << "\n";
        SetColor(7);
    }

    void DrawPathBar(int width)
    {
        string pathToShow = WStringToString(currentPath);
        SetColor(8);
        cout << (char)179;
        SetColor(14);
        cout << "Path: " << TruncateText(pathToShow, width - 10);
        SetColor(8);
        int remaining = width - pathToShow.length() - 8;
        for (int i = 0; i < remaining + 1; i++)
            cout << " ";
        cout << (char)179 << "\n";
        cout << (char)192;
        for (int i = 0; i < width - 2; i++)
            cout << (char)196;
        cout << (char)217 << "\n";
        SetColor(7);
    }

    void DrawThisPCView(int width, int height)
    {
        SetColor(11);
        cout << " THIS PC - DRIVES\n";
        SetColor(7);

        int contentHeight = height - 8;
        int availableLines = max(1, contentHeight - 4);
        int startIndex = scrollOffset;
        int endIndex = min((int)currentItems.size(), startIndex + availableLines);

        TableColumnSizes cols = CalculateThisPCColumns(width);

        SetColor(15);
        cout << (char)218 << (char)196 << (char)196 << (char)194;
        for (int i = 0; i < cols.nameWidth + 5; i++)
            cout << (char)196;
        cout << (char)194;
        for (int i = 0; i < cols.labelWidth; i++)
            cout << (char)196;
        cout << (char)194;
        for (int i = 0; i < cols.spaceWidth; i++)
            cout << (char)196;
        cout << (char)191 << "\n";

        cout << (char)179 << "  " << (char)179 << " ";
        cout << PadText("Name", cols.nameWidth + 4);
        cout << (char)179 << " ";
        cout << PadText("Label", cols.labelWidth - 1);
        cout << (char)179 << " ";
        cout << PadText("Used / Total", cols.spaceWidth - 1);
        cout << (char)179 << "\n";

        cout << (char)195 << (char)196 << (char)196 << (char)197;
        for (int i = 0; i < cols.nameWidth + 5; i++)
            cout << (char)196;
        cout << (char)197;
        for (int i = 0; i < cols.labelWidth; i++)
            cout << (char)196;
        cout << (char)197;
        for (int i = 0; i < cols.spaceWidth; i++)
            cout << (char)196;
        cout << (char)180 << "\n";
        SetColor(7);

        for (int i = startIndex; i < endIndex; i++)
        {
            FileSystemItem &drive = currentItems[i];
            bool isSelected = (i == selectedIndex);

            string name = WStringToString(drive.name);
            string label = WStringToString(drive.volumeLabel.empty() ? L"-" : drive.volumeLabel);
            string space = FormatFileSize(drive.totalSize - drive.freeSpace) + " / " + FormatFileSize(drive.totalSize);
            SetColor(15);
            cout << (char)179;
            SetColor(10);
            if (isSelected)
                cout << " D";
            else
                cout << "D ";
            SetColor(15);
            cout << (char)179;
            if (isSelected)
            {
                SetColor(11 | (1 << 4));
            }
            else
            {
                SetColor(15);
            }
            cout << "  " << PadText(TruncateText(name, cols.nameWidth - 1), cols.nameWidth + 4);
            SetColor(15);
            cout << (char)179;
            if (isSelected)
            {
                SetColor(11 | (1 << 4));
            }
            else
            {
                SetColor(15);
            }
            cout << "  " << PadText(TruncateText(label, cols.labelWidth - 1), cols.labelWidth - 1);
            SetColor(15);
            cout << (char)179;
            if (isSelected)
            {
                SetColor(11 | (1 << 4));
            }
            else
            {
                SetColor(15);
            }
            cout << " " << PadText(TruncateText(space, cols.spaceWidth - 1), cols.spaceWidth - 1);
            SetColor(15);
            cout << (char)179 << "\n";

            SetColor(7);
        }

        SetColor(15);
        cout << (char)192 << (char)196 << (char)196 << (char)193;
        for (int i = 0; i < cols.nameWidth + 5; i++)
            cout << (char)196;
        cout << (char)193;
        for (int i = 0; i < cols.labelWidth; i++)
            cout << (char)196;
        cout << (char)193;
        for (int i = 0; i < cols.spaceWidth; i++)
            cout << (char)196;
        cout << (char)217 << "\n";
        SetColor(7);
    }

    void DrawFileView(int width, int height)
    {
        SetColor(11);
        cout << " FILES & FOLDERS\n";
        SetColor(7);

        int contentHeight = height - 8;
        int availableLines = max(1, contentHeight - 4);
        int startIndex = scrollOffset;
        int endIndex = min((int)currentItems.size(), startIndex + availableLines);

        FileTableColumnSizes cols = CalculateFileColumns(width);

        SetColor(15);
        cout << (char)218 << (char)196 << (char)196 << (char)194;
        for (int i = 0; i < cols.nameWidth + 5; i++)
            cout << (char)196;
        cout << (char)194;
        for (int i = 0; i < cols.sizeWidth; i++)
            cout << (char)196;
        cout << (char)194;
        for (int i = 0; i < cols.dateWidth; i++)
            cout << (char)196;
        cout << (char)191 << "\n";

        cout << (char)179 << "  " << (char)179 << " ";
        cout << PadText("Name", cols.nameWidth + 4);
        cout << (char)179 << " ";
        cout << PadText("Size", cols.sizeWidth - 1);
        cout << (char)179 << " ";
        cout << PadText("Modified", cols.dateWidth - 1);
        cout << (char)179 << "\n";

        cout << (char)195 << (char)196 << (char)196 << (char)197;
        for (int i = 0; i < cols.nameWidth + 5; i++)
            cout << (char)196;
        cout << (char)197;
        for (int i = 0; i < cols.sizeWidth; i++)
            cout << (char)196;
        cout << (char)197;
        for (int i = 0; i < cols.dateWidth; i++)
            cout << (char)196;
        cout << (char)180 << "\n";
        SetColor(7);

        for (int i = startIndex; i < endIndex; i++)
        {
            FileSystemItem &item = currentItems[i];
            bool isSelected = (i == selectedIndex);

            string type = item.isDirectory ? "D" : "F";
            string name = WStringToString(item.name);
            string size = item.isDirectory ? "<DIR>" : FormatFileSize(item.size);

            char dateStr[20];
            struct tm *timeinfo = localtime(&item.lastModified);
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M", timeinfo);
            SetColor(15);
            cout << (char)179;
            if (type == "D")
            {
                SetColor(7);
            }
            else if (type == "F")
            {
                SetColor(13);
            }
            if (isSelected)
                cout << " " << type;
            else
                cout << type << " ";
            SetColor(15);
            cout << (char)179;
            if (isSelected)
            {
                SetColor(11 | (1 << 4));
            }
            else
            {
                SetColor(15);
            }
            cout << "  " << PadText(TruncateText(name, cols.nameWidth - 1), cols.nameWidth + 4);
            SetColor(15);
            cout << (char)179;
            if (isSelected)
            {
                SetColor(11 | (1 << 4));
            }
            else
            {
                SetColor(15);
            }
            cout << " " << PadText(TruncateText(size, cols.sizeWidth - 1), cols.sizeWidth - 1);
            SetColor(15);
            cout << (char)179;
            if (isSelected)
            {
                SetColor(11 | (1 << 4));
            }
            else
            {
                SetColor(15);
            }
            cout << " " << PadText(dateStr, cols.dateWidth - 1);
            SetColor(15);
            cout << (char)179 << "\n";

            SetColor(7);
        }

        SetColor(15);
        cout << (char)192 << (char)196 << (char)196 << (char)193;
        for (int i = 0; i < cols.nameWidth + 5; i++)
            cout << (char)196;
        cout << (char)193;
        for (int i = 0; i < cols.sizeWidth; i++)
            cout << (char)196;
        cout << (char)193;
        for (int i = 0; i < cols.dateWidth; i++)
            cout << (char)196;
        cout << (char)217 << "\n";
        SetColor(7);
    }

    void DrawStatusBar(int width, int height)
    {
        COORD coord;
        coord.X = 0;
        coord.Y = height - 1;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        SetColor(10);
        string status = " [Up/Down:Navigate] [Enter:Open] [Back:Backspace] [Del:Delete] [+:New Tab] [F5:Refresh] [Ctrl+H:Help] [Esc:Exit] ";
        long whiteSpace = (width - status.length()) / 2;
        for (size_t i = 0; i < whiteSpace; i++)
            cout << " ";
        cout << status;
        for (size_t i = whiteSpace + status.length(); i < width; i++)
            cout << " ";
        SetColor(7);
    }

    void ProcessKey(int key)
    {
        switch (key)
        {
        case 72:
            MoveUp();
            break;
        case 80:
            MoveDown();
            break;
        case 13:
        case 77:
            OpenSelected();
            break;
        case 8:
        case 75:
            GoBack();
            break;
        case 83:
            if (!isInThisPCView)
                DeleteSelected();
            break;
        case 63:
            RefreshItems();
            break;
        case 85:
        case 117:
            ShowHelp();
            break;
        case 'r':
        case 'R':
            RefreshItems();
            break;
        }
    }
};

class TabManager
{
private:
    vector<FileManagerTab *> tabs;
    int activeTabIndex;
    int MaxTabs;
    int InitialTabs;

public:
    TabManager()
    {
        MaxTabs = 9;
        InitialTabs = 4;
        activeTabIndex = 0;
        for (int i = 0; i < InitialTabs; i++)
        {
            tabs.push_back(new FileManagerTab());
        }
    }

    ~TabManager()
    {
        for (size_t i = 0; i < tabs.size(); i++)
        {
            delete tabs[i];
        }
    }

    void Run()
    {
        DWORD lastCheckTime = GetTickCount();
        DWORD checkInterval = 2000;

        while (true)
        {
            FileManagerTab *activeTab = tabs[activeTabIndex];
            activeTab->Draw(activeTabIndex, tabs.size());

            DWORD currentTime = GetTickCount();
            if (currentTime - lastCheckTime >= checkInterval)
            {
                if (activeTab->IsInThisPCView())
                {
                    activeTab->RefreshItems();
                }
                lastCheckTime = currentTime;
            }

            int key = getch();
            if (key == 0 || key == 224)
            {
                key = getch();
            }

            if (key >= '1' && key <= '9')
            {
                int tabNum = key - '1';
                if (tabNum < tabs.size())
                {
                    activeTabIndex = tabNum;
                }
                continue;
            }

            if (key == 43)
            {
                if (tabs.size() < MaxTabs)
                {
                    tabs.push_back(new FileManagerTab());
                    activeTabIndex = tabs.size() - 1;
                    tabs[activeTabIndex]->RefreshItems();
                }
                continue;
            }

            if (key == 85)
            {
                activeTab->ShowHelp();
                continue;
            }
            else if (key == 117)
            {
                activeTab->ShowHelp();
                continue;
            }

            if (key == 27)
            {
                return;
            }

            activeTab->ProcessKey(key);
        }
    }
};

bool IsInstalled()
{
    char systemPath[MAX_PATH];
    GetSystemDirectoryA(systemPath, MAX_PATH);
    string exePath = string(systemPath) + "\\gonda.exe";

    return (_access(exePath.c_str(), 0) == 0);
}

bool InstallProgram()
{
    char currentPath[MAX_PATH];
    GetModuleFileNameA(NULL, currentPath, MAX_PATH);

    char systemPath[MAX_PATH];
    GetSystemDirectoryA(systemPath, MAX_PATH);
    string destPath = string(systemPath) + "\\gonda.exe";

    if (CopyFileA(currentPath, destPath.c_str(), FALSE))
    {
        cout << "Installation successful!\n";
        cout << "You can now run 'gonda' from anywhere in CMD.\n";
        cout << "Press any key to exit...";
        getch();
        return true;
    }
    else
    {
        cout << "Installation failed! Make sure you run as Administrator.\n";
        cout << "Press any key to exit...";
        getch();
        return false;
    }
}

void ShowInstallScreen()
{
    ClearScreen();
    SetColor(11);
    cout << "===============================================\n";
    cout << "      GONDA FILE MANAGER - INSTALLATION      \n";
    cout << "===============================================\n\n";
    SetColor(7);

    cout << "Gonda is not installed in your system.\n";
    cout << "Do you want to install it? (Y/N): ";

    int key = getch();
    cout << (char)key << "\n\n";

    if (key == 'y' || key == 'Y')
    {
        if (InstallProgram())
        {
            exit(0);
        }
    }
    else
    {
        cout << "Starting without installation...\n";
        Sleep(1500);
    }
}

int main()
{
    SetupConsole();

    if (!IsInstalled())
    {
        ShowInstallScreen();
    }

    TabManager manager;
    manager.Run();

    return 0;
}

