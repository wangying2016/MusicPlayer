// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include "HttpGetResouce.h"
#include "../controls.extend/FileHelper.h"

#ifdef DWMBLUR	//win7毛玻璃开关
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
#endif

//////////////////////////////////////////////////////////////////////////
// 全局回调函数
int  CALLBACK  CallbackFunc(void* instance, void *user_data, TCallbackMessage message
	, unsigned int param1, unsigned int param2);

//////////////////////////////////////////////////////////////////////////
// 播放列表adpater
CTestAdapterFix::CTestAdapterFix()
{

}

CTestAdapterFix::~CTestAdapterFix()
{

}

int CTestAdapterFix::getCount()
{
	return m_vecMusicList.size();
}

void CTestAdapterFix::getView(int position, SWindow * pItem, pugi::xml_node xmlTemplate)
{
	if (pItem->GetChildrenCount() == 0)
	{
		pItem->InitFromXml(xmlTemplate);
	}
	InitOneView(position, pItem);
}

void CTestAdapterFix::InitOneView(int position, SWindow *pItem)
{
	// 处理路径字符串得到文件名
	SStringW strFilePath = m_vecMusicList[position];
	INT nCount = 0;
	INT nChangePre = 0;
	INT nChangeBck = 0;
	while (-1 != nChangePre) {
		nChangePre = strFilePath.Find(L"\\", nCount);
		if (-1 != nChangePre) {
			nChangeBck = nChangePre;
		} 
		nCount++;
	}
	if (-1 == nChangeBck) {
		nChangeBck = 1;
	}
	SStringW strFileName = strFilePath.Right(strFilePath.GetLength() - nChangeBck - 1);
	// 设置音频名称
	SOUI::SWindow *pText = pItem->FindChildByName2<SOUI::SWindow>(L"text_file_path");
	assert(pText);
	pText->SetWindowTextW(strFileName);
}

void CTestAdapterFix::UpdateList(std::vector<SStringW> &pMusicList)
{
	m_vecMusicList = pMusicList;
}

//////////////////////////////////////////////////////////////////////////
// 主对话框类

CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
	m_pPlayer = NULL;
	m_bPaused = FALSE;
}

CMainDlg::~CMainDlg()
{
	m_pPlayer->Close();
	m_pPlayer->Release();
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	#ifdef DWMBLUR	//win7毛玻璃开关
	MARGINS mar = {5,5,30,5};
	DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
	#endif
	m_pPlayer = CreateZPlay();
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	// 绑定一系列按钮操作
	SOUI::SButton *pButton = FindChildByName2<SOUI::SButton>(L"button_play_music");
	assert(pButton);
	pButton->GetEventSet()->subscribeEvent(SOUI::EVT_CMD, Subscriber(&CMainDlg::OnPlayButtonClick, this));
	// 初始化list
	SListView *pLstViewFix = FindChildByName2<SListView>("lv_music_fix");
	if (pLstViewFix)
	{
		ILvAdapter *pAdapter = new CTestAdapterFix;
		pLstViewFix->SetAdapter(pAdapter);
		pAdapter->Release();
	}
	// 音量滚动条
	SSliderBar* pSliderBar = FindChildByName2<SSliderBar>(L"slider_play_volume");
	assert(pSliderBar);
	pSliderBar->GetEventSet()->subscribeEvent(SOUI::EVT_SLIDER_POS, Subscriber(&CMainDlg::OnSetPlayVolume, this));
	// 初始化滚动条
	UINT uLeftVolume = 0;
	UINT uRightVolume = 0;
	m_pPlayer->GetPlayerVolume(&uLeftVolume, &uRightVolume);
	pSliderBar->SetValue((uLeftVolume + uRightVolume) / 2.0);
	// 设置回调响应函数
	CombindCallbackFunc();

	return 0;
}

void CMainDlg::PlayMusic()
{
	// 获取当前播放文件路径
	SListView *pLstViewFix = FindChildByName2<SListView>("lv_music_fix");
	assert(pLstViewFix);
	SStringW strMusicPath;
	INT nSel = pLstViewFix->GetSel();
	if (0 <= nSel && m_vecMusicList.size() > nSel) {
		strMusicPath = m_vecMusicList[nSel];
	}
	else {
		strMusicPath = L"";
	}
	// 打开文件，失败则弹出错误信息
	if (m_pPlayer->OpenFileW(strMusicPath.GetBuffer(strMusicPath.GetLength()), sfAutodetect) == 0) {
		SMessageBox(NULL, L"没有找到播放文件", L"提示", MB_OK);
	}
	else {
		m_pPlayer->Play();
		SOUI::SButton *pButton = FindChildByName2<SOUI::SButton>(L"button_play_music");
		assert(pButton);
		pButton->SetWindowTextW(L"停止");
		// 设置很短的timer移动滚动条
		SetTimer(m_uRefreshSliderTimer, 100);
	}
}

void CMainDlg::PlayStop()
{
	SOUI::SButton *pButton = FindChildByName2<SOUI::SButton>(L"button_play_music");
	assert(pButton);
	pButton->SetWindowTextW(L"播放");
	m_pPlayer->Stop();
}

