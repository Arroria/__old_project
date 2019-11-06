#include "stdafx.h"
#include "MainLoop.h"


enum class FT_WEIGHT
{
	DONTCARE    = FW_DONTCARE,
	THIN        = FW_THIN,
	EXTRALIGHT  = FW_EXTRALIGHT,
	LIGHT       = FW_LIGHT,
	NORMAL      = FW_NORMAL,

	MEDIUM      = FW_MEDIUM,
	SEMIBOLD    = FW_SEMIBOLD,
	BOLD        = FW_BOLD,
	EXTRABOLD   = FW_EXTRABOLD,
	HEAVY       = FW_HEAVY,

	MAX = 10
};
enum class FT_ITALIC
{
	True,
	False,

	MAX = 2
};
enum class FT_CHARSET
{
	ANSI		= ANSI_CHARSET		,
	DEFAULT		= DEFAULT_CHARSET		,
	SYMBOL		= SYMBOL_CHARSET		,
	SHIFTJIS	= SHIFTJIS_CHARSET	,
	HANGEUL		= HANGEUL_CHARSET		,

	GB2312		= GB2312_CHARSET		,
	CHINESEBIG5	= CHINESEBIG5_CHARSET	,
	OEM			= OEM_CHARSET			,
	JOHAB		= JOHAB_CHARSET,
	HEBREW		= HEBREW_CHARSET,

	ARABIC		= ARABIC_CHARSET,
	GREEK		= GREEK_CHARSET,
	TURKISH		= TURKISH_CHARSET,
	VIETNAMESE	= VIETNAMESE_CHARSET,
	THAI		= THAI_CHARSET,
	
	EASTEUROPE	= EASTEUROPE_CHARSET,
	RUSSIAN		= RUSSIAN_CHARSET,
	MAC			= MAC_CHARSET,
	BALTIC		= BALTIC_CHARSET,

	MAX = 19
};
enum class FT_OUTPUTPRECISION
{
	DEFAULT_PRECIS           = OUT_DEFAULT_PRECIS			,
	STRING_PRECIS            = OUT_STRING_PRECIS			,
	CHARACTER_PRECIS         = OUT_CHARACTER_PRECIS		,
	STROKE_PRECIS            = OUT_STROKE_PRECIS			,
	TT_PRECIS                = OUT_TT_PRECIS				,

	DEVICE_PRECIS            = OUT_DEVICE_PRECIS			,
	RASTER_PRECIS            = OUT_RASTER_PRECIS			,
	TT_ONLY_PRECIS           = OUT_TT_ONLY_PRECIS			,
	OUTLINE_PRECIS           = OUT_OUTLINE_PRECIS			,
	SCREEN_OUTLINE_PRECIS    = OUT_SCREEN_OUTLINE_PRECIS	,

	PS_ONLY_PRECIS           = OUT_PS_ONLY_PRECIS			,

	MAX = 11
};
enum class FT_QUALITY
{
	DEFAULT				= DEFAULT_QUALITY,
	DRAFT				= DRAFT_QUALITY,
	PROOF				= PROOF_QUALITY,
	NONANTIALIASED		= NONANTIALIASED_QUALITY,
	ANTIALIASED			= ANTIALIASED_QUALITY,

	CLEARTYPE			= CLEARTYPE_QUALITY,
	CLEARTYPE_NATURAL	= CLEARTYPE_NATURAL_QUALITY,
	
	MAX = 7
};
enum class FT_PITCH
{
	DEFAULT		= DEFAULT_PITCH,
	FIXED		= FIXED_PITCH,
	VARIABLE	= VARIABLE_PITCH,
	MONO_FONT_	= MONO_FONT,
	
	MAX = 4
};
enum class FT_FAMILY
{
	DONTCARE	= FF_DONTCARE,
	ROMAN		= FF_ROMAN,
	SWISS		= FF_SWISS,
	MODERN		= FF_MODERN,
	SCRIPT		= FF_SCRIPT,
	
	DECORATIVE	= FF_DECORATIVE,

	MAX = 6
};

///LPD3DXFONT g_fontList[(unsigned int)FT_WEIGHT::MAX][(unsigned int)FT_ITALIC::MAX][(unsigned int)FT_CHARSET::MAX][(unsigned int)FT_OUTPUTPRECISION::MAX][(unsigned int)FT_QUALITY::MAX][(unsigned int)FT_PITCH::MAX][(unsigned int)FT_FAMILY::MAX] = { NULL };


