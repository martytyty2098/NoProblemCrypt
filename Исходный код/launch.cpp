#include "launch.h"

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
    : frameMain(nullptr, wxID_ANY, wxT("NoProblemCrypt")),
    operatingMode(MainFrame::Mode::NONE)
{
    in_place = in_place_checkbox->IsChecked();
    greetingText->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_CENTER);
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
        wxFileDialog selectFileDialog(this, wxString(wxT("Âûáåðèòå ôàéëû êîòîðûå õîòèòå ")) + (operatingMode & MainFrame::ENCRYPT ? wxT("çàøèôðîâàòü") : wxT("ðàñøèôðîâàòü")), wxT("Ëþáîé ôàéë (*.*)|*.*"),
            wxEmptyString, wxEmptyString, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);

        if (selectFileDialog.ShowModal() == wxID_CANCEL) {
            operatingMode = MainFrame::NONE;
            return;
        }

        selectFileDialog.GetPaths(userFiles);
    }
    if (operatingMode & MainFrame::DIR)
    {
        wxDirDialog selectDirDialog(this, wxString(wxT("Âûáåðèòå ïàïêó êîòîðóþ õîòèòå ")) + (operatingMode & MainFrame::ENCRYPT ? wxT("çàøèôðîâàòü") : wxT("ðàñøèôðîâàòü")),
            wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

        if (selectDirDialog.ShowModal() == wxID_CANCEL) {
            operatingMode = MainFrame::NONE;
            return;
        }

        const std::filesystem::path dirPath = selectDirDialog.GetPath().fn_str();
        for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(dirPath, std::filesystem::directory_options::skip_permission_denied))
        {
            if (dir_entry.is_regular_file()) {
                userFiles.Add(wxString(dir_entry.path()));
            }
        }
    }

    greetingText->Hide();
    panelMain->Show();
    panelMain->GetContainingSizer()->Layout();

    wxString info_output = wxT("Âû âûáðàëè ") + wxString::Format(wxT("%i"), userFiles.GetCount());
    info_output.append(wxString(wxT(" ôàéë")) + (userFiles.GetCount() == 1 ? wxT("") : wxT("îâ")) + wxT(" äëÿ òîãî ÷òîáû ") + (operatingMode & MainFrame::ENCRYPT ? wxT("çàøèôðîâàòü") : wxT("ðàñøèôðîâàòü")) + wxT(".\n"));
    infoText->SetLabel(info_output);
    mainButton->SetLabel(operatingMode & MainFrame::ENCRYPT ? wxT("Çàøèôðîâàòü") : wxT("Ðàñøèôðîâàòü"));
    infoText->GetContainingSizer()->Layout();
}

