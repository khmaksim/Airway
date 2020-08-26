''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'Author Maksim Kh.
'Aviacominfo.com
'Drawing on map FIR
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

Const AA = 6378245.0
Const BB = 6356863.0
Const FileAdres = "D:\fir.txt"
Const DEGRAD = 0.01745329251994 '2p/360'
Const PI4    = 0.7853981633974

Public dId as long
Public ret as long
Public Txt as string
Public pX as string
Public pY as string

Public prjNameD as string
Public mapScaleD as string
Public drLat1cD as string
Public drLat2cD as string
Public coordSysD as string
Public angleND as string
Public drCLatcD as string
Public drCLoncD as string
Public coord1D as string
Public coord2D as string
Public vyspD as string
Public prevpD as string

Dim prjName as String
Dim mapScale as Double
Dim drLat1c as String
Dim drLat2c as String
Dim coordSys as String
Dim angleN as Double
Dim drCLatc as String
Dim drCLonc as String

'Coord map
Dim drTopLeft as double, drTopRight as double, drBottomLeft as double, drBottomRight as double
Dim ALFA As Double, DC As Double, K As Double, E2 As Double, C as Double, RoS  As Double
Dim drCLat as Double, drCLon as Double

Public name1 As String
Public name2 As String
Public note1 As String
Public note2 As String
Public nameRoute As String
Public course1 As String
Public course2 As String
Public lat1 As String, lat2 As String, lon1 As String, lon2 As String
Public distanceKm As String, distanceNm As String
Public nLat1 As Integer, sLat1 As Integer, wLon1 As Integer, eLon1 As Integer, nLat2 As Integer, sLat2 As Integer, wLon2 As Integer, eLon2 As Integer

Declare Sub ProcessIndividualElem(inElem as MbeElement)
Declare Sub ProcessElement(inElem as MbeElement)
Declare Sub fnXY(drLatRad As Double,drLonRad As Double,drX As Double,drY As Double) 
Declare Sub fnAlfaK(drLatRadN As Double,drLatRadS As Double,drScale As Double)
Declare Sub XYtoSTR(drX As Double, drY As Double, MSSTR As String)
Declare Sub PlaceGeoLine(lat() as String, lon() as String)
Declare Sub GetCenterOfPolygon(lat() As String, lon() As String, nameZoneCodeIcao As String, nameSector As String, callFuncFreq As String)
Declare Function PrecLatToRad(ll as String) As Double
Declare Function fnDelta(drLonRad As Double) As Double
Declare Function fnRo(drLatRad As Double) As Double
Declare Function fnU(drLatRad As Double) As Double
Declare Function ASin(drN As Double) As Double
Declare Function fnTg(drAnglRad As Double) As Double
Declare Function pow(drX As Double,drY As Double) As Double
Declare Function LatToRad(ll as String) As Double 
Declare Function LonToRad(ll as String) As Double
Declare Function fnE2( ) As Double
Declare Function fnR(drLatRad As Double) As Double
Declare Function fnN(drLatRad As Double) As Double 
Declare Function log10(drN As Double) As Double
Declare Function ARINCtoRNCLat(CRD as String) As String
Declare Function ARINCtoRNCLon(CRD as String) As String
Declare Function PrecLonToRad(ll as String) As Double 

Function PrecLatToRad(ll as String) As Double
    Dim drTmp As Double
    Dim typ   As String
      
    drTmp# = Val(Mid$(ll,2,2))
    drTmp# = drTmp + Val(Mid$(ll,5,2)) / 60.0
    drTmp# = drTmp + Val(Mid$(ll,8,2)) / 60.0 / 60.0
      drTmp# = drTmp * DEGRAD
      
      typ = Mid$(ll,1,1)
      
      if typ = "S" Or typ = "Ю" Then drTmp = -drTmp
      if typ = "s" Or typ = "ю" Then drTmp = -drTmp
      
      PrecLatToRad = drTmp

End Function

