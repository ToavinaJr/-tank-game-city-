#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QColorDialog>
#include <QObject>


class SettingsWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit SettingsWidget(QWidget* parent = nullptr);
    
signals:
    void backClicked();
    void settingsChanged();
    
private slots:
    void onSoundToggled(bool checked);
    void onMusicVolumeChanged(int value);
    void onSfxVolumeChanged(int value);
    void onTankColorClicked();
    void onSaveClicked();
    void onBackClicked();
    
private:
    void setupUI();
    void loadSettings();
    void saveSettings();
    
    QCheckBox* m_soundCheckBox;
    QSlider* m_musicVolumeSlider;
    QSlider* m_sfxVolumeSlider;
    QPushButton* m_tankColorButton;
    QPushButton* m_saveButton;
    QPushButton* m_backButton;
    QLabel* m_musicVolumeLabel;
    QLabel* m_sfxVolumeLabel;
    
    QColor m_selectedTankColor;
};

#endif // SETTINGSWIDGET_H
