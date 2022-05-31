//---------------------------------------------------------------------------

#ifndef mainfileH
#define mainfileH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdCustomTCPServer.hpp>
#include <IdTCPServer.hpp>
#include <IdCustomHTTPServer.hpp>
#include <IdHTTPServer.hpp>
#include <IdContext.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
    TIdTCPServer *IdTCPServer;
    TIdHTTPServer *IdHTTPServer;
    TMemo *InfoMemo;
    TStatusBar *StatusBar;
    TButton *TCPButton;
    TButton *WebButton;
    TButton *ClearButton;
    void __fastcall IdHTTPServerConnect(TIdContext *AContext);
    void __fastcall IdTCPServerExecute(TIdContext *AContext);
    void __fastcall IdHTTPServerDisconnect(TIdContext *AContext);
    void __fastcall TCPButtonClick(TObject *Sender);
    void __fastcall WebButtonClick(TObject *Sender);
    void __fastcall ClearButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