void CMainDlg::PlayPause()
{
	SOUI::SWindow *pButtonPause = FindChildByName2<SOUI::SWindow>(L"button_pause_music");
	assert(pButtonPause);
	if (!m_bPaused) {
		m_pPlayer->Pause();
		m_bPaused = TRUE;
		pButtonPause->SetWindowTextW(L"播放");
	} else {
		m_pPlayer->Resume();
		m_bPaused = FALSE;
		pButtonPause->SetWindowTextW(L"暂停");
	}
}

void CMainDlg::SetSliderPosition()
{
	// 计算当前比率
	TStreamTime curTime;
	TStreamTime totalTime;
	DOUBLE curRate = 0.0;
	m_pPlayer->GetPosition(&curTime);
	TStreamInfo info;
	m_pPlayer->GetStreamInfo(&info);
	totalTime = info.Length;
	curRate = (DOUBLE)(curTime.hms.minute * 60 + curTime.hms.second) /
		(totalTime.hms.minute * 60 + totalTime.hms.second);
	// 设置位置
	INT nPos = 255 * curRate;
	SOUI::SSliderBar *pSliderBar = FindChildByName2<SOUI::SSliderBar>(L"slider_play_time");
	assert(pSliderBar);
	pSliderBar->SetValue(nPos);
	// 设置时间显示
	SOUI::SWindow *pTextCurTime = FindChildByName2<SOUI::SWindow>(L"text_play_time");
	assert(pTextCurTime);
	pTextCurTime->SetWindowTextW(SStringW().Format(_T("播放中时间%d分%d秒"), curTime.hms.minute, 
		curTime.hms.second));
	SOUI::SWindow *pTextTotalTime = FindChildByName2<SOUI::SWindow>(L"text_total_time");
	assert(pTextTotalTime);
	pTextTotalTime->SetWindowTextW(SStringW().Format(_T("音频总时间%d分%d秒"), totalTime.hms.minute, 
		totalTime.hms.second));
}

bool CMainDlg::OnPlayButtonClick(SOUI::EventArgs* pEvt)
{
	SOUI::SButton *pButton = FindChildByName2<SOUI::SButton>(L"button_play_music");
	assert(pButton);
	SStringW strText = pButton->GetWindowTextW();
	if (L"停止" == strText) {
		PlayStop();
	}
	else {
		PlayMusic();
	}

	return true;
}

bool CMainDlg::OnSetPlayVolume(SOUI::EventArgs* pEvt)
{
	SSliderBar* pSliderBar = FindChildByName2<SSliderBar>(L"slider_play_volume");
	assert(pSliderBar);
	INT nPos = pSliderBar->GetValue();
	m_pPlayer->SetPlayerVolume(nPos, nPos);
	
	return true;
}

void CMainDlg::OnOpenFile()
{
	CFileDialogEx openDlg(TRUE, _T("mp3"), 0, 6, _T("mp3 files(*.mp3)\0*.mp3\0All files (*.*)\0*.*\0\0"));
	if (openDlg.DoModal() == IDOK) {
		// 读取文件路径
		SStringW strFilePath = openDlg.m_szFileName;
		m_vecMusicList.push_back(strFilePath);
		// 给m_vecMusicList去重
		std::vector<SStringW>::iterator end_unique = std::unique(m_vecMusicList.begin(), m_vecMusicList.end());
		m_vecMusicList.erase(end_unique, m_vecMusicList.end());
		// 更新list的数据
		SListView *pLstViewFix = FindChildByName2<SListView>("lv_music_fix");
		assert(pLstViewFix);
		CTestAdapterFix *pAdapter = static_cast<CTestAdapterFix*>(pLstViewFix->GetAdapter());
		assert(pAdapter);
		pAdapter->UpdateList(m_vecMusicList);
		pAdapter->notifyDataSetChanged();
	}
}

void CMainDlg::OnTimer(UINT_PTR idEvent)
{
	switch (idEvent) {
	case m_uRefreshSliderTimer:
	{
		SetSliderPosition();
	}
	break;
	}
}

int  CALLBACK  CallbackFunc(void* instance, void *user_data, TCallbackMessage message
	, unsigned int param1, unsigned int param2)
{
	ZPlay *myPlayer = (ZPlay*)instance;
	assert(myPlayer);
	CMainDlg *pDlg = (CMainDlg *)user_data;
	assert(pDlg);

	switch (message) {
	// 播放停止
	case MsgStop:
	{
		SOUI::SWindow *pButtonPlay = pDlg->FindChildByName2<SOUI::SWindow>(L"button_play_music");
		assert(pButtonPlay);
		pButtonPlay->SetWindowTextW(L"播放");
	}
	break;
	}

	return 0;
}

void CMainDlg::CombindCallbackFunc()
{
	// 播放停止回调
	m_pPlayer->SetCallbackFunc(CallbackFunc, MsgStop, (void *)this);
}

void CMainDlg::OnClose()
{
	CSimpleWnd::DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