LPD3DXFONT yo;
LPD3DXSPRITE sp;
LPD3DXFONT pFont = NULL;
LPD3DXFONT pFontItalic = NULL;
LPD3DXFONT pFontBold = NULL;
LPD3DXFONT pFontHeavy = NULL;
LPD3DXFONT pFontDraftQuality = NULL;
LPD3DXFONT pFontProofQuality = NULL;
bool MainLoop::Initialize()
{
	if (AllocConsole())
	{
		#pragma warning(push)
		#pragma warning(disable: 4996)
		_wfreopen(L"CONIN$", L"rb", stdin);
		_wfreopen(L"CONOUT$", L"wb", stdout);
		_wfreopen(L"CONOUT$", L"wb", stderr);
		#pragma warning(pop)
	}


	/*
	//Font Create Hell
	{
		#define FONT_NAME L"Bitstream Vera Sans Mono"
		#define FONT_SIZE -20

		//Font Weight
		for (int count_fontWeight = 0; count_fontWeight < (int)FT_WEIGHT::MAX; count_fontWeight++)
		{
			UINT fontWeight = NULL;
			switch (count_fontWeight)
			{
				case 0: fontWeight = (UINT)FT_WEIGHT::DONTCARE	;break; 
				case 1: fontWeight = (UINT)FT_WEIGHT::THIN		;break; 
				case 2: fontWeight = (UINT)FT_WEIGHT::EXTRALIGHT;break; 
				case 3: fontWeight = (UINT)FT_WEIGHT::LIGHT		;break; 
				case 4: fontWeight = (UINT)FT_WEIGHT::NORMAL	;break; 
				case 5: fontWeight = (UINT)FT_WEIGHT::MEDIUM	;break; 
				case 6: fontWeight = (UINT)FT_WEIGHT::SEMIBOLD	;break; 
				case 7: fontWeight = (UINT)FT_WEIGHT::BOLD		;break; 
				case 8: fontWeight = (UINT)FT_WEIGHT::EXTRABOLD	;break; 
				case 9: fontWeight = (UINT)FT_WEIGHT::HEAVY		;break;
			}

			//Font Italic
			for (int count_fontItalic = 0; count_fontItalic < (int)FT_ITALIC::MAX; count_fontItalic++)
			{
				bool fontItalic = NULL;
				switch (count_fontItalic)
				{
				case 0: fontItalic = (bool)FT_ITALIC::True; break;
				case 1: fontItalic = (bool)FT_ITALIC::False; break;
				}

				//Font Charset
				for (int count_fontCharSet = 0; count_fontCharSet < (int)FT_CHARSET::MAX; count_fontCharSet++)
				{
					DWORD fontCharSet = NULL;
					switch (count_fontCharSet)
					{
					case 0:		fontCharSet = (DWORD)FT_CHARSET::ANSI		; break;
					case 1:		fontCharSet = (DWORD)FT_CHARSET::DEFAULT	; break;
					case 2:		fontCharSet = (DWORD)FT_CHARSET::SYMBOL		; break;
					case 3:		fontCharSet = (DWORD)FT_CHARSET::SHIFTJIS	; break;
					case 4:		fontCharSet = (DWORD)FT_CHARSET::HANGEUL	; break;

					case 5:		fontCharSet = (DWORD)FT_CHARSET::GB2312		; break;
					case 6:		fontCharSet = (DWORD)FT_CHARSET::CHINESEBIG5; break;
					case 7:		fontCharSet = (DWORD)FT_CHARSET::OEM		; break;
					case 8:		fontCharSet = (DWORD)FT_CHARSET::JOHAB		; break;
					case 9:		fontCharSet = (DWORD)FT_CHARSET::HEBREW		; break;

					case 10:	fontCharSet = (DWORD)FT_CHARSET::ARABIC		; break;
					case 11:	fontCharSet = (DWORD)FT_CHARSET::GREEK		; break;
					case 12:	fontCharSet = (DWORD)FT_CHARSET::TURKISH	; break;
					case 13:	fontCharSet = (DWORD)FT_CHARSET::VIETNAMESE	; break;
					case 14:	fontCharSet = (DWORD)FT_CHARSET::THAI		; break;

					case 15:	fontCharSet = (DWORD)FT_CHARSET::EASTEUROPE	; break;
					case 16:	fontCharSet = (DWORD)FT_CHARSET::RUSSIAN	; break;
					case 17:	fontCharSet = (DWORD)FT_CHARSET::MAC		; break;
					case 18:	fontCharSet = (DWORD)FT_CHARSET::BALTIC		; break;
					}

					//Font OutputPercision
					for (int count_fontOutputPercision = 0; count_fontOutputPercision < (int)FT_OUTPUTPRECISION::MAX; count_fontOutputPercision++)
					{
						DWORD fontOutputPercision = NULL;
						switch (count_fontOutputPercision)
						{
						case 0:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::DEFAULT_PRECIS       ; break;
						case 1:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::STRING_PRECIS        ; break;
						case 2:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::CHARACTER_PRECIS     ; break;
						case 3:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::STROKE_PRECIS        ; break;
						case 4:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::TT_PRECIS            ; break;

						case 5:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::DEVICE_PRECIS        ; break;
						case 6:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::RASTER_PRECIS        ; break;
						case 7:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::TT_ONLY_PRECIS       ; break;
						case 8:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::OUTLINE_PRECIS       ; break;
						case 9:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::SCREEN_OUTLINE_PRECIS; break;

						case 10:	fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::PS_ONLY_PRECIS       ; break;
						}

						//Font Quality
						for (int count_fontQuality = 0; count_fontQuality < (int)FT_QUALITY::MAX; count_fontQuality++)
						{
							DWORD fontQuality = NULL;
							switch (count_fontQuality)
							{
							case 0:		fontQuality = (DWORD)FT_QUALITY::DEFAULT; break;
							case 1:		fontQuality = (DWORD)FT_QUALITY::DRAFT; break;
							case 2:		fontQuality = (DWORD)FT_QUALITY::PROOF; break;
							case 3:		fontQuality = (DWORD)FT_QUALITY::NONANTIALIASED; break;
							case 4:		fontQuality = (DWORD)FT_QUALITY::ANTIALIASED; break;

							case 5:		fontQuality = (DWORD)FT_QUALITY::CLEARTYPE; break;
							case 6:		fontQuality = (DWORD)FT_QUALITY::CLEARTYPE_NATURAL; break;
							}

							//Font Pitch
							for (int count_fontPitch = 0; count_fontPitch < (int)FT_PITCH::MAX; count_fontPitch++)
							{
								DWORD fontPitch = NULL;
								switch (count_fontPitch)
								{
								case 0:		fontPitch = (DWORD)FT_PITCH::DEFAULT	; break;
								case 1:		fontPitch = (DWORD)FT_PITCH::FIXED		; break;
								case 2:		fontPitch = (DWORD)FT_PITCH::VARIABLE	; break;
								case 3:		fontPitch = (DWORD)FT_PITCH::MONO_FONT_	; break;
								}

								//Font Family
								for (int count_fontFamily = 0; count_fontFamily < (int)FT_FAMILY::MAX; count_fontFamily++)
								{
									DWORD fontFamily = NULL;
									switch (count_fontFamily)
									{
									case 0:		fontFamily = (DWORD)FT_FAMILY::DONTCARE		; break;
									case 1:		fontFamily = (DWORD)FT_FAMILY::ROMAN		; break;
									case 2:		fontFamily = (DWORD)FT_FAMILY::SWISS		; break;
									case 3:		fontFamily = (DWORD)FT_FAMILY::MODERN		; break;
									case 4:		fontFamily = (DWORD)FT_FAMILY::SCRIPT		; break;
									
									case 5:		fontFamily = (DWORD)FT_FAMILY::DECORATIVE	; break;
									}

									//Let's make
									HRESULT result = D3DXCreateFontW(g_processManager->GetDevice(), -20, 0, fontWeight, 1, fontItalic, fontCharSet, fontOutputPercision, fontQuality, fontPitch | fontFamily, FONT_NAME, &g_fontList[count_fontWeight][count_fontItalic][count_fontCharSet][count_fontOutputPercision][count_fontQuality][count_fontPitch][count_fontFamily]);
									if (FAILED( result ))// && result != E_OUTOFMEMORY)
										cout << "폰트생성 오류발생." << "\nFontHeight : " << count_fontWeight << "\nFontItalic : " << count_fontItalic << "\nFontCharSet : " << count_fontCharSet << "\nFontOutputPercision : " << count_fontOutputPercision << "\nFontQuality : " << count_fontQuality << "\nFontPith : " << count_fontPitch << "\nFontFamily : " << count_fontFamily << endl;

								}
							}
						}
					}
				}
			}
		}
	}
	*/

	D3DXCreateSprite(g_processManager->GetDevice(), &sp);
	return false;
}


