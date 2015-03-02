const int WM_PUBLIC         = 1;
const int WM_PRIVATE        = 2;

 // Number of crypted sections
const int  NUMBER_OF_CRYPTED_SECTIONS       = 16;

  // Crypt hash types, possible hash values for functions:
  // EP_CryptHashBuffer
  // EP_CryptHashStringA
  // EP_CryptHashStringW
  // EP_CryptHashFileA
  // EP_CryptHashFileW
const int HASH_XOR32        = 0;
const int HASH_MD2          = 1;
const int HASH_MD5          = 2;
const int HASH_RipeMD160    = 3;
const int HASH_SH1          = 4;
const int HASH_SHA224       = 5;
const int HASH_SHA256       = 6;
const int HASH_SHA384       = 7;
const int HASH_SHA512       = 8;

 // Return values of EP_RegLoadKey
const int LOADKEY_SUCCEEDED = 0;
const int LOADKEY_REGINFONOTFOUND = 1;
const int LOADKEY_NAMEBUFFERTOOSMALL = 2;
const int LOADKEY_KEYBUFFERTOOSMALL = 3;

 // Return values of EP_RegKeyStatus
const int KEY_STATUS_DOESNOTEXIST                         = 0;
const int KEY_STATUS_VALID                                = 1;
const int KEY_STATUS_INVALID                              = 2;
const int KEY_STATUS_STOLEN                               = 3;
const int KEY_STATUS_DATEEXPIRED                          = 4;
const int KEY_STATUS_WITHOUTHARDWARELOCK                  = 5;
const int KEY_STATUS_WITHOUTEXPIRATIONDATE                = 6;
const int KEY_STATUS_WITHOUTREGISTERAFTERDATE             = 7;
const int KEY_STATUS_WITHOUTREGISTERBEFOREDATE            = 8;
const int KEY_STATUS_WITHOUTEXECUTIONSLIMIT               = 9;
const int KEY_STATUS_WITHOUTDAYSLIMIT                     = 10;
const int KEY_STATUS_WITHOUTRUNTIMELIMIT                  = 11;
const int KEY_STATUS_WITHOUTGLOBALTIMELIMIT               = 12;
const int KEY_STATUS_WITHOUTCOUNTRYLOCK                   = 13;
const int KEY_STATUS_COUNTRYINVALID                       = 14;
const int KEY_STATUS_REGISTERAFTERFAILED                  = 15;
const int KEY_STATUS_REGISTERBEFOREFAILED                 = 16;
const int KEY_STATUS_EXECUTIONSEXPIRED                    = 17;
const int KEY_STATUS_DAYSEXPIRED                          = 18;
const int KEY_STATUS_RUNTIMEEXPIRED                       = 19;
const int KEY_STATUS_GLOBALTIMEEXPIRED                    = 20;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_VOLUMESERIAL = 21;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_VOLUMENAME   = 22;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_COMPUTERNAME = 23;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_CPU          = 24;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_MOTHERBOARD  = 25;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_WINDOWSKEY   = 26;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_HDDSERIAL    = 27;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_USERNAME     = 28;
const int KEY_STATUS_HARDWAREINVALID                      = 29;

  // Counties code
