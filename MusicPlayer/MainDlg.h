// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////////////////////////////////////////////////////////////////
// 播放列表adpater
class CTestAdapterFix : public SAdapterBase
{
public:
	CTestAdapterFix();

	~CTestAdapterFix();

	void UpdateList(std::vector<SStringW> &pMusicList);

protected:
	virtual int getCount();

	virtual void getView(int position, SWindow * pItem, pugi::xml_node xmlTemplate);

	void InitOneView(int position, SWindow *pItem);

private:
	std::vector<SStringW> m_vecMusicList;
};

//////////////////////////////////////////////////////////////////////////
// 主对话框类

class CMainDlg : public SHostWnd
{
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, CSize size);

	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

protected:// 播放相关
	void PlayMusic();
	void PlayStop();
	void PlayPause();
	void SetSliderPosition();

protected:// 逻辑相关
	bool OnPlayButtonClick(SOUI::EventArgs* pEvt);
	bool OnSetPlayVolume(SOUI::EventArgs* pEvt);
	void OnOpenFile();
	void OnTimer(UINT_PTR idEvent);

protected:// 回调相关
	void CombindCallbackFunc();

protected:
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		EVENT_NAME_COMMAND(L"button_open_file", OnOpenFile)
		EVENT_NAME_COMMAND(L"button_pause_music", PlayPause)
		EVENT_MAP_END()
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		MSG_WM_TIMER(OnTimer)

		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
private:
	static const char m_uRefreshSliderTimer = 100;
	BOOL			m_bLayoutInited;
	BOOL			m_bPaused;
	ZPlay			*m_pPlayer;
	std::vector<SStringW> m_vecMusicList;
};