LPD3DXFONT g_font = nullptr;
std::wstring g_output;
int g_selete = 0;

void MainLoop::Update(double deltaTime)
{
	GetSingleton(InputManager)->Begin();

	//Font Create Hell
	{
#define FONT_NAME L"태 나무"
		//L"Bitstream Vera Sans Mono"
#define FONT_SIZE -20

		static int count_fontWeight				= 0;
		static int count_fontItalic				= 0;
		static int count_fontCharSet			= 0;
		static int count_fontOutputPercision	= 0;
		static int count_fontQuality			= 0;
		static int count_fontPitch				= 0;
		static int count_fontFamily				= 0;

		if (GetSingleton(InputManager)->IsKeyPressed('1'))	g_selete = 1;
		if (GetSingleton(InputManager)->IsKeyPressed('2'))	g_selete = 2;
		if (GetSingleton(InputManager)->IsKeyPressed('3'))	g_selete = 3;
		if (GetSingleton(InputManager)->IsKeyPressed('4'))	g_selete = 4;
		if (GetSingleton(InputManager)->IsKeyPressed('5'))	g_selete = 5;
		if (GetSingleton(InputManager)->IsKeyPressed('6'))	g_selete = 6;
		if (GetSingleton(InputManager)->IsKeyPressed('7'))	g_selete = 7;
		
		switch (g_selete)
		{
		case 1:	{ if (GetSingleton(InputManager)->IsKeyDown(VK_LEFT))	(count_fontWeight			)--;	if (GetSingleton(InputManager)->IsKeyDown(VK_RIGHT))	(count_fontWeight			)++;	if (count_fontWeight			>= (int)FT_WEIGHT			::MAX)	count_fontWeight			= 0;	if (count_fontWeight			< 0)	count_fontWeight			= (int)FT_WEIGHT			::MAX - 1; }	break;
		case 2:	{ if (GetSingleton(InputManager)->IsKeyDown(VK_LEFT))	(count_fontItalic			)--;	if (GetSingleton(InputManager)->IsKeyDown(VK_RIGHT))	(count_fontItalic			)++;	if (count_fontItalic			>= (int)FT_ITALIC			::MAX)	count_fontItalic			= 0;	if (count_fontItalic			< 0)	count_fontItalic			= (int)FT_ITALIC			::MAX - 1; }	break;
		case 3:	{ if (GetSingleton(InputManager)->IsKeyDown(VK_LEFT))	(count_fontCharSet			)--;	if (GetSingleton(InputManager)->IsKeyDown(VK_RIGHT))	(count_fontCharSet			)++;	if (count_fontCharSet			>= (int)FT_CHARSET			::MAX)	count_fontCharSet			= 0;	if (count_fontCharSet			< 0)	count_fontCharSet			= (int)FT_CHARSET			::MAX - 1; }	break;
		case 4:	{ if (GetSingleton(InputManager)->IsKeyDown(VK_LEFT))	(count_fontOutputPercision	)--;	if (GetSingleton(InputManager)->IsKeyDown(VK_RIGHT))	(count_fontOutputPercision	)++;	if (count_fontOutputPercision	>= (int)FT_OUTPUTPRECISION	::MAX)	count_fontOutputPercision	= 0;	if (count_fontOutputPercision	< 0)	count_fontOutputPercision	= (int)FT_OUTPUTPRECISION	::MAX - 1; }	break;
		case 5:	{ if (GetSingleton(InputManager)->IsKeyDown(VK_LEFT))	(count_fontQuality			)--;	if (GetSingleton(InputManager)->IsKeyDown(VK_RIGHT))	(count_fontQuality			)++;	if (count_fontQuality			>= (int)FT_QUALITY			::MAX)	count_fontQuality			= 0;	if (count_fontQuality			< 0)	count_fontQuality			= (int)FT_QUALITY			::MAX - 1; }	break;
		case 6:	{ if (GetSingleton(InputManager)->IsKeyDown(VK_LEFT))	(count_fontPitch			)--;	if (GetSingleton(InputManager)->IsKeyDown(VK_RIGHT))	(count_fontPitch			)++;	if (count_fontPitch				>= (int)FT_PITCH			::MAX)	count_fontPitch				= 0;	if (count_fontPitch				< 0)	count_fontPitch				= (int)FT_PITCH				::MAX - 1; }	break;
		case 7:	{ if (GetSingleton(InputManager)->IsKeyDown(VK_LEFT))	(count_fontFamily			)--;	if (GetSingleton(InputManager)->IsKeyDown(VK_RIGHT))	(count_fontFamily			)++;	if (count_fontFamily			>= (int)FT_FAMILY			::MAX)	count_fontFamily			= 0;	if (count_fontFamily			< 0)	count_fontFamily			= (int)FT_FAMILY			::MAX - 1; }	break;
		}


		UINT fontWeight = NULL;
		bool fontItalic = NULL;
		DWORD fontCharSet = NULL;
		DWORD fontOutputPercision = NULL;
		DWORD fontQuality = NULL;
		DWORD fontPitch = NULL;
		DWORD fontFamily = NULL;


		g_output = L"[Font]";
		wchar_t tempBuffer[1000] = { NULL };
		#define NoNamedDefine(left, right)	((g_output += L"\n") += _itow(left + 1, tempBuffer, 10)) += L" / "; g_output += _itow((int)right, tempBuffer, 10)

		//Font Weight
		NoNamedDefine(count_fontWeight, (int)FT_WEIGHT::MAX);
		g_output += L" Weight_";
		switch (count_fontWeight)
		{
		case 0: fontWeight = (UINT)FT_WEIGHT::DONTCARE;		g_output += L"DontCare";	break;
		case 1: fontWeight = (UINT)FT_WEIGHT::THIN;			g_output += L"Thin";		break;
		case 2: fontWeight = (UINT)FT_WEIGHT::EXTRALIGHT;	g_output += L"ExtraLight";	break;
		case 3: fontWeight = (UINT)FT_WEIGHT::LIGHT;		g_output += L"Light";		break;
		case 4: fontWeight = (UINT)FT_WEIGHT::NORMAL;		g_output += L"Normal";		break;
		case 5: fontWeight = (UINT)FT_WEIGHT::MEDIUM;		g_output += L"Medium";		break;
		case 6: fontWeight = (UINT)FT_WEIGHT::SEMIBOLD;		g_output += L"SemiBold";	break;
		case 7: fontWeight = (UINT)FT_WEIGHT::BOLD;			g_output += L"Bold";		break;
		case 8: fontWeight = (UINT)FT_WEIGHT::EXTRABOLD;	g_output += L"ExtraBold";	break;
		case 9: fontWeight = (UINT)FT_WEIGHT::HEAVY;		g_output += L"Heavy";		break;
		}

		//Font Italic
		NoNamedDefine(count_fontItalic, FT_ITALIC::MAX);
		g_output += L" Italic_";
		switch (count_fontItalic)
		{
		case 0: fontItalic = (bool)FT_ITALIC::True;		g_output += L"True";	break;
		case 1: fontItalic = (bool)FT_ITALIC::False;	g_output += L"False";	break;
		}

		//Font Charset
		NoNamedDefine(count_fontCharSet, FT_CHARSET::MAX);
		g_output += L" CharSet_";
		switch (count_fontCharSet)
		{
		case 0:		fontCharSet = (DWORD)FT_CHARSET::ANSI;			g_output += L"ANSI";		break;
		case 1:		fontCharSet = (DWORD)FT_CHARSET::DEFAULT;		g_output += L"Default";		break;
		case 2:		fontCharSet = (DWORD)FT_CHARSET::SYMBOL;		g_output += L"Symbol";		break;
		case 3:		fontCharSet = (DWORD)FT_CHARSET::SHIFTJIS;		g_output += L"ShiftJIS";	break;
		case 4:		fontCharSet = (DWORD)FT_CHARSET::HANGEUL;		g_output += L"한글";		break;

		case 5:		fontCharSet = (DWORD)FT_CHARSET::GB2312;		g_output += L"GB2312";		break;
		case 6:		fontCharSet = (DWORD)FT_CHARSET::CHINESEBIG5;	g_output += L"ChineseBig5";	break;
		case 7:		fontCharSet = (DWORD)FT_CHARSET::OEM;			g_output += L"OEM";			break;
		case 8:		fontCharSet = (DWORD)FT_CHARSET::JOHAB;			g_output += L"Johab";		break;
		case 9:		fontCharSet = (DWORD)FT_CHARSET::HEBREW;		g_output += L"Hebrew";		break;

		case 10:	fontCharSet = (DWORD)FT_CHARSET::ARABIC;		g_output += L"Arabic";		break;
		case 11:	fontCharSet = (DWORD)FT_CHARSET::GREEK;			g_output += L"Greek";		break;
		case 12:	fontCharSet = (DWORD)FT_CHARSET::TURKISH;		g_output += L"Turkish";		break;
		case 13:	fontCharSet = (DWORD)FT_CHARSET::VIETNAMESE;	g_output += L"Vietnamese";	break;
		case 14:	fontCharSet = (DWORD)FT_CHARSET::THAI;			g_output += L"Thai";		break;

		case 15:	fontCharSet = (DWORD)FT_CHARSET::EASTEUROPE;	g_output += L"EaseEurope";	break;
		case 16:	fontCharSet = (DWORD)FT_CHARSET::RUSSIAN;		g_output += L"Russian";		break;
		case 17:	fontCharSet = (DWORD)FT_CHARSET::MAC;			g_output += L"Mac";			break;
		case 18:	fontCharSet = (DWORD)FT_CHARSET::BALTIC;		g_output += L"Baltic";		break;
		}

		//Font OutputPercision
		NoNamedDefine(count_fontOutputPercision, FT_OUTPUTPRECISION::MAX);
		g_output += L" OutputPercision_";
		switch (count_fontOutputPercision)
		{
		case 0:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::DEFAULT_PRECIS;		g_output += L"Default";			break;
		case 1:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::STRING_PRECIS;			g_output += L"String";			break;
		case 2:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::CHARACTER_PRECIS;		g_output += L"Character";		break;
		case 3:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::STROKE_PRECIS;			g_output += L"Stroke";			break;
		case 4:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::TT_PRECIS;				g_output += L"TT";				break;

		case 5:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::DEVICE_PRECIS;			g_output += L"Device";			break;
		case 6:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::RASTER_PRECIS;			g_output += L"Raster";			break;
		case 7:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::TT_ONLY_PRECIS;		g_output += L"TTOnly";			break;
		case 8:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::OUTLINE_PRECIS;		g_output += L"OutLine";			break;
		case 9:		fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::SCREEN_OUTLINE_PRECIS; g_output += L"ScreenOutline";	break;

		case 10:	fontOutputPercision = (DWORD)FT_OUTPUTPRECISION::PS_ONLY_PRECIS;		g_output += L"PSOnly";			break;
		}

		//Font Quality
		NoNamedDefine(count_fontQuality, FT_QUALITY::MAX);
		g_output += L" Quality_";
		switch (count_fontQuality)
		{
		case 0:		fontQuality = (DWORD)FT_QUALITY::DEFAULT;			g_output += L"Default";				break;
		case 1:		fontQuality = (DWORD)FT_QUALITY::DRAFT;				g_output += L"Draft";				break;
		case 2:		fontQuality = (DWORD)FT_QUALITY::PROOF;				g_output += L"Proof";				break;
		case 3:		fontQuality = (DWORD)FT_QUALITY::NONANTIALIASED;	g_output += L"NonAntialiased";		break;
		case 4:		fontQuality = (DWORD)FT_QUALITY::ANTIALIASED;		g_output += L"Antialiased";			break;

		case 5:		fontQuality = (DWORD)FT_QUALITY::CLEARTYPE;			g_output += L"ClearType";			break;
		case 6:		fontQuality = (DWORD)FT_QUALITY::CLEARTYPE_NATURAL;	g_output += L"ClearTypeNatural";	break;
		}

		//Font Pitch
		NoNamedDefine(count_fontPitch, FT_PITCH::MAX);
		g_output += L" Pitch_";
		switch (count_fontPitch)
		{
		case 0:		fontPitch = (DWORD)FT_PITCH::DEFAULT;		g_output += L"Default";		break;
		case 1:		fontPitch = (DWORD)FT_PITCH::FIXED;			g_output += L"Fixed";		break;
		case 2:		fontPitch = (DWORD)FT_PITCH::VARIABLE;		g_output += L"Variale";		break;
		case 3:		fontPitch = (DWORD)FT_PITCH::MONO_FONT_;	g_output += L"MonoFont";	break;
		}

		//Font Family
		NoNamedDefine(count_fontFamily, FT_FAMILY::MAX);
		g_output += L" Family_";
		switch (count_fontFamily)
		{
		case 0:		fontFamily = (DWORD)FT_FAMILY::DONTCARE;	g_output += L"DontCare";	break;
		case 1:		fontFamily = (DWORD)FT_FAMILY::ROMAN;		g_output += L"Roman";		break;
		case 2:		fontFamily = (DWORD)FT_FAMILY::SWISS;		g_output += L"Swiss";		break;
		case 3:		fontFamily = (DWORD)FT_FAMILY::MODERN;		g_output += L"Modern";		break;
		case 4:		fontFamily = (DWORD)FT_FAMILY::SCRIPT;		g_output += L"Script";		break;

		case 5:		fontFamily = (DWORD)FT_FAMILY::DECORATIVE;	g_output += L"Decorative";	break;
		}


		//Let's make
		HRESULT result = D3DXCreateFontW(g_processManager->GetDevice(), -20, 0, fontWeight, 1, fontItalic, fontCharSet, fontOutputPercision, fontQuality, fontPitch | fontFamily, FONT_NAME, &g_font);
		if (FAILED(result))// && result != E_OUTOFMEMORY)
			cout << "폰트생성 오류발생." << "\nFontHeight : " << count_fontWeight << "\nFontItalic : " << count_fontItalic << "\nFontCharSet : " << count_fontCharSet << "\nFontOutputPercision : " << count_fontOutputPercision << "\nFontQuality : " << count_fontQuality << "\nFontPith : " << count_fontPitch << "\nFontFamily : " << count_fontFamily << endl;
	}
	
	GetSingleton(InputManager)->End();
}

