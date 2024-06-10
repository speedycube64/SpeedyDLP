#include "window.h"

#define DEBUG

//constructor (idk why a parameter has to be sent in)
Window::Window(QWidget *parent) : QWidget{parent}
{
    LoadSettings();

    //----------------MAIN LAYOUT-----------------

    //attach the main layout to the window
    setLayout(&mainLayout);

    //add the main components in order to the main layout
    mainLayout.addLayout(&urlLayout);
    mainLayout.addLayout(&directoryLayout);
    mainLayout.addLayout(&fileTypeLayout);
    mainLayout.addLayout(&fileNameBigLayout);
    mainLayout.addLayout(&miscOptionBigLayout);
    mainLayout.addLayout(&windowSettingsLayout);
    mainLayout.addWidget(&downloadButton);

    //----------------URL LAYOUT----------------

    //add the components in order to the url layout
    urlLayout.addWidget(&urlLabel);
    urlLayout.addWidget(&urlField);

    urlLabel.setText("URL:");

    //----------------DIRECTORY LAYOUT----------------

    directoryLayout.addWidget(&directoryLabel);
    directoryLayout.addWidget(&directoryField);
    directoryLayout.addWidget(&browseButton);

    directoryLabel.setText("Directory:");
    browseButton.setText("Browse");

    connect(&browseButton, SIGNAL (clicked()), this, SLOT (SelectDirectory()));

    //----------------FILE TYPE LAYOUT----------------

    //add the components in order to the file type layout
    fileTypeLayout.addWidget(&fileTypeLabel);
    fileTypeLayout.addWidget(&mp4Button);
    fileTypeLayout.addWidget(&mp3Button);
    fileTypeLayout.addWidget(&noneButton);

    //add extra space at the end to push the buttons to the left
    fileTypeLayout.addStretch();

    fileTypeLabel.setText("File type:");

    mp4Button.setText("MP4");
    mp3Button.setText("MP3");

    noneButton.setText("None");
    noneButton.setToolTip("Use this option if you only want to download the thumbnail or description.");

    //put the buttons in a group so only one can be selected at a time
    fileTypeButtonGroup.addButton(&mp4Button);
    fileTypeButtonGroup.addButton(&mp3Button);
    fileTypeButtonGroup.addButton(&noneButton);

    //----------------FILE NAME FORMAT LAYOUT----------------

    fileNameBigLayout.addLayout(&fileNameTopLayout);
    fileNameBigLayout.addLayout(&fileNameBottomLayout);

    //0th row
    fileNameTopLayout.addWidget(&fileNameLabel, 0, 0);

    //1st row
    fileNameTopLayout.addWidget(&playlistIndexBox, 1, 0);
    fileNameTopLayout.addWidget(&uploadDateBox, 1, 1);

    //2nd row
    fileNameTopLayout.addWidget(&channelBox, 2, 0);
    fileNameTopLayout.addWidget(&artistBox, 2, 1);

    //add blank space on the right
    fileNameTopLayout.setColumnStretch(2, 1);

    //4th row
    fileNameBottomLayout.addWidget(&customNameBox);
    fileNameBottomLayout.addWidget(&customNameField);

    fileNameLabel.setText("File name formatting:");
    fileNameLabel.setToolTip("The title will be part of the file name by default unless \"Custom file name\" is selected.");

    playlistIndexBox.setText("Playlist index");

    uploadDateBox.setText("Upload date");

    channelBox.setText("Channel name");

    artistBox.setText("Artist name(s)");

    customNameBox.setText("Custom file name:");
    customNameBox.setToolTip("If this option is selected, all above options will be ignored.\n"
                              "You may also use yt-dlp arguments here.");

    //connect the checkbox getting checked or unchecked to updating the layout
    connect(&customNameBox, &QCheckBox::stateChanged, this, &Window::UpdateFileNameLayout);

    //----------------TRIM VIDEO LAYOUT----------------

    trimLayout.addWidget(&trimBox);
    trimLayout.addWidget(&startLabel);
    trimLayout.addWidget(&startField);
    trimLayout.addWidget(&endLabel);
    trimLayout.addWidget(&endField);

    trimLayout.addStretch();

    trimBox.setText("Trim video:");
    trimBox.setToolTip("Requires FFmpeg.\nYou can leave fields blank to keep the original start and end.");

    startLabel.setText("Start:");
    startField.setPlaceholderText("XX:XX");
    startField.setFixedWidth(55);

    endLabel.setText("End:");
    endField.setPlaceholderText("XX:XX");
    endField.setFixedWidth(55);

    //connect the checkbox getting checked or unchecked to updating the layout
    connect(&trimBox, &QCheckBox::stateChanged, this, &Window::UpdateTrimLayout);

    //----------------MISC OPTION OUTER LAYOUT----------------

    miscOptionBigLayout.addWidget(&miscOptionLabel);
    miscOptionBigLayout.addLayout(&trimLayout);
    miscOptionBigLayout.addLayout(&miscOptionInnerLayout);
    miscOptionBigLayout.addLayout(&extraArgumentsLayout);

    miscOptionLabel.setText("Other options:");

    //----------------LIMIT RESOLUTION LAYOUT----------------

    limitResolutionLayout.addWidget(&limitResolutionBox);
    limitResolutionLayout.addWidget(&limitResolutionField);

    limitResolutionLayout.addStretch();

    limitResolutionBox.setText("Limit resolution:");
    limitResolutionBox.setToolTip("If checked, the height of the video will be limited to the number of pixels entered.");

    //make it so only numbers can be entered
    limitResolutionField.setValidator(new QIntValidator(0));

    limitResolutionField.setFixedWidth(35);
    limitResolutionField.setPlaceholderText("1080");

    //connect the checkbox getting checked or unchecked to updating the layout
    connect(&limitResolutionBox, &QCheckBox::stateChanged, this, &Window::UpdateResolutionLayout);

    //----------------MULTITHREADING LAYOUT----------------

    multiThreadLayout.addWidget(&multiThreadBox);
    multiThreadLayout.addWidget(&multiThreadField);

    multiThreadLayout.addStretch();

    multiThreadField.setMinimum(2);
    multiThreadField.setFixedWidth(35);

    multiThreadBox.setText("Multiple threads:");
    multiThreadBox.setToolTip("This will speed up downloads, but your connection may be kicked from some sites.");

    connect(&multiThreadBox, &QCheckBox::stateChanged, this, &Window::UpdateMultiThreadLayout);

    //----------------MISC OPTION INNER LAYOUT----------------

    //add each item to its appropriate spot

    //left column
    miscOptionInnerLayout.addLayout(&limitResolutionLayout, 0, 0);
    miscOptionInnerLayout.addWidget(&saveThumbnailBox, 1, 0);
    miscOptionInnerLayout.addWidget(&embedThumbnailBox, 2, 0);
    miscOptionInnerLayout.addWidget(&vp9Box, 3, 0);

    //right column
    miscOptionInnerLayout.addLayout(&multiThreadLayout, 0, 1);
    miscOptionInnerLayout.addWidget(&descriptionBox, 1, 1);
    miscOptionInnerLayout.addWidget(&addMetadataBox, 2, 1);

    //only add the console window box if on windows
    #ifdef Q_OS_WIN
        miscOptionInnerLayout.addWidget(&consoleWindowBox, 3, 1);
    #endif

    //add blank space on the right
    miscOptionInnerLayout.setColumnStretch(2, 1);

    saveThumbnailBox.setText("Save thumbnail as PNG");
    saveThumbnailBox.setToolTip("The image will have the same name as the downloaded video.");

    embedThumbnailBox.setText("Embed thumbnail");
    embedThumbnailBox.setToolTip("The video thumbnail in File Explorer will match that on YouTube.");

    descriptionBox.setText("Save description as TXT");
    descriptionBox.setToolTip("The text document will have the same name as the downloaded video.");

    vp9Box.setText("Allow VP9 codec");
    vp9Box.setToolTip("This codec has a better bitrate, but it is incompatible with some editing programs and video players.");

    addMetadataBox.setText("Add metadata");

    consoleWindowBox.setText("Show output in console window");
    consoleWindowBox.setToolTip("Windows only. Can be used for monitoring download progress.");

    //----------------EXTRA ARGUMENTS LAYOUT----------------

    extraArgumentsLayout.addWidget(&extraArgumentsBox);
    extraArgumentsLayout.addWidget(&extraArgumentsField);

    extraArgumentsBox.setText("Extra arguments:");
    extraArgumentsBox.setToolTip("Refer to the yt-dlp documentation for a list of all arguments.");

    connect(&extraArgumentsBox, &QCheckBox::stateChanged, this, &Window::UpdateExtraArgumentsLayout);

    //----------------WINDOW SETTINGS OUTER LAYOUT----------------

    windowSettingsLayout.addLayout(&themeLayout);
    windowSettingsLayout.addWidget(&alwaysOnTopBox);

    windowSettingsLayout.addStretch();

    //----------------THEME LAYOUT----------------

    themeLayout.addWidget(&themeLabel);
    themeLayout.addWidget(&darkModeButton);
    themeLayout.addWidget(&lightModeButton);

    themeButtonGroup.addButton(&darkModeButton);
    themeButtonGroup.addButton(&lightModeButton);

    themeLabel.setText("Theme:");
    darkModeButton.setText("Dark");
    lightModeButton.setText("Light");

    connect(&themeButtonGroup, &QButtonGroup::buttonClicked, this, &Window::ApplyTheme);

    //----------------ALWAYS ON TOP BOX----------------

    alwaysOnTopBox.setText("Always on top");

    connect(&alwaysOnTopBox, &QCheckBox::stateChanged, this, &Window::UpdateAlwaysOnTopSetting);

    //----------------DOWNLOAD BUTTON----------------

    //add the button to download the video
    downloadButton.setText("Download");

    //if the download button is clicked, assemble and run the command
    connect(&downloadButton, &QPushButton::clicked, this, &Window::DownloadVideo);

    //also save the current settings
    connect(&downloadButton, &QPushButton::clicked, this, &Window::SaveSettings);

}

