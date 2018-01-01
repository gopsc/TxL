VERSION 5.00
Begin VB.Form ���� 
   Caption         =   "AUDIO"
   ClientHeight    =   4500
   ClientLeft      =   1740
   ClientTop       =   2865
   ClientWidth     =   8325
   Icon            =   "����.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   300
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   555
   Begin VB.HScrollBar HScroll1 
      Height          =   255
      Left            =   0
      Max             =   24
      Min             =   -24
      TabIndex        =   5
      Top             =   720
      Width           =   8295
   End
   Begin VB.PictureBox BX 
      BackColor       =   &H00000000&
      FillColor       =   &H0000FF00&
      ForeColor       =   &H0000FF00&
      Height          =   3255
      Left            =   0
      ScaleHeight     =   213
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   549
      TabIndex        =   4
      Top             =   1200
      Width           =   8295
   End
   Begin VB.ComboBox �������ѡ�� 
      Height          =   300
      Left            =   1200
      TabIndex        =   2
      Top             =   360
      Width           =   7095
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   8640
      Top             =   120
   End
   Begin VB.ComboBox ����ѡ�� 
      Height          =   300
      Left            =   1200
      TabIndex        =   0
      Top             =   0
      Width           =   7095
   End
   Begin VB.Label Label3 
      Caption         =   "��������:0.1"
      BeginProperty Font 
         Name            =   "����"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   1200
      TabIndex        =   6
      Top             =   960
      Width           =   2655
   End
   Begin VB.Label Label2 
      Caption         =   "ѡ������豸:"
      Height          =   255
      Left            =   0
      TabIndex        =   3
      Top             =   360
      Width           =   3495
   End
   Begin VB.Label Label1 
      Caption         =   "ѡ�������豸:"
      Height          =   375
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   2655
   End
End
Attribute VB_Name = "����"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Function DeleteObject Lib "gdi32.dll" (ByVal hObject As Long) As Long
Private Declare Function CreatePen Lib "gdi32.dll" (ByVal nPenStyle As Long, ByVal nWidth As Long, ByVal crColor As Long) As Long
Private Declare Function SelectObject Lib "gdi32.dll" (ByVal hdc As Long, ByVal hObject As Long) As Long
Private Declare Function Polyline Lib "gdi32.dll" (ByVal hdc As Long, ByRef lpPoint As POINTAPI, ByVal nCount As Long) As Long
Private Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal X As Long, ByVal Y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
'Download by http://www.codefans.net
Private Declare Sub CopyMemory Lib "kernel32.dll" Alias "RtlMoveMemory" (ByRef Destination As Any, ByRef Source As Any, ByVal Length As Long)
Dim ��Ƶ���� As New clsWaveIn, ��Ƶ��� As New clsWaveOut, ���ݴ��� As New IWaveOut_Opus
Private ��ʽ����(19) As String
Dim ������() As Byte, ������2() As Integer
Dim ������� As New OutDevices, ����������� As Long
Dim �������� As Single
Private Type POINTAPI
    X As Long
    Y As Long
End Type

Private Sub BX_DblClick()
    ����S.Show
End Sub

Private Sub Form_Load()
    Dim i As Long, s As String
    For i = 0 To 3
        s = "Hz, " + CStr(8 + (i And 2) * 4) + "bit, " + Mid("Mono  Stereo", 1 + (i And 1) * 6, 6)
        ��ʽ����(i) = "11025" + s
        ��ʽ����(i + 4) = "22050" + s
        ��ʽ����(i + 8) = "44100" + s
        ��ʽ����(i + 12) = "48000" + s
        ��ʽ����(i + 16) = "96000" + s
    Next i
    �������� = 1
    ����ѡ��.Clear
    ��Ƶ����.EnumerateWaveInDevice
    For i = 0 To ��Ƶ����.WaveInDeviceCount - 1
        Call ����ѡ��.AddItem(��Ƶ����.WaveInDeviceName(i))
        If (i = (��Ƶ����.WaveInDeviceCount - 1)) Then ����ѡ��.ListIndex = 0
    Next i
    ����������� = �������.initOutDevice
    For i = 0 To ����������� - 1
        �������ѡ��.AddItem (�������.GetName(i))
    Next
    �������ѡ��.ListIndex = 0
    Call HScroll1_Change
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ��Ƶ����.Destroy
    ��Ƶ���.Destroy
    ���ݴ���.Destroy
    Timer1.Enabled = False
    End
End Sub

Private Sub HScroll1_Change()
    �������� = 2 ^ (HScroll1.Value / 8)
    Label3.Caption = ("��������:" & Format((��������), "0.000"))
End Sub

Private Sub HScroll1_Scroll()
    HScroll1_Change
End Sub