const int CN_AFGHANISTAN = 114;
const int CN_ALBANIA = 1;
const int CN_ALGERIA = 2;
const int CN_ARGENTINA = 3;
const int CN_ARMENIA = 4;
const int CN_AUSTRALIA = 5;
const int CN_AUSTRIA = 6;
const int CN_AZERBAIJAN = 7;
const int CN_BAHRAIN = 8;
const int CN_BANGLADESH = 115;
const int CN_BELARUS = 9;
const int CN_BELGIUM = 10;
const int CN_BELIZE = 11;
const int CN_BOLIVIA = 116;
const int CN_BOSNIA = 117;
const int CN_BRAZIL = 13;
const int CN_BRUNEI = 14;
const int CN_BULGARIA = 15;
const int CN_CAMBODIA = 16;
const int CN_CANADA = 17;
const int CN_CARRIBEAN = 118;
const int CN_CHILE = 20;
const int CN_CHINA = 21;
const int CN_COLOMBIA = 22;
const int CN_COSTARICA = 23;
const int CN_CROATIA = 24;
const int CN_CZECH = 25;
const int CN_DENMARK = 26;
const int CN_DOMINICAN = 27;
const int CN_ECUADOR = 28;
const int CN_EGYPT = 29;
const int CN_ELSALVADOR = 30;
const int CN_ESTONIA = 31;
const int CN_ETHIOPIA = 119;
const int CN_FAROE = 32;
const int CN_FINLAND = 33;
const int CN_FRANCE = 34;
const int CN_GEORGIA = 35;
const int CN_GERMANY = 36;
const int CN_GREECE = 37;
const int CN_GREENLAND = 120;
const int CN_GUATEMALA = 38;
const int CN_HONDURAS = 39;
const int CN_HONGKONG = 40;
const int CN_HUNGARU = 41;
const int CN_ICELAND = 42;
const int CN_INDIA = 43;
const int CN_INDONESIA = 44;
const int CN_IRAN = 45;
const int CN_IRAQ = 46;
const int CN_IRELAND = 47;
const int CN_ISRAEL = 48;
const int CN_ITALY = 49;
const int CN_JAMAICA = 50;
const int CN_JAPAN = 51;
const int CN_JORDAN = 52;
const int CN_KAZAKHSTAN = 53;
const int CN_KENYA = 54;
const int CN_KOREA = 56;
const int CN_KUWAIT = 57;
const int CN_KYRGYZSTAN = 58;
const int CN_LAOS = 121;
const int CN_LATVIA = 59;
const int CN_LEBANON = 60;
const int CN_LIBYAN = 122;
const int CN_LIECHTENSTEIN = 62;
const int CN_LITHUANIA = 63;
const int CN_LUXEMBOURG = 64;
const int CN_MACAO = 65;
const int CN_MACEDONIA = 66;
const int CN_MALAYSIA = 67;
const int CN_MALDIVES = 123;
const int CN_MALTA = 124;
const int CN_MEXOCI = 68;
const int CN_MONACO = 70;
const int CN_MONGOLIA = 71;
const int CN_MONTENEGRO = 125;
const int CN_MOROCCO = 72;
const int CN_NEPAL = 126;
const int CN_NETHERLANDS = 73;
const int CN_NEWZEALAND = 74;
const int CN_NICARAGUA = 75;
const int CN_NIGERIA = 127;
const int CN_NORWAY = 76;
const int CN_OMAN = 77;
const int CN_PAKISTAN = 78;
const int CN_PANAMA = 79;
const int CN_PARAGUAY = 80;
const int CN_PERY = 81;
const int CN_PHILIPPINES = 82;
const int CN_POLAND = 83;
const int CN_PORTUGAL = 84;
const int CN_PUERTORICO = 85;
const int CN_QATAR = 86;
const int CN_ROMANIA = 87;
const int CN_RUSSIA = 88;
const int CN_RWANDA = 128;
const int CN_SAUDIARABIA = 89;
const int CN_SENEGAL = 129;
const int CN_SERBIA = 130;
const int CN_SERBIAMONTENEGRO = 90;
const int CN_SINGAROPE = 91;
const int CN_SLOVAKIA = 92;
const int CN_SLOVENIA = 93;
const int CN_SOUTHAFRICA = 94;
const int CN_SPAIN = 95;
const int CN_SRILANKA = 131;
const int CN_SWEDEN = 96;
const int CN_SWITZERLAND = 97;
const int CN_SYRIAN = 132;
const int CN_TAIWAN = 98;
const int CN_TAJIKISTAN = 99;
const int CN_THAILAND = 100;
const int CN_TRINIDADTOBAGO = 101;
const int CN_TUNISIA = 102;
const int CN_TURKEY = 103;
const int CN_TURKMENISTAN = 133;
const int CN_UKRAINE = 104;
const int CN_UAE = 105;
const int CN_UNITEDKINGDOM = 106;
const int CN_USA = 107;
const int CN_URUGUAY = 108;
const int CN_UZBEKISTAN = 109;
const int CN_VENEZUELA = 110;
const int CN_VIETNAM = 111;
const int CN_YEMEN = 112;
const int CN_ZIMBABWE = 113;