void Window::DownloadVideo()
{
    //directory for video to be downloaded in
    QString directory = directoryField.text();

    //list of arguments to be passed into yt-dlp
    QStringList arguments;

    //read the checkboxes and other doohickeys to determine what the arguments are
    SetArguments(arguments);

    //print out all the arguments if debug is enabled
    #ifdef DEBUG
        for (const QString &str : arguments)
        {
            qDebug() << str;
        }
    #endif

    //create the process
    QProcess process;


    //if command line output option is checked
    if(consoleWindowBox.isChecked())
    {
        //if using Windows
        #ifdef Q_OS_WIN

            process.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args) {
                args->flags |= CREATE_NEW_CONSOLE;
            });

        #endif
    }

    //set the parameters of the process
    process.setWorkingDirectory (directory);
    process.setProgram("yt-dlp.exe");
    process.setArguments(arguments);

    //run the process
    process.startDetached();

}

void Window::SelectDirectory()
{

    QString currentDirectory = directoryField.text();

    //open a file selection window at the saved directory
    //(if the current directory is empty, it should open where the executable is)
    QString newDirectory = QFileDialog::getExistingDirectory
        (this, "Choose Folder To Download To", currentDirectory,
         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    //only update the directory if "select" was hit instead of "cancel"
    if(!newDirectory.isEmpty())
    {
        //update the text field
        directoryField.setText(newDirectory);
    }

}

void Window::UpdateTrimLayout()
{
    //enable or disable each widget in the layout (except for the first one)
    for (int i = 1; i < trimLayout.count() - 1; i++)
    {
        //set the enabled state of each item to be the same as the checkbox
        trimLayout.itemAt(i)->widget()->setEnabled(trimBox.isChecked());
    }
}

void Window::UpdateFileNameLayout()
{
    //enable or disable each checkbox in the top layout
    for (int i = 1; i < fileNameTopLayout.count(); i++)
    {
        //set the enabled state of each item to be the opposite of the checkbox
        fileNameTopLayout.itemAt(i)->widget()->setEnabled(!customNameBox.isChecked());
    }

    //enable the field only if the checkbox is checked
    customNameField.setEnabled(customNameBox.isChecked());
}

void Window::UpdateResolutionLayout()
{
    //enable or disable each widget in the layout (except for the first one and the blank space at the end)
    for (int i = 1; i < limitResolutionLayout.count() - 1; i++)
    {
        //set the enabled state of each item to be the same as the checkbox
        limitResolutionLayout.itemAt(i)->widget()->setEnabled(limitResolutionBox.isChecked());
    }
}

void Window::UpdateMultiThreadLayout()
{
    multiThreadField.setEnabled(multiThreadBox.isChecked());
}

void Window::UpdateExtraArgumentsLayout()
{
    extraArgumentsField.setEnabled(extraArgumentsBox.isChecked());
}

void Window::UpdateAlwaysOnTopSetting()
{

    if (alwaysOnTopBox.isChecked())
    {
        //turn on always on top
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    }
    else
    {
        //turn off always on top
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    }
    show();
}

void Window::ApplyTheme()
{
    QFile file;

    if(darkModeButton.isChecked())
    {
        file.setFileName(":/dark.qss");
    }
    else if (lightModeButton.isChecked())
    {
        file.setFileName(":/light.qss");
    }

    file.open(QFile::ReadOnly);

    QString styleSheet = QLatin1String(file.readAll());

    this->setStyleSheet(styleSheet);

}

void Window::SaveSettings()
{

    //make a json object
    QJsonObject settings;

    //save all the settings into the object
    SaveSettingsToObject(settings);

    //make a json document
    QJsonDocument jsonDoc(settings);

    //save the json object to the file
    QFile file(settingsFileName);
    //open the file in write open mode and text mode
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        //write the data to the file
        QTextStream fout(&file);
        fout << jsonDoc.toJson();
        file.close();
    }
}

