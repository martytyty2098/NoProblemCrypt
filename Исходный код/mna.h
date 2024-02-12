///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.0.0-0-g0efcecf)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/filepicker.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/infobar.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/hyperlink.h>
#include <wx/gauge.h>
#include <wx/statusbr.h>
#include <wx/menu.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define EncryptFileID 1000
#define DecryptFileID 1001
#define EncryptFolderID 1002
#define DecryptFolderID 1003

///////////////////////////////////////////////////////////////////////////////
/// Class frameMain
///////////////////////////////////////////////////////////////////////////////
class frameMain : public wxFrame
{
	private:

	protected:
		wxRichTextCtrl* greetingText;
		wxPanel* panelMain;
		wxStaticText* infoText;
		wxCheckBox* in_place_checkbox;
		wxDirPickerCtrl* dirPicker;
		wxButton* mainButton;
		wxInfoBar* invalidDirInfo;
		wxPanel* passwordPanel;
		wxStaticText* passwordEnterText;
		wxStaticText* m_staticText4;
		wxTextCtrl* passwordTxtCtrl;
		wxTextCtrl* passwordTxtCtrlVisible;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
		wxPanel* confirmationPanel;
		wxRichTextCtrl* confirmationInfo;
		wxHyperlinkCtrl* licenseLink;
		wxStdDialogButtonSizer* licenseConfirmation;
		wxButton* licenseConfirmationYes;
		wxButton* licenseConfirmationNo;
		wxPanel* processingPanel;
		wxStaticText* progressState;
		wxGauge* singleProgressGauge;
		wxGauge* totalProgressGauge;
		wxStaticText* filesAmountText;
		wxPanel* endPanel;
		wxRichTextCtrl* endText;
		wxStatusBar* m_statusBar2;
		wxMenuBar* menubarMain;
		wxMenu* menuFile;

		// Virtual event handlers, override them in your derived class
		virtual void InPlaceChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void MainButtonOnClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTabPressed( wxKeyEvent& event ) { event.Skip(); }
		virtual void FromPasswordToMenu( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPasswordSubmit( wxCommandEvent& event ) { event.Skip(); }
		virtual void FromConfirmToProcess( wxCommandEvent& event ) { event.Skip(); }
		virtual void ShowMenu( wxCommandEvent& event ) { event.Skip(); }


	public:

		frameMain( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("NoProblemCrypt"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 525,425 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~frameMain();

};