// Return results of the functions EP_RegEncryptRegistrationInformation and
// EP_RegDecryptRegistrationInformation
const int REG_CRYPT_OK                       = 0;
const int REG_CRYPT_INVALID                  = 1;
const int REG_CRYPT_INSUFFICIENT_BUFFER      = 2;
const int REG_CRYPT_INSUFFICIENT_NAME_BUFFER = 3;
const int REG_CRYPT_INSUFFICIENT_KEY_BUFFER  = 4;

#pragma pack(push, 4)

typedef struct _TKeyCountries
{
  char* Name;
  int Code;
} TKeyCountries, *PKeyCountries;

const TKeyCountries KEY_COUNTRIES[127] =
{
  "Afghanistan"               , CN_AFGHANISTAN      ,
  "Albania"                   , CN_ALBANIA          ,
  "Algeria"                   , CN_ALGERIA          ,
  "Argentina"                 , CN_ARGENTINA        ,
  "Armenia"                   , CN_ARMENIA          ,
  "Australia"                 , CN_AUSTRALIA        ,
  "Austria"                   , CN_AUSTRIA          ,
  "Azerbaijan"                , CN_AZERBAIJAN       ,
  "Bahrain"                   , CN_BAHRAIN          ,
  "Bangladesh"                , CN_BANGLADESH       ,
  "Belarus"                   , CN_BELARUS          ,
  "Belgium"                   , CN_BELGIUM          ,
  "Belize"                    , CN_BELIZE           ,
  "Bolivia"                   , CN_BOLIVIA          ,
  "Bosnia and Herzegovina"    , CN_BOSNIA           ,
  "Brazil"                    , CN_BRAZIL           ,
  "Brunei Darussalam"         , CN_BRUNEI           ,
  "Bulgaria"                  , CN_BULGARIA         ,
  "Cambodia"                  , CN_CAMBODIA         ,
  "Canada"                    , CN_CANADA           ,
  "Caribbean"                 , CN_CARRIBEAN        ,
  "Chile"                     , CN_CHILE            ,
  "China"                     , CN_CHINA            ,
  "Colombia"                  , CN_COLOMBIA         ,
  "Costa Rica"                , CN_COSTARICA        ,
  "Croatia"                   , CN_CROATIA          ,
  "Czech Republic"            , CN_CZECH            ,
  "Denmark"                   , CN_DENMARK          ,
  "Dominican Republic"        , CN_DOMINICAN        ,
  "Ecuador"                   , CN_ECUADOR          ,
  "Egypt"                     , CN_EGYPT            ,
  "El Salvador"               , CN_ELSALVADOR       ,
  "Estonia"                   , CN_ESTONIA          ,
  "Ethiopia"                  , CN_ETHIOPIA         ,
  "Faroe Islands"             , CN_FAROE            ,
  "Finland"                   , CN_FINLAND          ,
  "France"                    , CN_FRANCE           ,
  "Georgia"                   , CN_GEORGIA          ,
  "Germany"                   , CN_GERMANY          ,
  "Greece"                    , CN_GREECE           ,
  "Greenland"                 , CN_GREENLAND        ,
  "Guatemala"                 , CN_GUATEMALA        ,
  "Honduras"                  , CN_HONDURAS         ,
  "Hong Kong"                 , CN_HONGKONG         ,
  "Hungary"                   , CN_HUNGARU          ,
  "Iceland"                   , CN_ICELAND          ,
  "India"                     , CN_INDIA            ,
  "Indonesia"                 , CN_INDONESIA        ,
  "Iran"                      , CN_IRAN             ,
  "Iraq"                      , CN_IRAQ             ,
  "Ireland"                   , CN_IRELAND          ,
  "Israel"                    , CN_ISRAEL           ,
  "Italy"                     , CN_ITALY            ,
  "Jamaica"                   , CN_JAMAICA          ,
  "Japan"                     , CN_JAPAN            ,
  "Jordan"                    , CN_JORDAN           ,
  "Kazakhstan"                , CN_KAZAKHSTAN       ,
  "Kenya"                     , CN_KENYA            ,
  "Korea"                     , CN_KOREA            ,
  "Kuwait"                    , CN_KUWAIT           ,
  "Kyrgyzstan"                , CN_KYRGYZSTAN       ,
  "Laos"                      , CN_LAOS             ,
  "Latvia"                    , CN_LATVIA           ,
  "Lebanon"                   , CN_LEBANON          ,
  "Libyan"                    , CN_LIBYAN           ,
  "Liechtenstein"             , CN_LIECHTENSTEIN    ,
  "Lithuania"                 , CN_LITHUANIA        ,
  "Luxembourg"                , CN_LUXEMBOURG       ,
  "Macao"                     , CN_MACAO            ,
  "Macedonia"                 , CN_MACEDONIA        ,
  "Malaysia"                  , CN_MALAYSIA         ,
  "Maldives"                  , CN_MALDIVES         ,
  "Malta"                     , CN_MALTA            ,
  "Mexico"                    , CN_MEXOCI           ,
  "Monaco"                    , CN_MONACO           ,
  "Mongolia"                  , CN_MONGOLIA         ,
  "Montenegro"                , CN_MONTENEGRO       ,
  "Morocco"                   , CN_MOROCCO          ,
  "Nepal"                     , CN_NEPAL            ,
  "Netherlands"               , CN_NETHERLANDS      ,
  "New Zealand"               , CN_NEWZEALAND       ,
  "Nicaragua"                 , CN_NICARAGUA        ,
  "Nigeria"                   , CN_NIGERIA          ,
  "Norway"                    , CN_NORWAY           ,
  "Oman"                      , CN_OMAN             ,
  "Pakistan"                  , CN_PAKISTAN         ,
  "Panama"                    , CN_PANAMA           ,
  "Paraguay"                  , CN_PARAGUAY         ,
  "Peru"                      , CN_PERY             ,
  "Philippines"               , CN_PHILIPPINES      ,
  "Poland"                    , CN_POLAND           ,
  "Portugal"                  , CN_PORTUGAL         ,
  "Puerto Rico"               , CN_PUERTORICO       ,
  "Qatar"                     , CN_QATAR            ,
  "Romania"                   , CN_ROMANIA          ,
  "Russia"                    , CN_RUSSIA           ,
  "Rwanda"                    , CN_RWANDA           ,
  "Saudi Arabia"              , CN_SAUDIARABIA      ,
  "Senegal"                   , CN_SENEGAL          ,
  "Serbia"                    , CN_SERBIA           ,
  "Serbia and Montenegro"     , CN_SERBIAMONTENEGRO ,
  "Singapore"                 , CN_SINGAROPE        ,
  "Slovakia"                  , CN_SLOVAKIA         ,
  "Slovenia"                  , CN_SLOVENIA         ,
  "South Africa"              , CN_SOUTHAFRICA      ,
  "Spain"                     , CN_SPAIN            ,
  "Sri Lanka"                 , CN_SRILANKA         ,
  "Sweden"                    , CN_SWEDEN           ,
  "Switzerland"               , CN_SWITZERLAND      ,
  "Syrian"                    , CN_SYRIAN           ,
  "Taiwan"                    , CN_TAIWAN           ,
  "Tajikistan"                , CN_TAJIKISTAN       ,
  "Thailand"                  , CN_THAILAND         ,
  "Trinidad and Tobago"       , CN_TRINIDADTOBAGO   ,
  "Tunisia"                   , CN_TUNISIA          ,
  "Turkey"                    , CN_TURKEY           ,
  "Turkmenistan"              , CN_TURKMENISTAN     ,
  "Ukraine"                   , CN_UKRAINE          ,
  "United Arab Emirates"      , CN_UAE              ,
  "United Kingdom"            , CN_UNITEDKINGDOM    ,
  "United States"             , CN_USA              ,
  "Uruguay"                   , CN_URUGUAY          ,
  "Uzbekistan"                , CN_UZBEKISTAN       ,
  "Venezuela"                 , CN_VENEZUELA        ,
  "Viet Nam"                  , CN_VIETNAM          ,
  "Yemen"                     , CN_YEMEN            ,
  "Zimbabwe"                  , CN_ZIMBABWE
};

