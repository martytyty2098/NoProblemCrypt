///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.0.0-0-g0efcecf)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "mna.h"

///////////////////////////////////////////////////////////////////////////

frameMain::frameMain( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerFrameMain;
	bSizerFrameMain = new wxBoxSizer( wxVERTICAL );

	greetingText = new wxRichTextCtrl( this, wxID_ANY, wxT("Как использовать: Нажмите на кнопку \"Файл\" в верхнем левом углу экрана.\n\nО программе:\nЭта программа создана для шифрования файлов любого типа. Любой файл зашифрованный этой программой может быть расшифрован только этой программой, используя тот же самый пароль который вы ввели при шифровании.\nВосстановить файлы без этого пароля будет невозможно, поэтому если вы утратите пароль, то потеряете ваши файлы навсегда, можете считать это как плюсом, так и минусом.\n\nРазработчик:\nЭлектронная почта: shkafchik.game@gmail.com\nИсходный код: https://github.com/martytyty2098/NoProblemCrypt\nАккаунт на Github: https://github.com/martytyty2098\n"), wxDefaultPosition, wxSize( -1,-1 ), wxTE_READONLY|wxBORDER_SIMPLE|wxHSCROLL|wxVSCROLL|wxWANTS_CHARS );
	greetingText->SetFont( wxFont( 16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Consolas") ) );

	bSizerFrameMain->Add( greetingText, 1, wxALL|wxEXPAND, 5 );

	panelMain = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	panelMain->Hide();

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	infoText = new wxStaticText( panelMain, wxID_ANY, wxT("Вы выбрали 0 файлов"), wxPoint( 100,200 ), wxSize( -1,-1 ), wxALIGN_CENTER_HORIZONTAL );
	infoText->Wrap( -1 );
	infoText->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	bSizer5->Add( infoText, 0, wxALIGN_CENTER|wxTOP, 40 );

	in_place_checkbox = new wxCheckBox( panelMain, wxID_ANY, wxT("Переписать на месте"), wxDefaultPosition, wxSize( -1,-1 ), 0|wxBORDER_THEME|wxWANTS_CHARS );
	in_place_checkbox->SetFont( wxFont( 16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Consolas") ) );
	in_place_checkbox->SetToolTip( wxT("Не рекомендовано, если выбрано, файлы буду переписаны на месте, иначе будут созданы отдельно в выбранной ниже папке") );
	in_place_checkbox->SetHelpText( wxT("Не рекомендовано, если выбрано, файлы буду переписаны на месте, иначе будут созданы отдельно в выбранной ниже папке") );

	bSizer5->Add( in_place_checkbox, 0, wxALIGN_CENTER|wxTOP, 30 );

	dirPicker = new wxDirPickerCtrl( panelMain, wxID_ANY, wxT("Где новые файлы будут созданы"), wxT("Выберите папку"), wxDefaultPosition, wxSize( 300,-1 ), wxDIRP_DIR_MUST_EXIST|wxDIRP_USE_TEXTCTRL );
	dirPicker->SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	bSizer5->Add( dirPicker, 0, wxALIGN_CENTER|wxTOP, 14 );

	mainButton = new wxButton( panelMain, wxID_ANY, wxT("Зашифровать"), wxDefaultPosition, wxSize( 200,45 ), 0 );

	mainButton->SetDefault();
	mainButton->SetFont( wxFont( 18, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Consolas") ) );

	bSizer5->Add( mainButton, 0, wxALIGN_CENTER|wxTOP, 30 );

	invalidDirInfo = new wxInfoBar( panelMain );
	invalidDirInfo->SetShowHideEffects( wxSHOW_EFFECT_SLIDE_TO_RIGHT, wxSHOW_EFFECT_NONE );
	invalidDirInfo->SetEffectDuration( 500 );
	invalidDirInfo->SetFont( wxFont( 16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );
	invalidDirInfo->SetForegroundColour( wxColour( 255, 0, 0 ) );
	invalidDirInfo->Hide();

	bSizer5->Add( invalidDirInfo, 0, wxTOP, 25 );


	panelMain->SetSizer( bSizer5 );
	panelMain->Layout();
	bSizer5->Fit( panelMain );
	bSizerFrameMain->Add( panelMain, 1, wxALL|wxEXPAND, 0 );

	passwordPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	passwordPanel->Hide();

	wxBoxSizer* passwordSizer;
	passwordSizer = new wxBoxSizer( wxVERTICAL );

	passwordEnterText = new wxStaticText( passwordPanel, wxID_ANY, wxT("Теперь вы должны ввести пароль который нужно хранить в надёжном месте, если вы утратите свой пароль то не сможете расшифровать файлы обратно."), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	passwordEnterText->Wrap( 250 );
	passwordEnterText->SetFont( wxFont( 14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Consolas") ) );

	passwordSizer->Add( passwordEnterText, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP, 20 );

	m_staticText4 = new wxStaticText( passwordPanel, wxID_ANY, wxT("Нажмите TAB что бы показать/скрыть"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	m_staticText4->SetFont( wxFont( 10, wxFONTFAMILY_MODERN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_LIGHT, false, wxT("Cascadia Code Light") ) );

	passwordSizer->Add( m_staticText4, 0, wxALIGN_CENTER|wxTOP, 35 );

	passwordTxtCtrl = new wxTextCtrl( passwordPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,25 ), wxTE_CENTER|wxTE_NOHIDESEL|wxTE_PASSWORD|wxTE_PROCESS_TAB );
	passwordTxtCtrl->SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("System") ) );

	passwordSizer->Add( passwordTxtCtrl, 0, wxALIGN_CENTER|wxTOP, 5 );

	passwordTxtCtrlVisible = new wxTextCtrl( passwordPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,25 ), wxTE_CENTER|wxTE_NOHIDESEL|wxTE_PROCESS_TAB );
	passwordTxtCtrlVisible->SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("System") ) );
	passwordTxtCtrlVisible->Hide();

	passwordSizer->Add( passwordTxtCtrlVisible, 0, wxALIGN_CENTER|wxTOP, 5 );

	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( passwordPanel, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( passwordPanel, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();

	passwordSizer->Add( m_sdbSizer1, 0, wxALIGN_CENTER|wxTOP, 10 );


	passwordPanel->SetSizer( passwordSizer );
	passwordPanel->Layout();
	passwordSizer->Fit( passwordPanel );
	bSizerFrameMain->Add( passwordPanel, 1, wxEXPAND | wxALL, 5 );

	confirmationPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	confirmationPanel->Hide();

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	confirmationInfo = new wxRichTextCtrl( confirmationPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	confirmationInfo->SetFont( wxFont( 14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Consolas") ) );
	confirmationInfo->SetMaxSize( wxSize( -1,240 ) );

	bSizer4->Add( confirmationInfo, 1, wxEXPAND, 5 );

	licenseLink = new wxHyperlinkCtrl( confirmationPanel, wxID_ANY, wxT("Вы согласны с этим?"), wxT("https://github.com/martytyty2098/NoProblemCrypt/blob/main/LICENSE"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	licenseLink->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	bSizer4->Add( licenseLink, 0, wxALIGN_CENTER|wxTOP, 20 );

	licenseConfirmation = new wxStdDialogButtonSizer();
	licenseConfirmationYes = new wxButton( confirmationPanel, wxID_YES );
	licenseConfirmation->AddButton( licenseConfirmationYes );
	licenseConfirmationNo = new wxButton( confirmationPanel, wxID_NO );
	licenseConfirmation->AddButton( licenseConfirmationNo );
	licenseConfirmation->Realize();

	bSizer4->Add( licenseConfirmation, 0, wxALIGN_CENTER|wxTOP, 15 );


	confirmationPanel->SetSizer( bSizer4 );
	confirmationPanel->Layout();
	bSizer4->Fit( confirmationPanel );
	bSizerFrameMain->Add( confirmationPanel, 1, wxEXPAND | wxALL, 5 );

	processingPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	processingPanel->Hide();

	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxVERTICAL );

	progressState = new wxStaticText( processingPanel, wxID_ANY, wxT("Проверка файлов..."), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	progressState->Wrap( -1 );
	progressState->SetFont( wxFont( 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );

	bSizer51->Add( progressState, 0, wxALIGN_CENTER|wxTOP, 100 );

	singleProgressGauge = new wxGauge( processingPanel, wxID_ANY, 100, wxDefaultPosition, wxSize( 300,30 ), wxGA_HORIZONTAL );
	singleProgressGauge->SetValue( 0 );
	bSizer51->Add( singleProgressGauge, 0, wxALIGN_CENTER|wxTOP, 20 );

	totalProgressGauge = new wxGauge( processingPanel, wxID_ANY, 100, wxDefaultPosition, wxSize( 300,30 ), wxGA_HORIZONTAL );
	totalProgressGauge->SetValue( 0 );
	bSizer51->Add( totalProgressGauge, 0, wxALIGN_CENTER|wxTOP, 20 );

	filesAmountText = new wxStaticText( processingPanel, wxID_ANY, wxT("0 файлов из 0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	filesAmountText->Wrap( -1 );
	filesAmountText->SetFont( wxFont( 14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Consolas") ) );

	bSizer51->Add( filesAmountText, 0, wxALIGN_CENTER|wxTOP, 20 );


	processingPanel->SetSizer( bSizer51 );
	processingPanel->Layout();
	bSizer51->Fit( processingPanel );
	bSizerFrameMain->Add( processingPanel, 1, wxEXPAND | wxALL, 5 );

	endPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	endPanel->Hide();

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	endText = new wxRichTextCtrl( endPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	endText->SetFont( wxFont( 14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Consolas") ) );

	bSizer6->Add( endText, 1, wxEXPAND | wxALL, 0 );


	endPanel->SetSizer( bSizer6 );
	endPanel->Layout();
	bSizer6->Fit( endPanel );
	bSizerFrameMain->Add( endPanel, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizerFrameMain );
	this->Layout();
	m_statusBar2 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	menubarMain = new wxMenuBar( 0 );
	menuFile = new wxMenu();
	wxMenuItem* menuEncryptFiles;
	menuEncryptFiles = new wxMenuItem( menuFile, EncryptFileID, wxString( wxT("Зашифровать файлы") ) + wxT('\t') + wxT("F1"), wxT("Укажите на несколько файлов"), wxITEM_NORMAL );
	menuFile->Append( menuEncryptFiles );

	wxMenuItem* menuDecryptFiles;
	menuDecryptFiles = new wxMenuItem( menuFile, DecryptFileID, wxString( wxT("Расшифровать файлы") ) + wxT('\t') + wxT("F2"), wxT("Укажите на несколько файлов"), wxITEM_NORMAL );
	menuFile->Append( menuDecryptFiles );

	menuFile->AppendSeparator();

	wxMenuItem* menuEncryptFolder;
	menuEncryptFolder = new wxMenuItem( menuFile, EncryptFolderID, wxString( wxT("Зашифровать папку") ) + wxT('\t') + wxT("SHIFT+F1"), wxT("Укажите на папку"), wxITEM_NORMAL );
	menuFile->Append( menuEncryptFolder );

	wxMenuItem* menuDecryptFolder;
	menuDecryptFolder = new wxMenuItem( menuFile, DecryptFolderID, wxString( wxT("Расшифровать папку") ) + wxT('\t') + wxT("SHIFT+F2"), wxT("Укажите на папку"), wxITEM_NORMAL );
	menuFile->Append( menuDecryptFolder );

	menubarMain->Append( menuFile, wxT("Файл") );

	this->SetMenuBar( menubarMain );


	this->Centre( wxBOTH );

	// Connect Events
	in_place_checkbox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( frameMain::InPlaceChange ), NULL, this );
	mainButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frameMain::MainButtonOnClick ), NULL, this );
	passwordTxtCtrl->Connect( wxEVT_CHAR, wxKeyEventHandler( frameMain::OnTabPressed ), NULL, this );
	passwordTxtCtrlVisible->Connect( wxEVT_CHAR, wxKeyEventHandler( frameMain::OnTabPressed ), NULL, this );
	m_sdbSizer1Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frameMain::FromPasswordToMenu ), NULL, this );
	m_sdbSizer1OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frameMain::OnPasswordSubmit ), NULL, this );
	licenseConfirmationYes->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frameMain::FromConfirmToProcess ), NULL, this );
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frameMain::ShowMenu ), this, menuEncryptFiles->GetId());
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frameMain::ShowMenu ), this, menuDecryptFiles->GetId());
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frameMain::ShowMenu ), this, menuEncryptFolder->GetId());
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frameMain::ShowMenu ), this, menuDecryptFolder->GetId());
}

frameMain::~frameMain()
{
	// Disconnect Events
	in_place_checkbox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( frameMain::InPlaceChange ), NULL, this );
	mainButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frameMain::MainButtonOnClick ), NULL, this );
	passwordTxtCtrl->Disconnect( wxEVT_CHAR, wxKeyEventHandler( frameMain::OnTabPressed ), NULL, this );
	passwordTxtCtrlVisible->Disconnect( wxEVT_CHAR, wxKeyEventHandler( frameMain::OnTabPressed ), NULL, this );
	m_sdbSizer1Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frameMain::FromPasswordToMenu ), NULL, this );
	m_sdbSizer1OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frameMain::OnPasswordSubmit ), NULL, this );
	licenseConfirmationYes->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frameMain::FromConfirmToProcess ), NULL, this );

}
