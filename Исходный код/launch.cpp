#include "launch.h"
#include "key_icon.xpm"

wxDEFINE_EVENT(MESSAGE_FROM_THREAD, wxCommandEvent);

bool MyApp::OnInit()
{
    mainFrame = new MainFrame();
    mainFrame->Show(true);
    return true;
}

void MyApp::ShowErrorMsg(const wxString& msg, const wxString& caption)
{
    wxMessageDialog errorDialog(mainFrame, msg, caption);
    errorDialog.ShowModal();
    reinterpret_cast<MainFrame*>(mainFrame)->KillApp();
}

MainFrame::MainFrame()
    : frameMain(nullptr, wxID_ANY),
    operatingMode(MainFrame::Mode::NONE)
{
    SetIcon(wxIcon(key_icon));
    in_place = in_place_checkbox->IsChecked();
    greetingText->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_CENTER);
    greetingText->BeginTextColour(wxColour(170, 0, 0));
    greetingText->SetCaretPosition(greetingText->GetValue().length());
    greetingText->WriteText(wxT("���� ������������� ������ ��� ������� ������������ ����, ������� � ������ ����� �����."));
    greetingText->EndTextColour();
}

void MainFrame::ShowMenu(wxCommandEvent& event)
{
    if (currStage != MainFrame::Greeting && currStage != MainFrame::Menu)
        return;

    currStage = MainFrame::Menu;

    switch (event.GetId()) {
    case EncryptFileID:
        operatingMode = MainFrame::Mode(MainFrame::ENCRYPT | MainFrame::FILE);
        break;
    case DecryptFileID:
        operatingMode = MainFrame::Mode(MainFrame::DECRYPT | MainFrame::FILE);
        break;
    case EncryptFolderID:
        operatingMode = MainFrame::Mode(MainFrame::ENCRYPT | MainFrame::DIR);
        break;
    case DecryptFolderID:
        operatingMode = MainFrame::Mode(MainFrame::DECRYPT | MainFrame::DIR);
        break;
    }

    if (operatingMode & MainFrame::FILE)
    {
        wxFileDialog selectFileDialog(this, wxString(wxT("�������� ����� ����� ")) + (operatingMode & MainFrame::ENCRYPT ? wxT("�����������") : wxT("������������")), wxT("��� ����� (*.*)|*.*"),
            wxEmptyString, wxEmptyString, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);

        if (selectFileDialog.ShowModal() == wxID_CANCEL) {
            operatingMode = MainFrame::NONE;
            return;
        }

        selectFileDialog.GetPaths(userFiles);
    }
    if (operatingMode & MainFrame::DIR)
    {
        wxDirDialog selectDirDialog(this, wxString(wxT("�������� ����� ����� ")) + (operatingMode & MainFrame::ENCRYPT ? wxT("�����������") : wxT("������������")),
            wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

        if (selectDirDialog.ShowModal() == wxID_CANCEL) {
            operatingMode = MainFrame::NONE;
            return;
        }

        const std::filesystem::path dirPath = selectDirDialog.GetPath().fn_str();
        sourceDir = dirPath;
        for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(dirPath, std::filesystem::directory_options::skip_permission_denied))
        {
            if (dir_entry.is_regular_file()) {
                userFiles.Add(wxString(dir_entry.path()));
            }
            else if (dir_entry.is_directory()) {
                userDirs.Add(wxString(dir_entry.path()));
            }
        }
    }

    if (currPanel) {
        currPanel->Hide();
    }
    greetingText->Hide();
    panelMain->Show();
    panelMain->GetContainingSizer()->Layout();

    wxString info_output = wxT("�� ������� ") + wxString::Format(wxT("%i"), userFiles.GetCount());
    info_output.append(wxString(wxT(" ����")) + (userFiles.GetCount() == 1 ? wxT("") : wxT("��")) + wxT(" ����� ") + (operatingMode & MainFrame::ENCRYPT ? wxT("�����������") : wxT("������������")) + wxT(".\n"));
    infoText->SetLabel(info_output);
    mainButton->SetLabel(operatingMode & MainFrame::ENCRYPT ? wxT("�����������") : wxT("������������"));
    infoText->GetContainingSizer()->Layout();
}