typedef struct TWMContent {

    int   WMType;
    char* Name;
    int   NameLen;
    char* Text;
    int   TextLen;
    char* FileName;
    int   FileNameLen;
    char* AFile;
    int   AFileLen;

} TWMContent, *PWMContent;

typedef struct TKeyInformation {
	BOOL Stolen;               // {out} is key stolen
    DWORD CreationYear;        // {out} key creation year
    DWORD CreationMonth;       // {out} key creation month
    DWORD CreationDay;         // {out} key creation day
    BOOL UseKeyExpiration;     // {out} has key expiration date?
    DWORD ExpirationYear;      // {out} key expiration year
    DWORD ExpirationMonth;     // {out} key expiration month
    DWORD ExpirationDay;       // {out} key expiration day
    BOOL UseHardwareLocking;   // {out} hardware locked key
    BOOL UseExecutionsLimit;   // {out} limit key by executions?
    DWORD ExecutionsCount;     // {out} number of executions
    BOOL UseDaysLimit;         // {out} limit key by days?
    DWORD DaysCount;           // {out} number of days
    BOOL UseRunTimeLimit;      // {out} limit key by run time?
    DWORD RunTimeMinutes;      // {out} run time minutes
    BOOL UseGlobalTimeLimit;   // {out} limit key by global time?
    DWORD GlobalTimeMinutes;   // {out} global time minutes
    BOOL UseCountyLimit;       // {out} limit key by country?
    DWORD CountryCode;         // {out} country code
    BOOL UseRegisterAfter;     // {out} register key after date?
    DWORD RegisterAfterYear;   // {out} register after year
    DWORD RegisterAfterMonth;  // {out} register after month
    DWORD RegisterAfterDay;    // {out} register after day
    BOOL UseRegisterBefore;    // {out} register key before date?
    DWORD RegisterBeforeYear;  // {out} register before year
    DWORD RegisterBeforeMonth; // {out} register before month
    DWORD RegisterBeforeDay;   // {out} register before day
    BOOL EncryptedSections[NUMBER_OF_CRYPTED_SECTIONS]; // {out} Crypted sections
} TKeyInformation, *PKeyInformation;

