//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "mainfile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::IdHTTPServerConnect(TIdContext *AContext)
{
     //httpConnectLabel->Caption = "HTTP connect ok";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::IdHTTPServerDisconnect(TIdContext *AContext)
{
     //httpConnectLabel->Caption = "no HTTP connect";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::IdTCPServerExecute(TIdContext *AContext)
{
     //tcpConnectLabel->Caption = "TCP connect ok";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::TCPButtonClick(TObject *Sender)
{
  if(!IdTCPServer->Active)
  {
    IdTCPServer->Active = true;
    TCPButton->Caption = "Stop TCP server";
    InfoMemo->Lines->Add("Start TCP server, port " + IntToStr(IdTCPServer->DefaultPort));
  }
  else
  {
    IdTCPServer->Active = false;
    TCPButton->Caption = "Start TCP server";
    InfoMemo->Lines->Add("Stop TCP server");
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::WebButtonClick(TObject *Sender)
{
  if(!IdHTTPServer->Active)
  {
    IdHTTPServer->Active = true;
    WebButton->Caption = "Stop Web server";
    InfoMemo->Lines->Add("Start Web server, port " + IntToStr(IdHTTPServer->DefaultPort));
  }
  else
  {
    IdHTTPServer->Active = false;
    WebButton->Caption = "Start Web server";
    InfoMemo->Lines->Add("Stop Web server");
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ClearButtonClick(TObject *Sender)
{
  InfoMemo->Lines->Clear();
}
//---------------------------------------------------------------------------