void MainFrame::MainButtonOnClick(wxCommandEvent& event)
{
    if (!in_place)
    {
        wxString storageDir = dirPicker->GetTextCtrlValue();
        if (!std::filesystem::is_directory(storageDir.fn_str()))
        {
            invalidDirInfo->ShowMessage(wxT("����� �� ����������"));
            return;
        }
        creationDir = wxString(storageDir.fn_str() / CreateFreeDir(storageDir.fn_str(),
            std::string("ALL_") + (operatingMode & MainFrame::ENCRYPT ? "EN" : "DE") + "CRYPTED_FILES"));
    }

    currStage = MainFrame::Password;
    panelMain->Hide();
    passwordPanel->Show();
    passwordPanel->GetContainingSizer()->Layout();
}

void MainFrame::OnTabPressed(wxKeyEvent& event)
{
    if (event.GetKeyCode() == int('\t'))
    {
        if (event.GetEventObject() == passwordTxtCtrl)
        {
            wxString val = passwordTxtCtrl->GetValue();
            passwordTxtCtrlVisible->SetValue(val);
        }
        else
        {
            wxString val = passwordTxtCtrlVisible->GetValue();
            passwordTxtCtrl->SetValue(val);
        }

        if (passwordTxtCtrl->Show())
        {
            passwordTxtCtrlVisible->Hide();
            passwordTxtCtrl->SetFocus();
        }
        else
        {
            passwordTxtCtrl->Hide();
            passwordTxtCtrlVisible->Show();
            passwordTxtCtrlVisible->SetFocus();
        }
        passwordTxtCtrl->GetContainingSizer()->Layout();
    }
    else
    {
        event.Skip();
    }
}

void MainFrame::OnPasswordSubmit(wxCommandEvent& event)
{
    wxTextCtrl* pwdctrl;
    wxTextCtrl* pwdctrlHidden;
    if (passwordTxtCtrl->Show(false))
    {
        passwordTxtCtrl->Show(true);
        passwordTxtCtrl->GetContainingSizer()->Layout();
        pwdctrl = passwordTxtCtrl;
        pwdctrlHidden = passwordTxtCtrlVisible;
    }
    else
    {
        pwdctrl = passwordTxtCtrlVisible;
        pwdctrlHidden = passwordTxtCtrl;
    }

    if (pwdctrlHidden->GetValue().length() == 0 && pwdctrl->GetValue().length() == 0)
        return;

    if (password.length() == 0)
    {
        if (!pwdctrl->GetValue().IsAscii())
        {
            passwordEnterText->SetLabel(wxT("������ ������ ��������� ������ ���������� ����� � �����."));
            passwordEnterText->Wrap(300);
            pwdctrl->SetValue("");
            pwdctrlHidden->SetValue("");
            if (pwdctrl->Show())
            {
                pwdctrlHidden->Hide();
                pwdctrl->SetFocus();
            }
            pwdctrl->GetContainingSizer()->Layout();
            return;
        }
        password = pwdctrl->GetValue();
        passwordEnterText->SetLabel(wxT("��������� ������."));
        pwdctrl->SetValue("");
        pwdctrlHidden->SetValue("");
        pwdctrl->GetContainingSizer()->Layout();

        if (pwdctrl->Show())
        {
            pwdctrlHidden->Hide();
            pwdctrl->SetFocus();
        }
        passwordTxtCtrl->GetContainingSizer()->Layout();
    }
    else
    {
        if (pwdctrl->GetValue() == password || pwdctrlHidden->GetValue() == password)
        {
            FromPasswordToConfirmation();
        }
        else
        {
            passwordEnterText->SetLabel(wxT("������ �� ���������, ���������� �����."));
            pwdctrl->SetValue("");
            pwdctrlHidden->SetValue("");
            pwdctrl->GetContainingSizer()->Layout();
        }
    }
}