#pragma pack(pop)

extern "C"
{
// Registration API
BOOL __declspec(dllimport) __stdcall EP_RegSaveKey( const char* Name, const char* Key );
BOOL __declspec(dllimport) __stdcall EP_RegSaveKeyA( const char* Name, const char* Key );
BOOL __declspec(dllimport) __stdcall EP_RegSaveKeyW( const wchar_t* Name, const wchar_t* Key );
BOOL __declspec(dllimport) __stdcall EP_RegLoadKey( char** Name, char** Key );
BOOL __declspec(dllimport) __stdcall EP_RegLoadKeyA( char** Name, char** Key );
BOOL __declspec(dllimport) __stdcall EP_RegLoadKeyW( wchar_t** Name, wchar_t** Key );
int __declspec(dllimport) __stdcall EP_RegLoadKeyEx( const char* Name, int* NameLen, const char* Key, int* KeyLen);
BOOL __declspec(dllimport) __stdcall EP_RegCheckKey( const char* Name, const char* Key );
BOOL __declspec(dllimport) __stdcall EP_RegCheckKeyA( const char* Name, const char* Key );
BOOL __declspec(dllimport) __stdcall EP_RegCheckKeyW( const wchar_t* Name, const wchar_t* Key );
BOOL __declspec(dllimport) __stdcall EP_RegDeleteKey();
BOOL __declspec(dllimport) __stdcall EP_RegLoadAndCheckKey();
BOOL __declspec(dllimport) __stdcall EP_RegCheckAndSaveKey( const char* Name, const char* Key );
BOOL __declspec(dllimport) __stdcall EP_RegCheckAndSaveKeyA( const char* Name, const char* Key );
BOOL __declspec(dllimport) __stdcall EP_RegCheckAndSaveKeyW( const wchar_t* Name, const wchar_t* Key );
LPCTSTR __declspec(dllimport) __stdcall EP_RegHardwareID();
LPCTSTR __declspec(dllimport) __stdcall EP_RegHardwareIDA();
LPCWSTR __declspec(dllimport) __stdcall EP_RegHardwareIDW();
BOOL __declspec(dllimport) __stdcall EP_RegKeyCreationDate( int* Year, int* Month, int* Day );
BOOL __declspec(dllimport) __stdcall EP_RegKeyExpirationDate( int* Year, int* Month, int* Day );
int __declspec(dllimport) __stdcall EP_RegKeyCreationDateEx();
int __declspec(dllimport) __stdcall EP_RegKeyExpirationDateEx();
BOOL __declspec(dllimport) __stdcall EP_RegKeyExecutions( int* Total, int* Left );
int __declspec(dllimport) __stdcall EP_RegKeyExecutionsTotal();
int __declspec(dllimport) __stdcall EP_RegKeyExecutionsLeft();
BOOL __declspec(dllimport) __stdcall EP_RegKeyDays( int* Total, int* Left );
int __declspec(dllimport) __stdcall EP_RegKeyDaysTotal();
int __declspec(dllimport) __stdcall EP_RegKeyDaysLeft();
BOOL __declspec(dllimport) __stdcall EP_RegKeyRuntime( int* Total, int* Left );
int __declspec(dllimport) __stdcall EP_RegKeyRuntimeTotal();
int __declspec(dllimport) __stdcall EP_RegKeyRuntimeLeft();
BOOL __declspec(dllimport) __stdcall EP_RegKeyGlobalTime( int* Total, int* Left );
int __declspec(dllimport) __stdcall EP_RegKeyGlobalTimeTotal();
int __declspec(dllimport) __stdcall EP_RegKeyGlobalTimeLeft();
BOOL __declspec(dllimport) __stdcall EP_RegKeyRegisterAfterDate( int* Year, int* Month, int* Day );
int __declspec(dllimport) __stdcall EP_RegKeyRegisterAfterDateEx();
BOOL __declspec(dllimport) __stdcall EP_RegKeyRegisterBeforeDate( int* Year, int* Month, int* Day );
int __declspec(dllimport) __stdcall EP_RegKeyRegisterBeforeDateEx();
BOOL __declspec(dllimport) __stdcall EP_RegKeyInformation(const char* AName, const char* AKey, PKeyInformation AKeyInfo);
BOOL __declspec(dllimport) __stdcall EP_RegKeyInformationA(const char* AName, const char* AKey, PKeyInformation AKeyInfo);
BOOL __declspec(dllimport) __stdcall EP_RegKeyInformationW(const wchar_t* AName, const wchar_t* AKey, PKeyInformation AKeyInfo);
int __declspec(dllimport) __stdcall EP_RegKeyStatus();
void __declspec(dllimport) __stdcall EP_RegShowDialog();
int __declspec(dllimport) __stdcall EP_RegEncryptRegistrationInformation(const byte* ABuffer, int* ABufferSize, const byte* ARegName, int ARegNameLen, const byte* ARegKey, int ARegKeyLen);
int __declspec(dllimport) __stdcall EP_RegDecryptRegistrationInformation(const byte* ABuffer, int ABufferSize, const byte* ARegName, int* ARegNameLen, const byte* ARegKey, int* ARegKeyLen);

// Enigma Online Activation API
void __declspec(dllimport) __stdcall EP_ActivationShowDialog();

// Trial API
BOOL __declspec(dllimport) __stdcall EP_TrialExecutions( int* Total, int* Left );
int __declspec(dllimport) __stdcall EP_TrialExecutionsTotal();
int __declspec(dllimport) __stdcall EP_TrialExecutionsLeft();
BOOL __declspec(dllimport) __stdcall EP_TrialDays( int* Total, int* Left );
int __declspec(dllimport) __stdcall EP_TrialDaysTotal();
int __declspec(dllimport) __stdcall EP_TrialDaysLeft();
BOOL __declspec(dllimport) __stdcall EP_TrialExpirationDate( int* Year, int* Month, int* Day );
int __declspec(dllimport) __stdcall EP_TrialExpirationDateEx();
BOOL __declspec(dllimport) __stdcall EP_TrialDateTillDate( int* StartYear, int* StartMonth, int* StartDay, int* EndYear, int* EndMonth, int* EndDay );
int __declspec(dllimport) __stdcall EP_TrialDateTillDateStartEx();
int __declspec(dllimport) __stdcall EP_TrialDateTillDateEndEx();
BOOL __declspec(dllimport) __stdcall EP_TrialExecutionTime( int* Total, int* Left );
int __declspec(dllimport) __stdcall EP_TrialExecutionTimeTotal();
int __declspec(dllimport) __stdcall EP_TrialExecutionTimeLeft();
int __declspec(dllimport) __stdcall EP_TrialClockReversedDays();

// Misc API
int __declspec(dllimport) __stdcall EP_MiscGetWatermark( int ID, PWMContent WM );
int __declspec(dllimport) __stdcall EP_MiscCountryCode();

// Protection API
int __declspec(dllimport) __stdcall EP_ProtectedStringByID( int ID, const char* Str, int Len);
int __declspec(dllimport) __stdcall EP_ProtectedStringByKey( const char* Key, const char* Str, int Len);

// Crypt API
int __declspec(dllimport) __stdcall EP_CryptHashBuffer( int Hash, const byte* Buffer, int Size, const byte* Digest);
int __declspec(dllimport) __stdcall EP_CryptHashFileA( int Hash, const char* FileName, const byte* Digest);
int __declspec(dllimport) __stdcall EP_CryptHashFileW( int Hash, const wchar_t* FileName, const byte* Digest);
int __declspec(dllimport) __stdcall EP_CryptHashStringA( int Hash, const char* Str, const byte* Digest);
int __declspec(dllimport) __stdcall EP_CryptHashStringW( int Hash, const wchar_t* Str, const byte* Digest);
void __declspec(dllimport) __stdcall EP_CryptEncryptBuffer( const byte* Buffer, int Size, const char* Key);
void __declspec(dllimport) __stdcall EP_CryptEncryptBufferEx( const byte* InBuffer, const byte* OutBuffer, int Size, const byte* Key, int KeySize);
void __declspec(dllimport) __stdcall EP_CryptDecryptBuffer( const byte* Buffer, int Size, const char* Key);
void __declspec(dllimport) __stdcall EP_CryptDecryptBufferEx( const byte* InBuffer, const byte* OutBuffer, int Size, const byte* Key, int KeySize);

// Checkup API
BOOL __declspec(dllimport) __stdcall EP_CheckupCopies( int* Total, int* Current);
int __declspec(dllimport) __stdcall EP_CheckupCopiesTotal();
int __declspec(dllimport) __stdcall EP_CheckupCopiesCurrent();
BOOL __declspec(dllimport) __stdcall EP_CheckupIsProtected();
BOOL __declspec(dllimport) __stdcall EP_CheckupIsEnigmaOk();
BOOL __declspec(dllimport) __stdcall EP_CheckupVirtualizationTools();

// Enigma API
int __declspec(dllimport) __stdcall EP_EnigmaVersion();

// Splash screen
int __declspec(dllimport) __stdcall EP_SplashScreenShow();
void __declspec(dllimport) __stdcall EP_SplashScreenHide();
}