Function PrecLonToRad(ll as String) As Double
    Dim drTmp As Double, typ As String
    
    drTmp# = Val(Mid$(ll,12,3))
    drTmp# = drTmp + Val(Mid$(ll,16,2)) / 60.0
    drTmp# = drTmp + Val(Mid$(ll,19,2)) / 60.0 / 60.0
    drTmp# = drTmp * DEGRAD
      
    typ = Mid$(ll,11,1)
      
    If typ = "з" And drTmp < (90*DEGRAD) Then drTmp = - drTmp
    If typ = "з" And drTmp > (90*DEGRAD) Then drTmp = 360*DEGRAD - drTmp
      
    If typ = "W" And drTmp < (90*DEGRAD) Then drTmp = - drTmp
    If typ = "W" And drTmp > (90*DEGRAD) Then drTmp = 360*DEGRAD - drTmp
     
    If typ = "w" And drTmp < (90*DEGRAD) Then drTmp = - drTmp
    If typ = "w" And drTmp > (90*DEGRAD) Then drTmp = 360*DEGRAD - drTmp
      
    PrecLonToRad = drTmp
End Function

Sub XYtoSTR(drX As Double, drY As Double, MSSTR As String)
        Dim S1 As String, S2 As String 
   
   S1 = Format$(drX,"0.######") 
   Mid$(S1,InStr (S1,",",1)) = "."
   S2 = Format$(drY,"0.######") 
   Mid$(S2,InStr (S2,",",1)) = "."
   MSSTR = "XY=" + S1 + "," + S2
End Sub

Sub fnXY(drLatRad As Double, drLonRad As Double, drX As Double, drY As Double)
        Dim drDelta As Double, drRo As Double

        drDelta = fnDelta(drLonRad)
        drRo = fnRo(drLatRad)
        drX = drRo * Sin(drDelta)
        drY = RoS - drRo * Cos(drDelta)
End Sub

Function fnDelta(drLonRad As Double) As Double
        fnDelta = drLonRad * ALFA - DC
End Function
 
Function fnR(drLatRad As Double) As Double
        Dim drR As Double

        drR = fnN(drLatRad) * Cos(drLatRad)
        fnR = drR
End Function

Sub fnAlfaK(drLatRadN As Double,drLatRadS As Double,drScale As Double)
  Dim drLatRadM AS Double
  Dim drU1 AS Double, drU2 AS Double
  Dim drR1 AS Double, drR2 AS Double
  Dim drA AS Double
  
  drR1 = fnR(drLatRadN)
  drR2 = fnR(drLatRadS)
  drU1 = fnU(drLatRadN)
  drU2 = fnU(drLatRadS)

  drA = (log10(drR1) - log10(drR2))/(log10(drU2)-log10(drU1))
  K = (( 10.0 * drR1 ) / ( drScale * 1000.0 )) * ( pow(drU1,drA) / drA )
  ALFA = drA
End Sub
 
Function fnRo(drLatRad As Double) As Double
        Dim drRo As Double, drU  As Double

        drU = fnU(drLatRad)
        drRo = K / pow(drU,ALFA)
        fnRo = drRo
End Function 
 
Function fnU(drLatRad As Double) As Double
   Dim drPsi As Double, drTmp As Double, drU As Double, drE As Double

   drTmp = Sqr(E2) * Sin(drLatRad)
   drPsi = ASin(drTmp)
   drTmp = fnTg(drPsi)
   drE   = Sqr(E2)
   drU = fnTg(drLatRad) / pow(drTmp,drE)
   fnU = drU
End Function

Function ARINCtoRNCLat(CRD as String) As String
    Dim Lat As String
    Dim degree As integer
    Dim min As integer
    
    degree = Val(Mid$(CRD, 2, 2))
    min = Val(Mid$(CRD, 4, 2))

    If Cint(Val(Mid$(CRD, 6, 2)) / 6) = 10 Then
        min = min + 1
        
        'calc from minutes to degree'
            If min = 60 Then 
                Lat = CStr(degree + 1) + " 00.0"
            Else
                'calc from seconds to minutes'
                    If Len(CStr(min)) = 1 Then 
                        Lat = CStr(degree) + " 0" + CStr(min) + ".0"
                    Else
                        Lat = CStr(degree) + " " + CStr(min) + ".0"
                End If
            End If
        Else
                Lat = CStr(degree) + " " + CStr(min) + "." + CStr(Cint(Val(Mid$(CRD, 6, 2)) / 6))       
    End If 
    
    If Mid$(CRD, 1, 1) = "N" Then Lat = "с" + Lat
    If Mid$(CRD, 1, 1) = "S" Then Lat = "ю" + Lat
    
    ARINCtoRNCLat = Lat
End Function