Private Sub Timer1_Timer()
    Dim ������ָ�� As Long, ���������� As Long, i As Long, ii As Long
    If (��Ƶ����.BytesPerChannel = 2) Then
        ������ָ�� = VarPtr(������2(0))
    Else
        ������ָ�� = VarPtr(������(0))
    End If
    Do
        If (��Ƶ����.ReadRecording(������ָ��, 1) = 0) Then Exit Do
        If ��Ƶ����.BytesPerChannel <> 2 Then
            For i = 0 To ��Ƶ����.BufferSize - 1
                ������2(i) = (������(i) - &H80&) * &H100&
            Next i
        End If
        If �������� <> 1! Then
            For i = 0 To (��Ƶ����.SamplesPerBuffer * ��Ƶ����.Channels) - 1
                ii = ������2(i) * ��������
                If ii > &H7FFF& Then ii = &H7FFF&
                If ii < &HFFFF8000 Then ii = &HFFFF8000
                ������2(i) = ii
            Next i
        End If
        'Call ������
        Call ���ݴ���.TestFeed(VarPtr(������2(0)), ((UBound(������2) + 1) * 2))
        Call ��Ƶ���.OnTimer
    Loop
    Call ������
    Call ��ȫ���沨��
End Sub

Private Sub ��ʼ()
    Dim i As Long, Ƶ�� As Long
    '����:�������,Ƶ��,?,?,Ƶ��/25
    Ƶ�� = 44000
    If Not ��Ƶ����.Create(����ѡ��.ListIndex, Ƶ��, 2, 2, Ƶ�� / 25, 16) Then
        Exit Sub '����ʧ��
    End If
    Call ��Ƶ����.StartRecording '��ʼ¼��?
    'ReDim ������(��Ƶ����.BufferSize - 1) As Byte, ������2((��Ƶ����.BufferSize / 2) - 1) As Integer
    Timer1.Enabled = True
    Call ���ݴ���.TestStart
    Call ��Ƶ���.Create(Ƶ��, 2, 2, Ƶ�� / 25, 2, �������ѡ��.ListIndex)
    If ��Ƶ���.BytesPerChannel = 2 Then
        ReDim ������2(��Ƶ����.BufferSize \ 2& - 1) As Integer
    Else
        ReDim ������(��Ƶ����.BufferSize - 1) As Byte
        ReDim ������2(��Ƶ����.BufferSize \ 2& - 1) As Integer
    End If
    Set ��Ƶ���.DataObject = ���ݴ���
    Call ��Ƶ���.Play
End Sub

Private Sub ����ѡ��_Click()
    Dim �����豸��ʽ As Long, i As Long, l As Long
    If (����ѡ��.ListIndex < 0) Then Exit Sub
    �������ѡ��_Click
'    ����ѡ��.Clear
'    �����豸��ʽ = ��Ƶ����.WaveInDeviceFormats(����ѡ��.ListIndex)
'    l = &H1000&
'    For i = 12 To 15
'        If �����豸��ʽ And l Then
'            Call ����ѡ��.AddItem(��ʽ����(i) & ("[��ʱ��֧���Լ��趨Ƶ��]"))
'            ����ѡ��.ItemData(����ѡ��.ListCount - 1) = l
'            ����ѡ��.ListIndex = ����ѡ��.ListCount - 1
'        End If
'    Next i
End Sub

Private Sub ������()
    Dim m As Long, w As Long, h As Long, l() As POINTAPI, i As Long, ii As Long, hbr As Long, s As String
    m = ��Ƶ����.SamplesPerBuffer
    ReDim l(m - 1) As POINTAPI
    BX.Cls
    w = BX.Width
    h = BX.Height
    For i = 0 To m - 1
        l(i).X = (i * w) \ m
        l(i).Y = (0.5! - (������2(i) / 65535!)) * h
    Next i
    Call Polyline(BX.hdc, l(0), m - 1)
End Sub


Private Sub ��ȫ���沨��()
    Dim m As Long, w As Long, h As Long, l() As POINTAPI, i As Long, ii As Long, hbr As Long, s As String
    m = ��Ƶ����.SamplesPerBuffer
    ReDim l(m - 1) As POINTAPI
    ����S.Cls
    w = ����S.ScaleWidth
    h = ����S.ScaleHeight
    For i = 0 To m - 1
        l(i).X = (i * w) \ m
        l(i).Y = (0.5! - (������2(i) / 65535!)) * h
    Next i
    Call Polyline(����S.hdc, l(0), m - 1)
End Sub

Private Sub �������ѡ��_Click()
    ��Ƶ����.Destroy
    ��Ƶ���.Destroy
    ���ݴ���.Destroy
    Timer1.Enabled = False
    ��ʼ
End Sub
