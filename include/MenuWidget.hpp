#ifndef MENUWIDGET_HPP
#define MENUWIDGET_HPP

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);

signals:
    void startGame();
    void openSettings();
    void quitGame();

private slots:
    void onStartClicked();
    void onSettingsClicked();
    void onQuitClicked();

private:
    QLabel *m_titleLabel;
    QPushButton *m_startButton;
    QPushButton *m_settingsButton;
    QPushButton *m_quitButton;
    QVBoxLayout *m_layout;
};

#endif // MENUWIDGET_HPP