Function ARINCtoRNCLon(CRD as String) As String
    Dim Lon As String, degree As integer, min As integer
    
    degree = Val(Mid$(CRD, 2, 3))
    min = Val(Mid$(CRD, 5, 2))
    Lon = Mid$(CRD, 2, 3) + " " + Mid$(CRD, 5, 2) + "." + CStr(Cint(Val(Mid$(CRD, 7, 2)) / 6))
    
    'calc from seconds to minutes'
    If Cint(Val(Mid$(CRD, 7, 2)) / 6) = 10 Then 
        min = min + 1

        If min = 60 Then 
                'calc from minutes to degree'
                Lon = CStr(degree + 1) + " 00.0"
        Else
                Lon = CStr(degree) + " " + CStr(min) + ".0"
                End If      
    End If
    
    If Mid$(CRD, 1, 1) = "E" Then Lon = "в" + Lon
    If Mid$(CRD, 1, 1) = "W" Then Lon = "з" + Lon 
   
    ARINCtoRNCLon = Lon  
End Function

Function log10(drN As Double) As Double
    log10 = Log(drN) / Log(10)
End Function

Function fnE2() As Double
        fnE2 = (AA * AA - BB * BB) / (AA * AA)
End Function
 
Function fnN(drLatRad As Double) As Double
        Dim drN As Double, drTmp As Double
 
        drTmp = 1.0 - E2 * Sin(drLatRad) * Sin(drLatRad)
        drN = AA / Sqr( drTmp )
        fnN = drN
End Function

Function ASin(drN As Double) As Double
    If drN = 1 Then
        ASin = PI / 2
    ElseIf drN = -1 Then
        ASin = -PI / 2   
    Else 
        ASin = Atn( drN / Sqr(1 - drN ^ 2) )
    End If 
End Function
 
Function fnTg(drAnglRad As Double) As Double
        Dim drTg As Double
 
        drTg = Tan(PI4 + drAnglRad / 2.0)
        fnTg = drTg
End Function 

Function pow(drX As Double, drY As Double) As Double
    pow = drX ^ CSng(drY)
End Function
 
Function LatToRad(ll as String) As Double
    Dim drTmp As Double, typ As String
      
    drTmp# = (Val(Mid$(ll, 2, 2)) + (Val(Mid$(ll, 5, 5)) / 60.0)) * DEGRAD
    typ = Mid$(ll, 1, 1)
    
    if typ = "S" Or typ = "s" Or typ = "Ю" Or typ = "ю" Then drTmp = -drTmp
    
    LatToRad = drTmp
End Function

Function LonToRad(ll as String) As Double
    Dim drTmp As Double, typ As String

    drTmp# = Val(Mid$(ll, 2, 3))
    
    if drTmp# = 0 then
        drTmp# = Val(Mid$(ll, 3, 3))
        drTmp# = drTmp + Val(Mid$(ll, 7, 5)) / 60.0
    else
        drTmp# = drTmp + Val(Mid$(ll, 6, 5)) / 60.0
    End If
    
    drTmp# = drTmp * DEGRAD
    typ = Mid$(ll, 1, 1)
      
    If typ = "з" And drTmp < (90*DEGRAD) Then drTmp = - drTmp
    If typ = "з" And drTmp > (90*DEGRAD) Then drTmp = 360*DEGRAD - drTmp
   
    If typ = "W" And drTmp < (90*DEGRAD) Then drTmp = - drTmp
    If typ = "W" And drTmp > (90*DEGRAD) Then drTmp = 360*DEGRAD - drTmp
      
    If typ = "w" And drTmp < (90*DEGRAD) Then drTmp = - drTmp
    If typ = "w" And drTmp > (90*DEGRAD) Then drTmp = 360*DEGRAD - drTmp
      
    LonToRad = drTmp
End Function
 
Sub ProcessElement(inElem as MbeElement)
    Dim gotNext as integer
    Do
        'call function to process individual element at current position'
        
        ProcessIndividualElem inElem
                If inElem.isHeader <> 0 Then
            ' if any components in complex, process them recursively'
                If inElem.nextComponent = MBE_Success Then
                        Call ProcessElement(inElem)
                End If
                gotNext = inElem.nextElement
                Else
                gotNext = inElem.nextComponent
                End If
    Loop While gotNext = MBE_Success
End Sub