void MainFrame::MainButtonOnClick(wxCommandEvent& event)
{
    if (!in_place)
    {
        wxString storageDir = dirPicker->GetTextCtrlValue();
        if (!std::filesystem::is_directory(storageDir.fn_str()))
        {
            invalidDirInfo->ShowMessage(wxT("Ïàïêà íå ñóùåñòâóåò"));
            return;
        }
        creationDir = wxString(storageDir.fn_str() / CreateFreeDir(storageDir.fn_str(), "ALL_FILES"));
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
            passwordEnterText->SetLabel(wxT("Ýòîò ïàðîëü ñîäåðæèò íåäîïóñòèìûå ñèìâîëû,\nðàçðåøåíû òîëüêî àíãëèéñêèå áóêâû è öèôðû."));
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
        passwordEnterText->SetLabel(wxT("Ïîâòîðèòå ïàðîëü."));
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
            passwordEnterText->SetLabel(wxT("Ïàðîëü íå ñîâïàäàåò, ïîïðîáóéòå çàíîâî."));
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
    passwordPanel->Hide();
    panelMain->Show();
    passwordEnterText->SetLabel(wxT("Òåïåðü âû äîëæíû ââåñòè ïàðîëü êîòîðûé íóæíî õðàíèòü â íàä¸æíîì ìåñòå, åñëè âû óòðàòèòå ñâîé ïàðîëü òî íå ñìîæåòå ðàñøèôðîâàòü ôàéëû îáðàòíî."));
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
    if (licenseLink->GetLabel() != wxT("Âû ïðî÷èòàëè âñ¸ âûøå?"))
    {
        licenseLink->SetLabel(wxT("Âû ïðî÷èòàëè âñ¸ âûøå?"));
        licenseLink->GetContainingSizer()->Layout();
        return;
    }

    currStage = MainFrame::Processing;

    confirmationPanel->Hide();
    processingPanel->Show();
    processingPanel->GetContainingSizer()->Layout();

    this->Bind(MESSAGE_FROM_THREAD, &MainFrame::OnThreadMessage, this);

    CheckAllFiles();
    std::thread work_horse(ProcessAllFilesW);
    work_horse.detach();
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
            confirmationInfo->WriteText(wxT("Âû âûáðàëè çàøèôðîâàòü âàøè ôàéëû íà ìåñòå, ýòî çíà÷èò ÷òî ýòè ôàéëû áóäóò ïåðåïèñàíû è åäèíñòâåííûé ñïîñîá èõ âîññòàíîâèòü - ýòî ðàñøèôðîâàòü èõ îáðàòíî ýòîé ïðîãðàììîé, èñïîëüçóÿ ïðè ýòîì òîò æå ñàìûé ïàðîëü êîòîðûé âû ââåëè ðàíåå.\n"));
        }
        
        confirmationInfo->BeginTextColour(wxColour(170, 0, 0));
        confirmationInfo->WriteText(wxT("ÂÀÆÍÎ: ÍÅ ÇÀÊÐÛÂÀÉÒÅ ÏÐÎÃÐÀÌÌÓ ÂÎ ÂÐÅÌß ÑËÅÄÓÞÙÅÃÎ ÝÒÀÏÀ! Åñëè çàêðîåòå, âàøè ôàéëû áóäóò ïîâðåæäåíû íàâñåãäà."));
        confirmationInfo->EndTextColour();
    }
    else
    {
        confirmationInfo->WriteText(wxString(wxT("Âñå âàøè ")) + (operatingMode & MainFrame::ENCRYPT ? wxT("çàøèôðîâàííûå") : wxT("ðàñøèôðîâàííûå")) + wxT(" ôàéëû áóäóò ñîçäàíû â ýòîé ïàïêå:\n"));
        confirmationInfo->WriteText(creationDir + wxT("\n"));
    }
    confirmationInfo->BeginBold();
    confirmationInfo->WriteText(wxT(" Åñëè âû çàáóäåòå ñâîé ïàðîëü, âàøè ôàéëû áóäåò íåâîçìîæíî âîññòàíîâèòü.\n"));
    confirmationInfo->EndBold();
}

void MainFrame::CheckAllFiles()
{
    int ios_flag = std::ios::out | std::ios::app; // very important std::ios::app
    for (size_t i = 0; i < userFiles.GetCount(); ++i)
    {
        auto evt = new wxCommandEvent(MESSAGE_FROM_THREAD);
        std::fstream tempfs(userFiles[i].fn_str(), ios_flag);
        if (!tempfs.is_open())
        {
            MyApp::ShowErrorMsg(wxT("Ôàéë ïî ïóòè ") + userFiles[i] +
                wxT(" íå áûë îòêðûò, âîçìîæíî ïîòîìó ÷òî ó ïðîãðàììû íåò ê íåìó äîñòóïà, ïîïðîáóéòå óáðàòü ñ ýòîãî ôàéëà \"Òîëüêî ÷òåíèå\" èëè çàïóñòèòü ýòó ïðîãðàììó â ðåæèìå àäìèíèñòðàòîðà."),
                wxT("Îøèáêà: ôàéë íå áûë îòêðûò"));
		this->KillApp();
		return;
        }
        tempfs.seekp(0, std::ios::end);
        totalBytes += tempfs.tellp();
        evt->SetClientData(new ThreadMessage(ThreadMessage::CHANGE_SINGLE_GAUGE, userFiles.GetCount(), i + 1));
        wxQueueEvent(this, evt);
    }
}

