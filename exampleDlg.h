#pragma once

#include <QtWidgets/QWidget>
#include <windows.h>

class QLabel;
class QTableWidget;
class QStackedWidget;
class QToolButton;
class QCheckBox;
class QComboBox;
class QCloseEvent;
class QKeyEvent;
class QMouseEvent;
class QEvent;
class QObject;
class QByteArray;

class CexampleDlg : public QWidget
{
public:
    explicit CexampleDlg(QWidget* parent = nullptr);
    ~CexampleDlg() override;

    void UpdateList(long index, long action);
    void CoordUpdate(int x, int y);

    void OnBnClickedButtonCloseallwin();
    void OnBnClickedButtonStartall();
    void OnBnClickedButtonStopall();
    void OnBnClickedButtonPauseall();
    void OnBnClickedButtonResumeall();
    void OnBnClickedButtonSoft();
    LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);

    int GetautoRuneSolver();
    int GetfriendPlayerNotification();
    int GethuangMen();
    int GetexpPot();
    int GetEXP_PARK();
    int Gethunt();
    int Getmap();
    int Getkuxing();
    int GetautoWealth();
    int GetautoOil();
    int Getignite();
    int GetExp10();
    int GetExp30();
    int GetWhiteDetect();

protected:
    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void InitUi();
    void InitCombos();
    void LoadUiState();
    void SaveUiState();
    int FindListIndex(long index) const;
    void SyncTabSelection(int tabIndex);
    void RegisterHotKeys();
    void UnregisterHotKeys();

    QLabel* m_coordLabel;
    QTableWidget* m_statusList;
    QStackedWidget* m_optionTabs;
    QToolButton* m_tabCommon;
    QToolButton* m_tabDetect;

    QToolButton* autoRuneSolver;
    QToolButton* friendPlayerNotification;
    QToolButton* huangMen;
    QToolButton* expPot;
    QToolButton* EXP_PARK;
    QToolButton* hunt;
    QComboBox* mapList;
    QToolButton* EXP_MVP;
    QToolButton* autoWealth;
    QToolButton* autoOil;
    QToolButton* bless;
    QToolButton* EXP15M;
    QToolButton* EXP30M;
    QToolButton* whiteDetect;

    QWidget* m_titleBar;
    HWND m_hwnd;
    QLabel* m_bgLabel;
    QPixmap m_originalBg;
};