Sub ProcessIndividualElem(inElem as MbeElement)
        Dim etext as string
        Dim res as integer
        Dim mdstring as string
        
        if inElem.type=MBE_Text then
                res=inElem.getString(etext)
                if Left$(etext,20)="Наименование проекта" then
                        'MbeMessageBox Right$(etext,4)'
                        prjName=Right$(etext,3)
                End If
          
                if Left$(etext,10)="Масштаб: 1" then
                mdstring= Mid$(etext,12)
                        'MbeMessageBox "SCALE:"+mdstring'
                        mapScale=Val(mdstring)
                End If
          
                if Left$(etext,25)="Стандартная параллель 1: " then
                        'MbeMessageBox "SP1 "+Right$(etext,9)'
                        drLat1c=Right$(etext,9)
                        drLat1c=Left$(drLat1c,8)
                End If
          
                if Left$(etext,25)="Стандартная параллель 2: " then
                        'MbeMessageBox "SP2 "+Right$(etext,9)'
                        drLat2c=Right$(etext,9)
                        'MbeMessageBox drLat2c'
                        'drLat2c=Left$(drLat2c,8)'
                End If
          
                if Left$(etext,19)="Система координат: " then
                        mdstring=Mid$(etext,19,Len(etext)-18)
                        'MbeMessageBox mdstring'
                        coordSys=mdstring
                End If
          
                if Left$(etext,14)="Угол наклона: " then
                        mdstring=Mid$(etext,14,Len(etext)-13)
                        'MbeMessageBox  "ANGLE:"+mdstring'
                        angleN=Val(mdstring)
                End If
                
                if Left$(etext,25)="Координаты центра листа: " then
                        mdstring=Mid$(etext,26,9)
                        drCLatc=mdstring
                        'MbeMessageBox "CENTER " + mdstring + " 1"'

                        mdstring=Right$(etext, 10)
                        drCLonc=mdstring
                        'MbeMessageBox  "CENTER "+ mdstring + " 1"'
                End If

                if Left$(etext,16)="Верхний правый: " then
                        drTopRight = Val(Mid$(etext,16,9))
                End If

                if Left$(etext,15)="Верхний левый: " then
                        drTopLeft = Val(Mid$(etext,15,9))
                End If

                if Left$(etext,15)="Нижний правый: " then
                        drBottomRight = Val(Mid$(etext,15,9))
                End If

                if Left$(etext,14)="Нижний левый: " then
                        drBottomLeft = Val(Mid$(etext,14,9))
                End If
    End If 

End Sub 

' Draw line
Sub PlaceGeoLine(lat() as String, lon() as String)
    Dim drLatt1 as Double, drLont1 as Double
    Dim drLatt2 as Double, drLont2 as Double
    Dim drX1 as Double, drY1 as Double, drX2 as Double, drY2 as Double

    ' Setting for line
    MbeSendKeyIn "AC=100;"
    MbeSendKeyIn "CO=21;"    ' 96 - black, 0 - red
    MbeSendKeyIn "ACTIVE LEVEL=1;"      ' 17
    MbeSendKeyIn "LV=1;"
    MbeSendKeyIn "LC=220;"
    MbeSendKeyIn "WT=4;"
    MbeSendKeyIn "PLACE LINE;"

    Dim i As Long

    For i = LBound(lat) To UBound(lat) - 2
        ' Conver to radian
        drLatt1=LatToRad(ArincToRncLat(lat(i)))
        drLont1=LonToRad(ArincToRncLon(lon(i)))
        drLatt2=LatToRad(ArincToRncLat(lat(i + 1)))
        drLont2=LonToRad(ArincToRncLon(lon(i + 1)))

        fnXY drLatt1,drLont1,drX1,drY1
        fnXY drLatt2,drLont2,drX2,drY2

        'Draw line
        MbeSendKeyIn "XY="+Str$(drX1)+","+Str$(drY1)+";"
        MbeSendKeyIn "XY="+Str$(drX2)+","+Str$(drY2)+";"

        MbeSendReset
    Next i

    For i = LBound(lat) To UBound(lat) - 2
        ' Conver to radian
        drLatt1=LatToRad(ArincToRncLat(lat(i)))
        drLont1=LonToRad(ArincToRncLon(lon(i)))

        fnXY drLatt1,drLont1,drX1,drY1

        ' Draw mark points
        MbeSendKeyIn "ACTIVE LEVEL=0;"
        MbeSendKeyIn "AC=MARK;"
        MbeSendKeyIn "PLACE CELL RELATIVE;"
        MbeSendKeyIn "XY="+Str$(0)+","+Str$(0)+";"

        MbeSendCommand "ACTIVE COLOR 101"
        MbeSendCommand "CHOOSE ELEMENT"
        MbeSendCommand "CHANGE COLOR "
        MbeSendKeyIn "XY="+Str$(0)+","+Str$(0)+";"
        MbeSendKeyIn "XY="+Str$(0)+","+Str$(0)+";"

        MbeSendKeyIn "MOVE" 
        MbeSendKeyIn "XY="+Str$(0)+","+Str$(0)+";"
        MbeSendKeyIn "XY="+Str$(drX1)+","+Str$(drY1)+";"
        MbeSendReset
    Next i