void Window::LoadSettings()
{
    QFile file(settingsFileName);

    //make a blank json object
    QJsonObject settings;

    //attempt to open the file
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //make a json document with all the stuff in the file in it
        QByteArray jsonData = file.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

        //attach the newly read json data to the object
        settings = jsonDoc.object();
    }

    LoadSettingsFromObject(settings);

    //apply theme and update all layouts
    ApplyTheme();
    UpdateAlwaysOnTopSetting();
    UpdateTrimLayout();
    UpdateFileNameLayout();
    UpdateResolutionLayout();
    UpdateMultiThreadLayout();
    UpdateExtraArgumentsLayout();


}

void Window::LoadSettingsFromObject(QJsonObject settings)
{
    //set each setting in the program based on what the json object has
    //(if the file doesn't exist, the object will be empty, so all defaults will be applied)

    directoryField.setText(settings["directory"].toString());
    //default is a blank directory field

    //file type options

    if(settings["fileType"] == "mp4")
    {
        mp4Button.setChecked(true);
    }
    else if (settings["fileType"] == "mp3")
    {
        mp3Button.setChecked(true);
    }
    else if ((settings["fileType"]) == "none")
    {
        noneButton.setChecked(true);
    }
    //if setting could not be found, use mp4 as default
    else
    {
        mp4Button.setChecked(true);
    }

    //file name formatting options (default is false)
    channelBox.setChecked(settings["useChannelName"].toBool());
    artistBox.setChecked(settings["useArtistName"].toBool());
    uploadDateBox.setChecked(settings["useUploadDate"].toBool());
    playlistIndexBox.setChecked(settings["usePlaylistIndex"].toBool());

    //limit resolution
    limitResolutionBox.setChecked(settings["limitResolution"].toBool());
    if(limitResolutionBox.isChecked())
    {
        limitResolutionField.setText(settings["maxResolution"].toString());
    }

    //multithreading
    int threadCount = settings["threadCount"].toInt();
    if (threadCount > 1)
    {
        multiThreadBox.setChecked(true);
        multiThreadField.setValue(threadCount);
    }

    if (!settings["extraArguments"].toString().isEmpty())
    {
        extraArgumentsBox.setChecked(true);
        extraArgumentsField.setText(settings["extraArguments"].toString());
    }

    //other options (default is false)
    descriptionBox.setChecked(settings["saveDescription"].toBool());
    saveThumbnailBox.setChecked(settings["saveThumbnail"].toBool());
    vp9Box.setChecked(settings["allowVP9"].toBool());
    embedThumbnailBox.setChecked(settings["embedThumbnail"].toBool());
    addMetadataBox.setChecked(settings["addMetadata"].toBool());
    consoleWindowBox.setChecked(settings["openConsoleWindow"].toBool());

    //window settings
    if(settings["theme"] == "dark")
    {
        darkModeButton.setChecked(true);
    }
    else if(settings["theme"] == "light")
    {
        lightModeButton.setChecked(true);
    }
    else
    {
        darkModeButton.setChecked(true);
    }

    alwaysOnTopBox.setChecked(settings["alwaysOnTop"].toBool());

}

