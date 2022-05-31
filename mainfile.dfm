object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'Locate Server'
  ClientHeight = 472
  ClientWidth = 833
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 144
  TextHeight = 19
  object InfoMemo: TMemo
    Left = 8
    Top = 16
    Width = 817
    Height = 153
    Color = clBackground
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clLime
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Lines.Strings = (
      '')
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 453
    Width = 833
    Height = 19
    Panels = <
      item
        Text = 'TCP clients: 0'
        Width = 150
      end
      item
        Text = 'Web clients: 0'
        Width = 150
      end>
    ExplicitLeft = 624
    ExplicitTop = 448
    ExplicitWidth = 0
  end
  object TCPButton: TButton
    Left = 8
    Top = 416
    Width = 145
    Height = 25
    Caption = 'Start TCP server'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = TCPButtonClick
  end
  object WebButton: TButton
    Left = 168
    Top = 416
    Width = 145
    Height = 25
    Caption = 'Start Web server'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = WebButtonClick
  end
  object ClearButton: TButton
    Left = 712
    Top = 416
    Width = 113
    Height = 25
    Caption = 'Clear log'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = ClearButtonClick
  end
  object IdTCPServer: TIdTCPServer
    Bindings = <>
    DefaultPort = 1234
    MaxConnections = 15
    OnExecute = IdTCPServerExecute
    Left = 80
    Top = 296
  end
  object IdHTTPServer: TIdHTTPServer
    Bindings = <>
    MaxConnections = 15
    OnConnect = IdHTTPServerConnect
    OnDisconnect = IdHTTPServerDisconnect
    AutoStartSession = True
    Left = 16
    Top = 296
  end
end
