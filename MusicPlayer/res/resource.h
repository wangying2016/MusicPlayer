//stamp:0749a0e5dd0b0ecc
/*<------------------------------------------------------------------------------------------------->*/
/*该文件由uiresbuilder生成，请不要手动修改*/
/*<------------------------------------------------------------------------------------------------->*/
#pragma once
#include <res.mgr/snamedvalue.h>
#define ROBJ_IN_CPP \
namespace SOUI \
{\
    const _R R;\
    const _UIRES UIRES;\
}
namespace SOUI
{
	class _UIRES{
		public:
		class _UIDEF{
			public:
			_UIDEF(){
				XML_INIT = _T("UIDEF:XML_INIT");
			}
			const TCHAR * XML_INIT;
		}UIDEF;
		class _LAYOUT{
			public:
			_LAYOUT(){
				XML_MAINWND = _T("LAYOUT:XML_MAINWND");
			}
			const TCHAR * XML_MAINWND;
		}LAYOUT;
		class _values{
			public:
			_values(){
				string = _T("values:string");
				color = _T("values:color");
				skin = _T("values:skin");
			}
			const TCHAR * string;
			const TCHAR * color;
			const TCHAR * skin;
		}values;
		class _IMGX{
			public:
			_IMGX(){
				jpg_bkgnd = _T("IMGX:jpg_bkgnd");
			}
			const TCHAR * jpg_bkgnd;
		}IMGX;
		class _ICON{
			public:
			_ICON(){
				ICON_LOGO = _T("ICON:ICON_LOGO");
			}
			const TCHAR * ICON_LOGO;
		}ICON;
	};
	const SNamedID::NAMEDVALUE namedXmlID[]={
		{L"btn_close",65536},
		{L"btn_max",65537},
		{L"btn_min",65539},
		{L"btn_restore",65538},
		{L"button_open_file",65540},
		{L"button_pause_music",65542},
		{L"button_play_music",65541},
		{L"lv_music_fix",65546},
		{L"slider_play_time",65548},
		{L"slider_play_volume",65543},
		{L"text_file_path",65547},
		{L"text_play_time",65544},
		{L"text_total_time",65545}		};
	class _R{
	public:
		class _name{
		public:
		_name(){
			btn_close = namedXmlID[0].strName;
			btn_max = namedXmlID[1].strName;
			btn_min = namedXmlID[2].strName;
			btn_restore = namedXmlID[3].strName;
			button_open_file = namedXmlID[4].strName;
			button_pause_music = namedXmlID[5].strName;
			button_play_music = namedXmlID[6].strName;
			lv_music_fix = namedXmlID[7].strName;
			slider_play_time = namedXmlID[8].strName;
			slider_play_volume = namedXmlID[9].strName;
			text_file_path = namedXmlID[10].strName;
			text_play_time = namedXmlID[11].strName;
			text_total_time = namedXmlID[12].strName;
		}
		 const wchar_t * btn_close;
		 const wchar_t * btn_max;
		 const wchar_t * btn_min;
		 const wchar_t * btn_restore;
		 const wchar_t * button_open_file;
		 const wchar_t * button_pause_music;
		 const wchar_t * button_play_music;
		 const wchar_t * lv_music_fix;
		 const wchar_t * slider_play_time;
		 const wchar_t * slider_play_volume;
		 const wchar_t * text_file_path;
		 const wchar_t * text_play_time;
		 const wchar_t * text_total_time;
		}name;

		class _id{
		public:
		const static int btn_close	=	65536;
		const static int btn_max	=	65537;
		const static int btn_min	=	65539;
		const static int btn_restore	=	65538;
		const static int button_open_file	=	65540;
		const static int button_pause_music	=	65542;
		const static int button_play_music	=	65541;
		const static int lv_music_fix	=	65546;
		const static int slider_play_time	=	65548;
		const static int slider_play_volume	=	65543;
		const static int text_file_path	=	65547;
		const static int text_play_time	=	65544;
		const static int text_total_time	=	65545;
		}id;

		class _string{
		public:
		const static int title	=	0;
		const static int ver	=	1;
		}string;

		class _color{
		public:
		const static int blue	=	0;
		const static int gray	=	1;
		const static int green	=	2;
		const static int red	=	3;
		const static int white	=	4;
		}color;

	};

#ifdef R_IN_CPP
	 extern const _R R;
	 extern const _UIRES UIRES;
#else
	 extern const __declspec(selectany) _R & R = _R();
	 extern const __declspec(selectany) _UIRES & UIRES = _UIRES();
#endif//R_IN_CPP
}