void MainFrame::FromPasswordToMenu(wxCommandEvent& event)
{
    currStage = MainFrame::Menu;
    password.Clear();
    passwordTxtCtrl->SetValue("");
    passwordTxtCtrlVisible->SetValue("");
    passwordEnterText->SetLabel(wxT("������ �� ������ ������ ������ ������� ������ ������� � ������� �����, ���� �� ��� ��������� �� �� �������� ������������ ���� �����."));
    passwordEnterText->Wrap(300);
    passwordEnterText->GetContainingSizer()->Layout();
    passwordEnterText->Wrap(300);
    passwordPanel->Hide();
    panelMain->Show();
    panelMain->GetContainingSizer()->Layout();
}

void MainFrame::InPlaceChange(wxCommandEvent& event)
{
    this->in_place = in_place_checkbox->GetValue();
    if (!this->in_place)
    {
        dirPicker->Show();
    }
    else
    {
        dirPicker->Hide();
    }
    dirPicker->GetContainingSizer()->Layout();
}

void MainFrame::KillApp()
{
    this->Destroy();
}

void MainFrame::FromConfirmToProcess(wxCommandEvent& event)
{
    if (licenseLink->GetLabel() != wxT("�� ��������� �� ����?"))
    {
        licenseLink->SetLabel(wxT("�� ��������� �� ����?"));
        licenseLink->GetContainingSizer()->Layout();
        return;
    }

    currStage = MainFrame::Processing;

    confirmationPanel->Hide();
    processingPanel->Show();
    processingPanel->GetContainingSizer()->Layout();

    this->Bind(MESSAGE_FROM_THREAD, &MainFrame::OnThreadMessage, this);

    std::thread work_horse(ProcessAllFilesW);
    work_horse.detach();
}

void MainFrame::ShowAbout(wxCommandEvent& event)
{
    if (currStage != MainFrame::Greeting) return;
    if (currPanel) currPanel->Hide();
    else greetingText->Hide();
    switch (event.GetId()) {
    case ABOUT_DEV:
        currPanel = devPanel;
        break;
    case ABOUT_ALG:
        currPanel = algPanel;
        break;
    case ABOUT_PROGRAM:
        currPanel = aboutPanel;
        break;
    default:
        return;
    }
    currPanel->Show();
    currPanel->GetContainingSizer()->Layout();
}

void MainFrame::FromPasswordToConfirmation()
{
    currStage = MainFrame::Confirmation;
    passwordPanel->Hide();
    confirmationPanel->Show();
    confirmationPanel->GetContainingSizer()->Layout();

    if (in_place)
    {
        if (operatingMode & MainFrame::ENCRYPT)
        {
            confirmationInfo->WriteText(wxT("�� ������� ����������� ����� �� �����, ��� ������ ��� ��� ����� ���������� � ������������ ������ �� ������������ - ������������ �� ��������� ������, ������� �� ����� �����.\n"));
        }
        
        confirmationInfo->BeginTextColour(wxColour(170, 0, 0));
        confirmationInfo->WriteText(wxT("�����: �� ���������� ��������� �� ����� ���������� �����. ���� ��������, �� ���� �� ����� ������ ����� ���� ��������."));
        confirmationInfo->EndTextColour();
    }
    else
    {
        confirmationInfo->WriteText(wxString(wxT("��� ���� ")) + (operatingMode & MainFrame::ENCRYPT ? wxT("�������������") : wxT("��������������")) + wxT(" ����� ����� ������� � �����:\n"));
        confirmationInfo->WriteText(creationDir + wxT("\n"));
    }
    confirmationInfo->BeginBold();
    confirmationInfo->WriteText(wxT(" ���� �� �������� ������, ���� ����� ����� �������� ��������.\n"));
    confirmationInfo->EndBold();
}