void MainFrame::ProcessAllFiles()
{
    for (size_t i = 0; i < userFiles.GetCount(); ++i)
    {
        {
            auto evt = new wxCommandEvent(MESSAGE_FROM_THREAD);
            evt->SetClientData(new ThreadMessage(ThreadMessage::CHANGE_PROCESSING_TEXT, -1, -1, wxString(std::filesystem::path(userFiles[i].fn_str()).filename())));
            wxQueueEvent(this, evt);
        }
        {
            auto evt = new wxCommandEvent(MESSAGE_FROM_THREAD);
            evt->SetClientData(new ThreadMessage(ThreadMessage::CHANGE_FILES_AMOUNT_TEXT, -1, -1, wxString::Format(wxT("%i"), i) + wxT(" ôàéëîâ èç ") + wxString::Format(wxT("%i"), userFiles.GetCount())));
            wxQueueEvent(this, evt);
        }
        FastEncryptFile(userFiles[i], password.ToStdString(), operatingMode & MainFrame::ENCRYPT, in_place, creationDir);
        
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
        endTextVal = (operatingMode & MainFrame::ENCRYPT ? wxString(wxT("Çàøèôðîâàííûå")) : wxString(wxT("Ðàñøèôðîâàííûå"))) + wxT(" ôàéëû áûëè ñîçäàíû â ") + creationDir + wxT("\n");
    }
    endTextVal.append(wxT("Ïðîãðàììà îòðàáîòàëà, ñìîòðèòå ñîîáùåíèÿ íèæå åñëè ïðîèçîøëè îøèáêè."));
    endTextVal.append(wxT("\nËîã:\n") + (NPClog.empty() ? wxString(wxT("Ïóñòî")) : NPClog));
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
        evt->SetClientData(new ThreadMessage(ThreadMessage::LOG_APPEND, -1, -1, wxT("Îøèáêà: ôàéë ïî ïóòè ") + path_to_file +
            wxT(" íå áûë îòêðûò, âîçìîæíî ïîòîìó ÷òî ó ïðîãðàììû íåò ê íåìó äîñòóïà, ïîïðîáóéòå óáðàòü ñ ýòîãî ôàéëà \"Òîëüêî ÷òåíèå\" èëè çàïóñòèòü ýòó ïðîãðàììó â ðåæèìå àäìèíèñòðàòîðà.\n")));
        wxQueueEvent(mainFrame, evt);
        return std::string();
    }
    if (!clonedFile.is_open() && !in_place)
    {
        auto evt = new wxCommandEvent(MESSAGE_FROM_THREAD);
        evt->SetClientData(new ThreadMessage(ThreadMessage::LOG_APPEND, -1, -1, wxT("Îøèáêà: íåâîçìîæíî ñîçäàòü ôàéë â ïàïêå ") + writeDir +
            wxT(" ïîïðîáóéòå çàïóñòèòü ýòó ïðîãðàììó â ðåæèìå àäìèíèñòðàòîðà èëè îñâîáîäèòå ìåñòî íà äèñêå.\n")));
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
            processedBytes += delim;
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
        evt->SetClientData(new ThreadMessage(ThreadMessage::LOG_APPEND, -1, -1, wxString(wxT("Íåâîçìîæíî áåçîïàñíî ")) + (encrypt ? wxT("çàøèôðîâàòü") : wxT("ðàñøèôðîâàòü")) + wxT(" ôàéë ïî ïóòè ") + path_to_file +
            wxT(" ïîòîìó, îí áûë ïðîïóùåí ïðîãðàììîé. Ïîïðîáîéòå çàïóñòèòü ýòó ïðîãðàììó â ðåæèìå àäìèíèñòðàòîðà èëè îñâîáîäèòå ìåñòî íà äèñêå.\n")));
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
        MyApp::ShowErrorMsg(wxT("Íåâîçìîæíî ñîçäàòü ïàïêó â ") + wxString(parentDir) + " ïîïðîáóéòå çàïóñòèòü ýòó ïðîãðàììó â ðåæèìå àäìèíèñòðàòîðà.",
            wxT("Îøèáêà: íåâîçìîæíî ñîçäàòü ïàïêó"));
    }
    return path_out;
}
