#ifndef WINDOW_H
#define WINDOW_H

#include <QApplication>
#include <QWidget>
#include <QtWidgets>
#include <QString>
#include <QProcess>
#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

class Window : public QWidget
{
    Q_OBJECT

public:
    //constructor (idk why a parameter has to be sent in)
    explicit Window(QWidget *parent = nullptr);

private:

    //loads settings from the JSON file
    //triggered when opening the program
    void LoadSettings();
    void LoadSettingsFromObject(QJsonObject settings);
    QString settingsFileName = "settings.json";

    //saves settings to a JSON object
    //done during SaveSettings();
    void SaveSettingsToObject(QJsonObject& settings);

    //sets the arguments in a list so they can be passed to yt-dlp
    void SetArguments(QStringList& arguments);

    //main layout which will contain everything
    QVBoxLayout mainLayout;

    //layout with url text field
    QHBoxLayout urlLayout;
    QLabel urlLabel;
    QLineEdit urlField;

    //layout with directory text field and browse button
    QHBoxLayout directoryLayout;
    QLabel directoryLabel;
    QLineEdit directoryField;
    QPushButton browseButton;
    //current "saved" directory
    QString currentDirectory;

    //layout with option to download as either mp3 or mp4 or neither
    QHBoxLayout fileTypeLayout;
    QLabel fileTypeLabel;
    QRadioButton mp4Button;
    QRadioButton mp3Button;
    QRadioButton noneButton;
    QButtonGroup fileTypeButtonGroup;

    //layout with options for file name formatting
    QVBoxLayout fileNameBigLayout;

    QGridLayout fileNameTopLayout;
    QLabel fileNameLabel;
    QCheckBox playlistIndexBox;
    QCheckBox uploadDateBox;
    QCheckBox channelBox;
    QCheckBox artistBox;

    QHBoxLayout fileNameBottomLayout;
    QCheckBox customNameBox;
    QLineEdit customNameField;

    //layout with other options below
    QVBoxLayout miscOptionBigLayout;

    //layout with option to trim the video
    QHBoxLayout trimLayout;
    QCheckBox trimBox;
    QLabel startLabel;
    QLabel endLabel;
    QLineEdit startField;
    QLineEdit endField;

    //layout with option to limit resolution
    QHBoxLayout limitResolutionLayout;
    QCheckBox limitResolutionBox;
    QLineEdit limitResolutionField;

    //layout with multithreading option
    QHBoxLayout multiThreadLayout;
    QCheckBox multiThreadBox;
    QSpinBox multiThreadField;

    //inner layout with miscellaneous options
    QGridLayout miscOptionInnerLayout;
    QLabel miscOptionLabel;
    QCheckBox saveThumbnailBox;
    QCheckBox embedThumbnailBox;
    QCheckBox addMetadataBox;
    QCheckBox descriptionBox;
    QCheckBox vp9Box;
    QCheckBox consoleWindowBox;

    //layout for extra arguments
    QHBoxLayout extraArgumentsLayout;
    QCheckBox extraArgumentsBox;
    QLineEdit extraArgumentsField;

    //layout for window settings
    QHBoxLayout windowSettingsLayout;

    //theme layout
    QHBoxLayout themeLayout;
    QLabel themeLabel;
    QRadioButton darkModeButton;
    QRadioButton lightModeButton;
    QButtonGroup themeButtonGroup;

    //always on top checkbox
    QCheckBox alwaysOnTopBox;

    //download button
    QPushButton downloadButton;


private slots:
    //run the command to download the file with yt-dlp
    //triggered when download button is pressed
    void DownloadVideo();

    //opens file explorer to select a folder
    //triggered when "browse" button is clicked
    void SelectDirectory();

    //saves settings to a JSON file
    //triggered when download button is pressed
    void SaveSettings();

    //light and dark themes
    void ApplyTheme();

    //gray out buttons on each layout when the appropriate boxes are checked
    void UpdateTrimLayout();
    void UpdateFileNameLayout();
    void UpdateResolutionLayout();
    void UpdateMultiThreadLayout();
    void UpdateExtraArgumentsLayout();

    //change whether it's always on top
    void UpdateAlwaysOnTopSetting();

};

#endif // WINDOW_H