void MainFrame::CheckAllFiles()
{
    int ios_flag = std::ios::in; // very important std::ios::app
    if (in_place)
        ios_flag = ios_flag | std::ios::out | std::ios::app;

    for (size_t i = 0; i < userFiles.GetCount(); ++i)
    {
        auto evt = new wxCommandEvent(MESSAGE_FROM_THREAD);
        std::fstream tempfs(userFiles[i].fn_str(), ios_flag);
        if (!tempfs.is_open())
        {
            MyApp::ShowErrorMsg(wxT("���� �� ���� ") + userFiles[i] +
                wxT(" �� ��� ������ ������ ��� � ��������� ��� � ���� �������, ���������� ������ � ���� ����� \"������ ������\" ��� ��������� ��� ��������� � ������ ��������������."),
                wxT("������: ���� �� ������"));
		    this->KillApp();
		    return;
        }
        tempfs.seekp(0, std::ios::end);
        if (totalBytes < _UI64_MAX) {
            totalBytes += tempfs.tellp() / 1000;
        }
        evt->SetClientData(new ThreadMessage(ThreadMessage::CHANGE_SINGLE_GAUGE, userFiles.GetCount(), i + 1));
        wxQueueEvent(this, evt);
    }
}

void MainFrame::ProcessAllFiles()
{
    for (size_t i = 0; i < userDirs.GetCount() && !in_place; ++i)
    {
        std::filesystem::path relative = std::filesystem::relative(userDirs[i].fn_str(), sourceDir);
        std::filesystem::path toCreate = std::filesystem::path(creationDir.fn_str()) / relative;
        std::filesystem::create_directory(toCreate);
    }

    for (size_t i = 0; i < userFiles.GetCount(); ++i)
    {
        {
            auto evt = new wxCommandEvent(MESSAGE_FROM_THREAD);
            evt->SetClientData(new ThreadMessage(ThreadMessage::CHANGE_PROCESSING_TEXT, -1, -1, wxString(std::filesystem::path(userFiles[i].fn_str()).filename())));
            wxQueueEvent(this, evt);
        }
        {
            auto evt = new wxCommandEvent(MESSAGE_FROM_THREAD);
            evt->SetClientData(new ThreadMessage(ThreadMessage::CHANGE_FILES_AMOUNT_TEXT, -1, -1, wxString::Format(wxT("%i"), i) + wxT(" ������ �� ") + wxString::Format(wxT("%i"), userFiles.GetCount())));
            wxQueueEvent(this, evt);
        }
        
        std::filesystem::path relative = std::filesystem::relative(userFiles[i].fn_str(), sourceDir);
        std::filesystem::path toCreate = std::filesystem::path(creationDir.fn_str()) / relative;
        FastEncryptFile(userFiles[i], password.ToStdString(), operatingMode & MainFrame::ENCRYPT, in_place, wxString(toCreate.remove_filename()));
    }
    auto evt = new wxCommandEvent(MESSAGE_FROM_THREAD);
    evt->SetClientData(new ThreadMessage(ThreadMessage::PROCESSING_ENDED));
    wxQueueEvent(this, evt);
}

ThreadMessage::ThreadMessage(int _type, long _gaugeMax, long _gaugeCurr, wxString _msg)
    :type(_type), gaugeMax(_gaugeMax), gaugeCurr(_gaugeCurr), msg(_msg)
{}

