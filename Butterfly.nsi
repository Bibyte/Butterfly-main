
 
  !define MUI_PRODUCT "SIG Beta Ver. 1.0"
  !define MUI_FILE "savefile"
  !define MUI_VERSION ""
  !define MUI_BRANDINGTEXT "SIG Beta Ver. 1.0"
  CRCCheck On
 

  !include "${NSISDIR}\Contrib\Modern UI\System.nsh"
  !include WinVer.nsh
  !include x64.nsh
  !include StdUtils.nsh

Name "BiByte Butterfly"
RequestExecutionLevel admin

OutFile "Setups\Butterfly_Setup.exe"
SetCompressor /SOLID lzma

  !define MUI_ICON "Logo.ico"
  !define MUI_UNICON "Logo.ico"
  !define MUI_SPECIALBITMAP "Logo.png"

;------------------------------------------------------------------------
; Команда InstallDir определяет директорию для установки программы. Путь
; записывается в переменную $INSTDIR. Сюда будем устанавливать нашу 
; программу. Переменная $PROGRAMFILES определяет 
; путь к каталогу Program files
InstallDir "$PROGRAMFILES\BiByte\Butterfly"

;--------------------------------
;Modern UI Configuration
 
  !define MUI_WELCOMEPAGE  
  !define MUI_LICENSEPAGE
  !define MUI_DIRECTORYPAGE
  !define MUI_ABORTWARNING
  !define MUI_UNINSTALLER
  !define MUI_UNCONFIRMPAGE
  !define MUI_FINISHPAGE  
 
 

;--------------------------------
;Language
 
  !insertmacro MUI_LANGUAGE "English"
 
 
;-------------------------------- 
;Modern UI System
 
  ;!insertmacro MUI_SYSTEM 
 


;--------------------------------
;Data
 
  ;LicenseData "Lees_mij.txt"



;------------------------------------------------------------------------
; Проверка ключа реестра для директории, в которую будет устанавливаться
; программа (при повторной установке старый ключ будет переписан 
; автоматически). mp - каталог, куда устанавливается программа

InstallDirRegKey HKLM SOFTWARE\BiByte\Butterfly\Uninst "Install_Dir"


SilentInstall normal


;------------------------------------------------------------------------
; Этот ОБЯЗАТЕЛЬНЫЙ раздел описывает материал для установки. WebProtectorPlus - 
; имя проекта, определенное командой Name.
Section "Butterfly (required)"
SetShellVarContext all
;------------------------------------------------------------------------


  
  SetOutPath $INSTDIR
 
  File "D:\Projects\QT\Butterfly\Export_files\Butterfly.exe"
  File "D:\Projects\QT\Butterfly\Export_files\Clear_Autostart.reg"
  File "D:\Projects\QT\Butterfly\Export_files\icudt53.dll"
  File "D:\Projects\QT\Butterfly\Export_files\icuin53.dll"
  File "D:\Projects\QT\Butterfly\Export_files\icuuc53.dll"
  File "D:\Projects\QT\Butterfly\Export_files\libeay32.dll"
  File "D:\Projects\QT\Butterfly\Export_files\libgcc_s_dw2-1.dll"
  File "D:\Projects\QT\Butterfly\Export_files\libstdc++-6.dll"
  File "D:\Projects\QT\Butterfly\Export_files\libwinpthread-1.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5Core.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5Gui.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5Multimedia.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5MultimediaWidgets.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5Network.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5OpenGL.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5Positioning.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5PrintSupport.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5Qml.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5Quick.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5Sensors.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5Sql.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5WebChannel.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5WebKit.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5WebKitWidgets.dll"
  File "D:\Projects\QT\Butterfly\Export_files\Qt5Widgets.dll"
  File "D:\Projects\QT\Butterfly\Export_files\ssleay32.dll"

  SetOutPath $INSTDIR\filter-profiles
        
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\Bitrix_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\CNCat_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\DLE_no_porno_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\DLE_searsh_profile_Butterfly_2_0.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\DLE_searsh_profile_Butterfly_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\Drupal_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\IPB_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\Joomla_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\Maxsite_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\No_otbor_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\phpBB_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\phpBB3_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\Torrent_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\UCOZ_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\vBulletin_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\filter-profiles\WordPress_original.ini"

  SetOutPath $INSTDIR\imageformats

  File "D:\Projects\QT\Butterfly\Export_files\imageformats\qgif4.dll"
  File "D:\Projects\QT\Butterfly\Export_files\imageformats\qico4.dll"
  File "D:\Projects\QT\Butterfly\Export_files\imageformats\qjpeg4.dll"
  
  SetOutPath $INSTDIR\lang
  
  File "D:\Projects\QT\Butterfly\Export_files\lang\ru.lng"
  
  SetOutPath $INSTDIR\plugins\platforms
  
  File "D:\Projects\QT\Butterfly\Export_files\plugins\platforms\qminimal.dll"
  File "D:\Projects\QT\Butterfly\Export_files\plugins\platforms\qwindows.dll"
  
  SetOutPath $INSTDIR\search-profiles
  
  File "D:\Projects\QT\Butterfly\Export_files\search-profiles\Ask_Searsh_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\search-profiles\Bing_searsh_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\search-profiles\Dmoz_Catalog_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\search-profiles\Google_searsh_EN_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\search-profiles\Google_searsh_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\search-profiles\Google_searsh_RU_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\search-profiles\Google_searsh_USA_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\search-profiles\Nigma_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\search-profiles\NoNaMe_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\search-profiles\Rambler_Search_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\search-profiles\Rambler_TOP100_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\search-profiles\Refer_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\search-profiles\Yandex_catalog_original.ini"
  File "D:\Projects\QT\Butterfly\Export_files\search-profiles\Yandex_searsh_original.ini"
 
  WriteRegStr HKLM SOFTWARE\BiByte\Butterfly\Uninst "Install_Dir" "$INSTDIR"