bool MainLoop::Render()
{
	D3DXMATRIX m;
	D3DXMatrixScaling(&m, 3, 3, 3);
	sp->SetTransform(&m);
	sp->Begin(D3DXSPRITE_ALPHABLEND);
	if (!g_font)
		return false;
	RECT rc;
	SetRect(&rc, 25, 10, 0, 0);
	g_font->DrawTextW(sp, g_output.c_str(), -1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	
	g_output.clear();
	SetRect(&rc, 10, 10, 0, 0);
	for (int temp = 0; temp < g_selete; temp++)
		g_output += L"\n";
	g_output += L"★";
	g_font->DrawTextW(sp, g_output.c_str(), -1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	g_font->Release();
	 sp->End();

	/// RECT rc;
	/// int y = 10;
	/// 
	/// SetRect(&rc, 10, y += 30, 0, 0);			pFontItalic->DrawText(NULL, _T("ID3DXFont - 연출 연습용 !@#$ 1234 [Italic]"),				-1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	/// SetRect(&rc, 10, y += 30, 0, 0);			pFontBold->DrawText(NULL, _T("ID3DXFont - 연출 연습용 !@#$ 1234 [Bold]"),					-1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	/// SetRect(&rc, 10, y += 30, 0, 0);			pFontHeavy->DrawText(NULL, _T("ID3DXFont - 연출 연습용 !@#$ 1234 [Heavy]"),					-1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	/// SetRect(&rc, 10, y += 30, 0, 0);			pFontDraftQuality->DrawText(NULL, _T("ID3DXFont - 연출 연습용 !@#$ 1234 [Draft Quality]"),	-1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	/// SetRect(&rc, 10, y += 30, 0, 0);			pFontProofQuality->DrawText(NULL, _T("ID3DXFont - 연출 연습용 !@#$ 1234 [Proof Quality]"),	-1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	/// SetRect(&rc, 10 + 2, y += 30 + 2, 0, 0);	pFont->DrawText(NULL, _T("ID3DXFont - 연출 연습용 !@#$ 1234 [Shadow]"),						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
	/// SetRect(&rc, 10, y - 2, 0, 0);				pFont->DrawText(NULL, _T("ID3DXFont - 연출 연습용 !@#$ 1234 [Shadow]"),						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	/// y += 30;
	/// 
	/// //Render Quality
	/// SetRect(&rc, 10, 10, 0, 0);
	/// if (GetAsyncKeyState('1'))
	/// 	pFontDraftQuality->DrawText(NULL, _T("[Draft Quality]"), -1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	/// else if (GetAsyncKeyState('2'))
	/// 	pFontProofQuality->DrawText(NULL, _T("[Proof Quality]"), -1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	/// else
	/// 	pFont->DrawText(NULL, _T("[Normal]"), -1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	/// 
	/// //Strock
	/// if (GetAsyncKeyState('Z'))
	/// {
	/// 	SetRect(&rc, 10, y - 1, 0, 0);	pFont->DrawText(NULL, _T("[Stroke On]"), -1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f));
	/// 	SetRect(&rc, 11, y, 0, 0);		pFont->DrawText(NULL, _T("[Stroke On]"), -1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f));
	/// 	SetRect(&rc, 10, y + 1, 0, 0);	pFont->DrawText(NULL, _T("[Stroke On]"), -1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f));
	/// 	SetRect(&rc, 9, y, 0, 0);		pFont->DrawText(NULL, _T("[Stroke On]"), -1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f));
	/// 	SetRect(&rc, 10, y, 0, 0);		pFont->DrawText(NULL, _T("[Stroke On]"), -1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	/// }
	/// else
	/// 	SetRect(&rc, 10, y, 0, 0);		pFont->DrawText(NULL, _T("[Stroke Off]"), -1, &rc, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	/// sp->Begin(D3DXSPRITE_ALPHABLEND);
	/// RECT sb;
	/// sb.left		= 200;
	/// sb.top		= 200;
	/// sb.right	= 400;
	/// sb.bottom	= 400;
	/// yo->DrawTextW(NULL, L"Yo", -1, &sb, DT_NOCLIP | DT_CENTER, D3DXCOLOR(1, 1, 0, 1));
	/// yo->DrawTextW(NULL, L"Center", -1, &sb, DT_NOCLIP | DT_CENTER | DT_VCENTER, D3DXCOLOR(1, 1, 0, 1));
	/// yo->DrawTextW(NULL, L"Hello,\tworld!\nHello, world!", -1, &sb, DT_NOCLIP | DT_CENTER | DT_BOTTOM, D3DXCOLOR(1, 1, 0, 1));
	/// sp->End();
	return false;
}

bool MainLoop::Relase()
{
	ReleaseSingleton( InputManager );
	///	for (int count_fontWeight = 0; count_fontWeight < (int)FT_WEIGHT::MAX; count_fontWeight++)
	///		for (int count_fontItalic = 0; count_fontItalic < (int)FT_ITALIC::MAX; count_fontItalic++)
	///			for (int count_fontCharSet = 0; count_fontCharSet < (int)FT_CHARSET::MAX; count_fontCharSet++)
	///				for (int count_fontOutputPercision = 0; count_fontOutputPercision < (int)FT_OUTPUTPRECISION::MAX; count_fontOutputPercision++)
	///					for (int count_fontQuality = 0; count_fontQuality < (int)FT_QUALITY::MAX; count_fontQuality++)
	///						for (int count_fontPitch = 0; count_fontPitch < (int)FT_PITCH::MAX; count_fontPitch++)
	///							for (int count_fontFamily = 0; count_fontFamily < (int)FT_FAMILY::MAX; count_fontFamily++)
	///								SAFE_RELEASE( g_fontList[count_fontWeight][count_fontItalic][count_fontCharSet][count_fontOutputPercision][count_fontQuality][count_fontPitch][count_fontFamily] );
	FreeConsole();
	sp->Release();
	return false;
}

LRESULT MainLoop::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetSingleton(InputManager)->WndProc(message, wParam, lParam);

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}
}
