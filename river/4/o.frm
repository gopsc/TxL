VERSION 5.00
Begin VB.Form o 
   BackColor       =   &H80000008&
   BorderStyle     =   0  'None
   Caption         =   "AUDIO"
   ClientHeight    =   1290
   ClientLeft      =   19680
   ClientTop       =   12240
   ClientWidth     =   2280
   FillColor       =   &H00FFFFFF&
   ForeColor       =   &H8000000E&
   Icon            =   "o.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   86
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   152
   ShowInTaskbar   =   0   'False
   Begin VB.Timer Timer2 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   1440
      Top             =   120
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   840
      Top             =   120
   End
   Begin VB.ComboBox ����ѡ�� 
      Appearance      =   0  'Flat
      BackColor       =   &H80000001&
      Height          =   300
      Left            =   240
      TabIndex        =   0
      Top             =   600
      Visible         =   0   'False
      Width           =   2655
   End
End
Attribute VB_Name = "o"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Function Polyline Lib "gdi32.dll" (ByVal hdc As Long, ByRef lpPoint As POINTAPI, ByVal nCount As Long) As Long
Dim oo() As POINTAPI
Dim ��Ƶ���� As New clsWaveIn, ������() As Integer
Dim �������� As Single



Private Sub Timer2_Timer()
    
    p(oooo) = ������(o5)
    
    
    oooo = oooo + 1
    If oooo = 10001 Then
        oooo = 0
        ReDim p(10000)
        o6 = 0
        o7 = 0
    End If
    
    
    o5 = o5 + 1
    
    
    
    If o5 = ��Ƶ����.BufferSize \ 2& Then
        o5 = 0
    End If
    
    
    
    
    
    


   
    If oooo > 0 Then
        If p(oooo - 1) / 65535! > 0.1 Or p(oooo - 1) / 65535! < -0.1 Then
            o7 = 1
            o6 = 0
        Else
            If o7 = 1 Then
                o6 = o6 + 1
            End If
        End If
    End If
    
    
    If o7 = 0 And oooo > 1 Then
        If (p(oooo) - p(oooo - 1) <= 0 And p(oooo + 1) - p(oooo) > 0) Or ((p(oooo) - p(oooo - 1) >= 0 And p(oooo + 1) - p(oooo) < 0)) Or (p(oooo - 1) >= 0 And p(oooo) < 0) Or (p(oooo - 1) <= 0 And p(oooo) > 0) Then
            ReDim p(10000)
            oooo = 0
        End If
    Else
        If o6 = 20 Then
            o6 = 0
            o7 = 0
            Timer2.Enabled = False
            Call ʶ��
        End If
    End If
    
    
    
End Sub

Private Sub ����ѡ��_Click()
    Dim �����豸��ʽ As Long, i As Long, l As Long
    If (����ѡ��.ListIndex < 0) Then Exit Sub
    ��ʼ
End Sub

Private Sub ʶ��()
    qop.Cls
    qop.Show
    Dim e As Long, r As String, u As Long, s As Long, d As Long
    u = 1
    
    For e = 1 To oooo
        qop.Line (e * 20, (0.5 - p(e) / 65535!) * qop.Height)-(e * 20 + 20, (0.5 - p(e + 1) / 65535!) * qop.Height), vbGreen
        If (p(e) - p(e - 1) <= 0 And p(e + 1) - p(e) > 0) Or ((p(e) - p(e - 1) >= 0 And p(e + 1) - p(e) < 0)) Or (p(e - 1) >= 0 And p(e) < 0) Or (p(e - 1) <= 0 And p(e) > 0) Then
            qop.Line (u * 80, (0.5 - d) * qop.Height)-(u * 80 + 80, (0.5 - e / 1000 + s) * qop.Height), vbRed
            u = u + 1
            d = (e - s) / 1000
            s = e / 1000
        End If
    Next
    
    ReDim p(10000)
    oooo = 0
    
    Timer2.Enabled = True
End Sub


Private Sub Timer1_Timer()
Timer1.Enabled = False
    Dim ������ָ�� As Long, i As Long, m As Long, w As Long, h As Long
    If (��Ƶ����.BytesPerChannel = 2) Then
        ������ָ�� = VarPtr(������(0))
                            '������ָ���ǻ������ĵ�ַ
    End If
    Do
        If (��Ƶ����.ReadRecording(������ָ��, 1) = 0) Then Exit Do
    Loop
Call ������
End Sub




Private Sub ��ʼ()
    Dim i As Long, Ƶ�� As Long
    Ƶ�� = 44000
    If Not ��Ƶ����.Create(����ѡ��.ListIndex, Ƶ��, 2, 2, Ƶ�� / 25, 16) Then
        Exit Sub
    End If
  Dim io As Long
  ReDim oo(o.ScaleWidth) As POINTAPI
  For io = 0 To o.ScaleWidth
     oo(io).X = io
     oo(io).Y = 0.5 * o.ScaleHeight
  Next
   
     
    Call ��Ƶ����.StartRecording
    Timer1.Enabled = True
        ReDim ������(��Ƶ����.BufferSize \ 2& - 1) As Integer
        ReDim a(��Ƶ����.BufferSize \ 2& - 1) As POINTAPI
        ReDim p(10000) As Long
        Timer2.Enabled = True
End Sub
Private Sub Form_click()
End
End Sub
Private Sub Form_Load()
Dim i As Long, b As Long
����ѡ��.Clear
��Ƶ����.EnumerateWaveInDevice

For i = 0 To ��Ƶ����.WaveInDeviceCount - 1
        Call ����ѡ��.AddItem(��Ƶ����.WaveInDeviceName(i))
        If (i = (��Ƶ����.WaveInDeviceCount - 1)) Then ����ѡ��.ListIndex = 0
    Next i
End Sub

Private Sub ������()
    Dim m As Long, w As Long, h As Long, l() As Long, i As Long, ii As Long
    m = ��Ƶ����.SamplesPerBuffer
    ReDim l(m) As Long, q(m) As Long
    o.Cls
    w = o.ScaleWidth
    h = o.ScaleHeight
    For i = 0 To o.ScaleWidth - 1
        l(i) = oo(i + 1).Y
        oo(i).Y = oo(i + 1).Y
    Next i
     l(o.ScaleWidth) = (0.5! - (������(0) / 65535!)) * h
      oo(o.ScaleWidth).Y = l(o.ScaleWidth)
     For i = 0 To o.ScaleWidth
    Me.Line (i, l(i))-(i + 1, l(i + 1)), RGB(0, 0, 255)
 '   Me.PSet (i, l(i)), vbBlue
    Next
    
    
Timer1.Enabled = True
End Sub