End Sub 

'Calculated center for area
Sub GetCenterOfPolygon(lat() As String, lon() As String, nameZoneCodeIcao As String, nameSector As String, callFuncFreq As String)
    'Dim x as Double, y as Double, z as Double
     Dim drLatt1 as Double, drLont1 as Double
    Dim drLatt2 as Double, drLont2 as Double
    Dim drX1 as Double, drY1 as Double, drX2 as Double, drY2 as Double
    'Dim lat1 as Double, lon1 as Double
    Dim i As Long
    Dim centerX as Double, centerY as Double, a as Double
    centerX = 0
    centerY = 0

    For i = LBound(lat) To UBound(lat) - 2
        'Conver to radian
        drLatt1=LatToRad(ArincToRncLat(lat(i)))
        drLont1=LonToRad(ArincToRncLon(lon(i)))
        drLatt2=LatToRad(ArincToRncLat(lat(i + 1)))
        drLont2=LonToRad(ArincToRncLon(lon(i + 1)))

        fnXY drLatt1,drLont1,drX1,drY1
        fnXY drLatt2,drLont2,drX2,drY2

        centerX = centerX + (drX1 + drX2) * (drX1 * drY2 - drX2 * drY1)
        centerY = centerY + (drY1 + drY2) * (drX1 * drY2 - drX2 * drY1)
        a = a + (drX1 * drY2 - drX2 * drY1)

        'lat1 = drLatt * PI / 180
        'lon1 = drLont * PI / 180
        'x = x + Cos(lat1) * Cos(lon1)
        'y = y + Cos(lat1) * Sin(lon1)
        'z = z + Sin(lat1)
    Next i

    centerX = (1 / (6 * (a / 2))) * centerX
    centerY = (1 / (6 * (a * 0.5))) * centerY

    'Dim lonCenter as Double
    'Dim latCenter as Double
    'Dim Hyp as Double

    'lonCenter = Atan2(y, x)
    'Hyp = Sqr(x * x + y * y)
    'latCenter = Atan2(z, Hyp)
    'latCenter = latCenter * 180/PI
    'lonCenter = lonCenter * 180/PI

    Dim maxLengthStr As Integer
    maxLengthStr = 0

    MbeSendKeyIn "CO=21;"
    MbeSendKeyIn "TH=2.0;"
    MbeSendKeyIn "TW=2.0;"
    MbeSendKeyIn "ACTIVE FONT=ft-32;"       
    MbeSendKeyIn "PLACE TEXT;"+UCase$(nameZoneCodeIcao)
    MbeSendKeyIn "XY="+Str$(centerX)+","+Str$(centerY)
    If Len(nameZoneCodeIcao) > maxLengthStr Then
        maxLengthStr = Len(nameZoneCodeIcao)
    End If
    
    MbeSendKeyIn "PLACE TEXT;"+UCase$(nameSector)
    MbeSendKeyIn "DX=,-3.0"
    If Len(nameSector) > maxLengthStr Then
        maxLengthStr = Len(nameSector)
    End If
    
    MbeSendKeyIn "ACTIVE FONT=ft-12;"     
    MbeSendKeyIn "PLACE TEXT;"+UCase$(callFuncFreq)
    MbeSendKeyIn "DX=,-3.0"
    If Len(callFuncFreq) > maxLengthStr Then
        maxLengthStr = Len(callFuncFreq)
    End If
    maxLengthStr = maxLengthStr * 1.5

    'Setting for line
    MbeSendKeyIn "AC=100;"
    MbeSendKeyIn "CO=21;"    ' 96 - black, 0 - red
    MbeSendKeyIn "ACTIVE LEVEL=1;"      ' 17
    MbeSendKeyIn "LV=1;"
    MbeSendKeyIn "LC=010;"
    MbeSendKeyIn "WT=2;"
    MbeSendKeyIn "PLACE LINE;"
    'Draw line
    MbeSendKeyIn "XY="+Str$(centerX - maxLengthStr / 2)+","+Str$(centerY + 3)+";"
    MbeSendKeyIn "XY="+Str$(centerX + maxLengthStr / 2)+","+Str$(centerY + 3)+";"
    MbeSendKeyIn "XY="+Str$(centerX + maxLengthStr / 2)+","+Str$(centerY - 9)+";"
    MbeSendKeyIn "XY="+Str$(centerX - maxLengthStr / 2)+","+Str$(centerY - 9)+";"
    MbeSendKeyIn "XY="+Str$(centerX - maxLengthStr / 2)+","+Str$(centerY + 3)+";"