;------------------------------------------------------------------------
; Записать ключи деинсталляции для Windows. WebProtectorPlus - имя проекта, 
; определенное командой Name. Слова "только удаление" необязательны. Эта
; информация отображается в списке установленных программ, когда вы 
; выбираете Панель управления -> Установка и удаление программ
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Butterfly" "DisplayName" "Butterfly (uninstall only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Butterfly" "UninstallString" '"$INSTDIR\uninstall.exe"'
;------------------------------------------------------------------------
  WriteUninstaller "uninstall.exe"
  ; Прописывает деинсталлятор (и дополнительно - путь) для определенного
  ; командой File файла.
;------------------------------------------------------------------------


  
SectionEnd
; Эта команда закрывает текущий открытый раздел
;------------------------------------------------------------------------
; Раздел опций (могут быть отключены пользователем). Создаем ярлыки
Section "Start Menu Shortcuts"
SetShellVarContext all
  ; Команда CreateDirectory создает каталог по указанному пути.
  ; Переменная $SMPROGRAMS определяет путь к пункту меню Пуск -> 
  ; Программы, т.е. мы создаем каталог WebProtectorPlus в меню Пуск -> Программы
  CreateDirectory "$SMPROGRAMS\BiByte\Butterfly"

  ; В этом каталоге создаем ярлыки с помощью команды CreateShortCut:
  
  ; Ярлык WebProtectorPlus - программа
  CreateShortCut "$SMPROGRAMS\BiByte\Butterfly\Butterfly.lnk" "$INSTDIR\Butterfly.exe" "" "$INSTDIR\Butterfly.exe" 0
  
  ; Ярлык Uninstall - деинсталляция
  CreateShortCut "$SMPROGRAMS\BiByte\Butterfly\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0



SectionEnd
;------------------------------------------------------------------------
; Деинсталлятор. Эта информация выводится при удалении программы
UninstallText "The Butterfly will be removed from your computer. Click Uninstall, to continue." "Uninstall from:"
;------------------------------------------------------------------------
; Раздел деинсталляции
Section "Uninstall"
SetShellVarContext all

Var /GLOBAL bStarted
StrCpy $bStarted 0

FindProcDLL::FindProc "butterfly.exe"
IntCmp $R0 1 0 nostarted
    StrCpy $bStarted 1
	MessageBox MB_OK "Can't uninstall now! The Butterfly is busy. Please try again later!"
    Quit
nostarted:



;Delete Files 
  RMDir /r "$INSTDIR\*.*"    
 
;Remove the installation directory
  RMDir "$INSTDIR"


  Delete "$SMPROGRAMS\BiByte\*.*"
  RMDir "$SMPROGRAMS\BiByte"       ; удалить каталог WebProtectorPlus из меню Программы
 
  RMDir /r "$PROGRAMFILES\BiByte"
  ; /r - с этим параметром каталог удаляется, даже если он НЕ пустой.
  
    ; Удалить ключи из реестра
  DeleteRegKey HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Butterfly"

  DeleteRegKey HKEY_CURRENT_USER "Software\BiByte\Butterfly"
  DeleteRegKey HKEY_CURRENT_USER "Software\BiByte"
 
SectionEnd

;--------------------------------    
;MessageBox Section
 
 
;Function that calls a messagebox when installation finished correctly
;Function .onInstSuccess
;  MessageBox MB_OK "You have successfully installed Web Protector Plus. Use the desktop icon to start the program."
;FunctionEnd
 
 
;Function un.onUninstSuccess
;  MessageBox MB_OK "You have successfully uninstalled Web Protector Plus."
;FunctionEnd
 
 
;eof



; Отмучились - исходный файл готов.