void Window::SaveSettingsToObject(QJsonObject& settings)
{

    //directory
    settings["directory"] = directoryField.text();

    //file type
    if(mp4Button.isChecked())
    {
        settings["fileType"] = "mp4";
    }
    else if (mp3Button.isChecked())
    {
        settings["fileType"] = "mp3";
    }
    else if (noneButton.isChecked())
    {
        settings["fileType"] = "none";
    }

    //file name formatting options
    settings["usePlaylistIndex"] = playlistIndexBox.isChecked();
    settings["useChannelName"] = channelBox.isChecked();
    settings["useArtistName"] = artistBox.isChecked();
    settings["useUploadDate"] = uploadDateBox.isChecked();

    //limit resolution
    settings["limitResolution"] = limitResolutionBox.isChecked();
    if(limitResolutionBox.isChecked())
    {
        settings["maxResolution"] = limitResolutionField.text();
    }

    //multithreading
    settings["threadCount"] = multiThreadBox.isChecked() ? multiThreadField.value() : 1;

    //extra arguments
    settings["extraArguments"] = extraArgumentsBox.isChecked() ? extraArgumentsField.text() : "";

    //other settings
    settings["saveDescription"] = descriptionBox.isChecked();
    settings["saveThumbnail"] = saveThumbnailBox.isChecked();
    settings["allowVP9"] = vp9Box.isChecked();
    settings["embedThumbnail"] = embedThumbnailBox.isChecked();
    settings["addMetadata"] = addMetadataBox.isChecked();
    settings["openConsoleWindow"] = consoleWindowBox.isChecked();

    //window settings
    if(darkModeButton.isChecked())
    {
        settings["theme"] = "dark";
    }
    else if (lightModeButton.isChecked())
    {
        settings["theme"] = "light";
    }

    settings["alwaysOnTop"] = alwaysOnTopBox.isChecked();
}

