#include "mna.h"
#include <fstream>
#include <wx/wx.h>
#include <locale>
#include <codecvt>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <thread>

#define safe_add(ch, term) unsigned char(ch + term)
#define safe_subtract(ch, sub) unsigned char(ch - sub)

std::filesystem::path FastEncryptFile(const wxString& path_to_file, const std::string& key, const bool encrypt, const bool in_place, const wxString& writeDir);
std::filesystem::path CreateFreeDir(const std::filesystem::path& parentDir, const std::string& dirName = "NoProblemCrypt_temp_");
void copyFile(std::fstream& source, std::fstream& dest);
void safeDeleteFile(const std::filesystem::path& filePath);
void ProcessAllFilesW();

frameMain* mainFrame = nullptr;
const std::streamsize buff_size = 8192;
char* const globalBuff = new char[buff_size];
wxString NPClog;
uint64_t totalBytes = 0;
uint64_t processedBytes = 0;

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
    static void ShowErrorMsg(const wxString& msg, const wxString& caption);
};

wxIMPLEMENT_APP(MyApp);

class MainFrame : public frameMain
{
public:
    MainFrame();
    ~MainFrame();
    void ShowMenu(wxCommandEvent& event) override;
    void MainButtonOnClick(wxCommandEvent& event) override;
    void OnTabPressed(wxKeyEvent& event) override;
    void OnPasswordSubmit(wxCommandEvent& event) override;
    void FromPasswordToMenu(wxCommandEvent& event) override;
    void InPlaceChange(wxCommandEvent& event) override;
    void FromConfirmToProcess(wxCommandEvent& event) override;
    void ShowAbout(wxCommandEvent& event) override;
    void KillApp();
    void FromPasswordToConfirmation();
    void CheckAllFiles();
    void ProcessAllFiles();
    void OnThreadMessage(wxCommandEvent& event);
    void FromProcessToEnd();
private:
    enum Mode {NONE, ENCRYPT, DECRYPT, FILE = 4, DIR = 8};
    enum Stage {Greeting, Menu, Password, Confirmation, Processing};
    MainFrame::Mode operatingMode;
    bool in_place;
    wxString creationDir = "";
    Stage currStage = MainFrame::Greeting;
    wxArrayString userFiles;
    wxArrayString userDirs;
    std::filesystem::path sourceDir;
    wxString password;
    wxPanel* currPanel = nullptr;
};

class ThreadMessage
{
public:
    enum MessageType { NONE, CHANGE_TOTAL_GAUGE, CHANGE_SINGLE_GAUGE, LOG_APPEND = 4, PROCESSING_ENDED = 8 , CHANGE_PROCESSING_TEXT = 16 , CHANGE_FILES_AMOUNT_TEXT = 32 };
    int type;
    long gaugeMax;
    long gaugeCurr;
    wxString msg;
    ThreadMessage(int _type, long _gaugeMax = -1, long _gaugeCurr = -1, wxString _msg = {});
};