End Sub

Function Atan2(y As Double, x As Double) As Double
    If x > 0 Then
        Atan2 = Atn(y / x)
    ElseIf x < 0 Then
        Atan2 = Sgn(y) * (Pi - Atn(Abs(y / x)))
    ElseIf y = 0 Then
        Atan2 = 0
    Else
        Atan2 = Sgn(y) * Pi / 2
    End If
End Function

Sub Main
    Dim drLat1 As Double, drLat2 As Double, res as Double
    dId = 1
    Dim element as New MbeElement
    Dim filePos as Long, elem_string as string, resel as long
    nLat1 = 1
    eLon1 = 1
    nLat2 = 1
    eLon2 = 1
        
    'Read the first element
    filePos = element.fromFile(0)
          
        Do While filePos >= 0
        if element.type=MBE_CellHeader then
            if element.cellName="MAPSET" then
                ProcessElement element
                End If
        End If
        
        filePos = element.fromFile(filePos + element.fileSize)
    Loop 
         
    prjNameD=prjName 
    mapScale = mapScale / 100000
    mapScaleD=Str$(mapScale)
    drLat1cD=drLat1c
    drLat2cD=drLat2c
    coordSysD=coordSys
    angleND=Str$(angleN)
    drCLatcD=drCLatc
    drCLoncD=drCLonc
    'ret=MbeOpenModalDialog(1)
        
    'All var page reading
    drLat1#=LatToRad(ArincToRNCLat(drLat1c))
    drLat2#=LatToRad(ArincToRNCLat(drLat2c))
    drCLat#=LatToRad(ARINCtoRNCLat(drCLatc))
    drCLon#=LonToRad(ARINCtoRNCLon(drCLonc))

    'Dim s$(1 to 3)
    'Dim scale As Integer
    's$(1) = "3"
    's$(2) = "5"
    's$(3) = "10"
    MbeMessageBox mapScaleD
    'scale = MbeSelectBox("Please enter a scale:", s$, "Enter a scale")
    'mapScale = Val(s$(scale))

    C = (AA-BB)/AA
    E2=fnE2
    ALFA = 1 
    K = 6356.78/2
        
    If drLat1c <> drLat2c then 
        fnAlfaK drLat1,drLat2,mapScale
    End if
 
    DC = fnDelta(drCLon)
    RoS  = fnRo(drCLat)
 
    MbeSendKeyIn "AS=1"
    MbeSendCommand "ATTACH LIBRARY C:\MAPCNTRL\PROJECTS\aeroflot\CELL\aeroflot.CEL"
        
    Open FileAdres for input as #1
        
    Dim coord1 As String
    Dim coord2 As String
    Dim nameZoneCodeIcao As String, nameSector As String, callFuncFreq As String
    Dim lat() as String, lon() as String
    Dim i as Integer

    do
        Line Input #1,nameZoneCodeIcao
        Line Input #1,nameSector
        Line Input #1,callFuncFreq

        i = 0
        do
            Line Input #1,coord1
            ' Check end file
            If coord1 = "1" Then Exit Do
            
            Line Input #1,coord2
            
            coord1=Trim$(coord1)
            coord2=Trim$(coord2)
            ReDim Preserve lat (i + 1)
            ReDim Preserve lon (i + 1)
            lat (i) = coord1
            lon (i) = coord2
            i = i + 1
        Loop while 1

        GetCenterOfPolygon lat, lon, nameZoneCodeIcao, nameSector, callFuncFreq
        PlaceGeoLine lat ,lon 
    Loop while not EOF(1)

    MbeSendCommand "NULL"              
End Sub
 