void Window::SetArguments(QStringList& arguments)
{



    //file type and resolution limit
    if (mp4Button.isChecked())
    {
        //if limit resolution is checked, use the argument that limits resolution
        if(limitResolutionBox.isChecked())
        {
            QString resolution;
            //if a number is in the field, use that; otherwise, use the default of 1080
            if (!limitResolutionField.text().isEmpty())
            {
                resolution = limitResolutionField.text();
            }
            else
            {
                resolution = "1080";
            }

            arguments << "-f"
                      << ("bestvideo[ext=mp4][height<=" + resolution + "]+bestaudio[ext=m4a]/best[ext=mp4]/best");
        }
        //otherwise put the default mp4 argument in
        else
        {
            arguments << "-f" << "bestvideo[ext=mp4]+bestaudio[ext=m4a]/best[ext=mp4]/best";
        }
    }
    else if (mp3Button.isChecked())
    {
        arguments << "-x" << "--audio-format" << "mp3" << "--audio-quality" << "0";
    }
    else if (noneButton.isChecked())
    {
        arguments << "--skip-download";
    }

    //trim settings
    if (trimBox.isChecked())
    {
        arguments << "--download-sections";

        QString startTime;
        QString endTime;

        startTime = startField.text().isEmpty() ? "0:00" : startField.text();
        endTime = endField.text().isEmpty() ? "inf" : endField.text();

        //format the trim argument, for example, "*1:20-1:30"
        arguments << ("*" + startTime + "-" + endTime);

        //this argument is for more accurate trimming
        arguments << "--force-keyframes-at-cuts";

    }

    //file name format settings

    QString fileName;

    //if a custom file name is specified and the box is checked, use that
    if (customNameBox.isChecked())
    {
        fileName = customNameField.text();
    }
    //otherwise, use what other boxes are checked
    else
    {
        //add selected prefixes
        if (playlistIndexBox.isChecked())
        {
            //couldn't get the automatic zero padding working with only using playlist index when available
            //i chose only using playlist index when available and forcing zero padding to 3 digits (001)
            fileName += "%(playlist_index&{0:03d}. |)s";
        }
        if (uploadDateBox.isChecked())
        {
            fileName += "%(upload_date)s ";
        }
        if (channelBox.isChecked())
        {
            fileName += "%(uploader)s - ";
        }
        if (artistBox.isChecked())
        {
            fileName += "%(artist&{} - |)s";
        }

        //always put the title in
        fileName += "%(title)s";

    }

    //put the extension at the end and add the output argument to the list
    arguments << "-o" << (fileName + ".%(ext)s");

    //multithreading
    if(multiThreadBox.isChecked())
    {
        arguments << "-N" << QString::number(multiThreadField.value());
    }

    //other settings
    if (saveThumbnailBox.isChecked())
    {
        arguments << "--write-thumbnail" << "--convert-thumbnails" << "png";
    }

    if (descriptionBox.isChecked())
    {
        arguments << "--print-to-file" << "description" << (fileName + ".txt");
    }

    if (!vp9Box.isChecked())
    {
        arguments << "-S" << "vcodec:!vp09";
    }

    if (embedThumbnailBox.isChecked())
    {
        arguments << "--embed-thumbnail";
    }

    if (addMetadataBox.isChecked())
    {
        arguments << "--add-metadata";
    }

    if (extraArgumentsBox.isChecked())
    {
        //take each "word" from the field and put it into arguments
        arguments << extraArgumentsField.text().split(" ", Qt::SkipEmptyParts);
    }

    //get the url from the url field and add it as the last argument of yt-dlp
    QString url = urlField.text();
    arguments << url;

}