void MainFrame::OnThreadMessage(wxCommandEvent& event)
{
    auto evtData = reinterpret_cast<ThreadMessage*>(event.GetClientData());
    auto gauge = evtData->type & ThreadMessage::CHANGE_TOTAL_GAUGE ? totalProgressGauge : (evtData->type & ThreadMessage::CHANGE_SINGLE_GAUGE ? singleProgressGauge : nullptr);
    if (gauge) {
        gauge->SetRange(evtData->gaugeMax > -1 ? evtData->gaugeMax : gauge->GetRange());
        gauge->SetValue(evtData->gaugeCurr > -1 ? evtData->gaugeCurr : gauge->GetValue());
    }
    if (evtData->type & ThreadMessage::LOG_APPEND && !evtData->msg.empty())
        NPClog.append(evtData->msg);
    if (evtData->type & ThreadMessage::PROCESSING_ENDED)
        FromProcessToEnd();
    if (evtData->type & ThreadMessage::CHANGE_PROCESSING_TEXT) {
        progressState->SetLabel(evtData->msg);
        progressState->GetContainingSizer()->Layout();
    }
    if (evtData->type & ThreadMessage::CHANGE_FILES_AMOUNT_TEXT) {
        filesAmountText->SetLabel(evtData->msg);
        filesAmountText->GetContainingSizer()->Layout();
    }
}

void MainFrame::FromProcessToEnd()
{
    processingPanel->Hide();
    endPanel->Show();
    endPanel->GetContainingSizer()->Layout();
    wxString endTextVal;
    if (!in_place) {
        endTextVal = (operatingMode & MainFrame::ENCRYPT ? wxString(wxT("�������������")) : wxString(wxT("��������������"))) + wxT(" ����� ���� ������� � ����� ") + creationDir + wxT("\n");
    }
    endTextVal.append(wxT("��������� ����������, �������� ��� ����� ���� �������� ������."));
    endTextVal.append(wxT("\n���:\n") + (NPClog.empty() ? wxString(wxT("�����")) : NPClog));
    endText->SetValue(endTextVal);
}

MainFrame::~MainFrame()
{
}

std::filesystem::path FastEncryptFile(const wxString& path_to_file, const std::string& key, const bool encrypt, const bool in_place, const wxString& writeDir)
{
    std::filesystem::path filePath(path_to_file.fn_str());
    std::filesystem::path clonedFilePath;
    std::fstream file, clonedFile;
    if (in_place) {
        file.open(filePath, std::ios::out | std::ios::in | std::ios::binary);
    }
    else {
        clonedFilePath = writeDir.fn_str() / filePath.filename();
        file.open(filePath, std::ios::in | std::ios::binary);
        clonedFile.open(clonedFilePath, std::ios::out | std::ios::binary);
    }

    if (!file.is_open())
    {
        auto evt = new wxCommandEvent(MESSAGE_FROM_THREAD);
        evt->SetClientData(new ThreadMessage(ThreadMessage::LOG_APPEND, -1, -1, wxT("������: ���� �� ���� ") + path_to_file +
            wxT(" �� ��� ������ ������ ��� � ��������� ��� � ���� �������, ������ ��� �������� ����������, ���������� ������ � ���� ����� \"������ ������\" ��� ��������� ��� ��������� � ������ ��������������.\n")));
        wxQueueEvent(mainFrame, evt);
        return std::string();
    }
    if (!clonedFile.is_open() && !in_place)
    {
        auto evt = new wxCommandEvent(MESSAGE_FROM_THREAD);
        evt->SetClientData(new ThreadMessage(ThreadMessage::LOG_APPEND, -1, -1, wxT("������: �� ������� ������� ���� �� ���� ") + writeDir +
            wxT(" ���������� ��������� ��� ��������� � ������ �������������� ��� ���������� ����� �� �����.\n")));
        wxQueueEvent(mainFrame, evt);
        return std::string();
    }

	const size_t key_length = key.length();

	std::streamsize bytesRead = buff_size;
    constexpr int delim = buff_size * 122;
    file.seekp(0, std::ios::end);
    const auto fileSize = file.tellp();
    file.seekp(0, std::ios::beg);
	while (bytesRead == buff_size)
	{
        auto lastPos = file.tellp();
		file.read(globalBuff, buff_size);
		bytesRead = file.gcount();
        if (!file.good()) {
            file.clear();
            file.seekp(lastPos);
        }
		else if (in_place) file.seekp(-bytesRead, std::ios::cur);
		for (std::streamsize i = 0; i < bytesRead; ++i)
		{
			globalBuff[i] = encrypt ? safe_add(globalBuff[i], key[i % key_length]) : safe_subtract(globalBuff[i], key[i % key_length]);
		}
		(in_place ? file : clonedFile).write(globalBuff, bytesRead);
        if (file.tellp() % delim == 0)
        {
            processedBytes += delim / 1000;
            {
                auto evt = new wxCommandEvent(MESSAGE_FROM_THREAD);
                auto ratio = double(file.tellp()) / double(fileSize);
                ratio *= 1000.0;
                evt->SetClientData(new ThreadMessage(ThreadMessage::CHANGE_SINGLE_GAUGE, 1000, (int)ratio));
                wxQueueEvent(mainFrame, evt);
            }
            auto ratio = double(processedBytes) / double(totalBytes);
            ratio *= 1000.0;
            auto evt = new wxCommandEvent(MESSAGE_FROM_THREAD);
            evt->SetClientData(new ThreadMessage(ThreadMessage::CHANGE_TOTAL_GAUGE, 1000, (int)ratio));
            wxQueueEvent(mainFrame, evt);
        }
	}

    if (!in_place) clonedFile.close();
    file.close();

    if (!in_place && std::filesystem::file_size(filePath) != std::filesystem::file_size(clonedFilePath))
    {
        auto evt = new wxCommandEvent(MESSAGE_FROM_THREAD);
        evt->SetClientData(new ThreadMessage(ThreadMessage::LOG_APPEND, -1, -1, wxString(wxT("�� ������� ��������� ")) + (encrypt ? wxT("����������") : wxT("������������")) + wxT(" ���� �� ���� ") + path_to_file +
            wxT(" ������ ���� ���� ��� �������� ���������, ���������� ��������� ��� ��������� � ������ �������������� ��� ���������� ����� �� �����.\n")));
        wxQueueEvent(mainFrame, evt);
        
        safeDeleteFile(clonedFilePath);
        return std::string();
    }
    return in_place ? filePath : clonedFilePath;
}

void safeDeleteFile(const std::filesystem::path& filePath)
{
    std::fstream file(filePath, std::ios::in | std::ios::binary);
    const auto fileSize = std::filesystem::file_size(filePath);
    file.close();
    file.open(filePath, std::ios::out | std::ios::binary);
    char* tempBuff = new char[buff_size];
    memset(tempBuff, 0, buff_size);
    
    for (int i = 0; i < fileSize; i += buff_size)
    {
        file.write(tempBuff, buff_size);
    }
    delete[] tempBuff;
    file.close();
    std::filesystem::remove(filePath);
}

void ProcessAllFilesW()
{
    reinterpret_cast<MainFrame*>(mainFrame)->CheckAllFiles();
    reinterpret_cast<MainFrame*>(mainFrame)->ProcessAllFiles();
}

void copyFile(std::fstream& source, std::fstream& dest)
{
    std::streamsize bytes_read = 0;
    while (source.good())
    {
        source.read(globalBuff, buff_size);
        bytes_read = source.gcount();
        dest.write(globalBuff, bytes_read);
    }
    dest.flush();
}

std::filesystem::path CreateFreeDir(const std::filesystem::path& parentDir, const std::string& dirName)
{
    std::filesystem::path path_out = parentDir / dirName;
    for (int i = 1; std::filesystem::is_directory(path_out); ++i) {
        path_out = parentDir / (dirName + std::to_string(i));
    }

    if (!std::filesystem::create_directory(path_out))
    {
        MyApp::ShowErrorMsg(wxT("�� ������� ������� ����� ��� ") + wxString(parentDir) + " ���������� ��������� ��� ��������� � ������ ��������������.",
            wxT("������: �� ������� ������� �����"));
    }
    return path_out;
}
