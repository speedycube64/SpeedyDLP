#include "window.h"

#define DEBUG

//constructor (idk why a parameter has to be sent in)
Window::Window(QWidget *parent) : QWidget{parent}
{

    //----------------LOAD SETTINGS-----------------

    LoadSettings();

    //----------------MAIN LAYOUT-----------------

    //attach the main layout to the window
    setLayout(&mainLayout);

    //add the main components in order to the main layout
    mainLayout.addLayout(&urlLayout);
    mainLayout.addLayout(&directoryLayout);
    mainLayout.addLayout(&fileTypeLayout);
    mainLayout.addLayout(&fileNameBigLayout);
    mainLayout.addLayout(&trimLayout);
    mainLayout.addLayout(&limitResolutionLayout);
    mainLayout.addLayout(&miscOptionLayout);
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
    customNameBox.setToolTip("If this option is selected, all above options will be ignored.\n\
                              You may also use yt-dlp arguments here.");

    //connect the checkbox getting checked or unchecked to updating the layout
    connect(&customNameBox, &QCheckBox::stateChanged, this, &Window::UpdateFileNameLayout);

    //----------------TRIM VIDEO LAYOUT----------------

    trimLayout.addWidget(&trimBox);
    trimLayout.addWidget(&startLabel);
    trimLayout.addWidget(&startField);
    trimLayout.addWidget(&endLabel);
    trimLayout.addWidget(&endField);

    trimBox.setText("Trim video:");
    trimBox.setToolTip("Requires FFmpeg.\nYou can leave fields blank to keep the original start and end.");

    startLabel.setText("Start:");
    startField.setPlaceholderText("XX:XX");
    endLabel.setText("End:");
    endField.setPlaceholderText("XX:XX");

    //update the layout on startup so the stuff is disabled
    UpdateTrimLayout();

    //connect the checkbox getting checked or unchecked to updating the layout
    connect(&trimBox, &QCheckBox::stateChanged, this, &Window::UpdateTrimLayout);

    //----------------LIMIT RESOLUTION LAYOUT----------------

    limitResolutionLayout.addWidget(&limitResolutionBox);
    limitResolutionLayout.addWidget(&limitResolutionField);
    limitResolutionLayout.addWidget(&pLabel);

    limitResolutionLayout.addStretch();

    limitResolutionBox.setText("Limit resolution:");
    limitResolutionBox.setToolTip("If checked, the height of the video will be limited to the number of pixels entered.");

    pLabel.setText("p");

    limitResolutionField.setValidator(new QIntValidator());
    limitResolutionField.setFixedWidth(35);
    limitResolutionField.setPlaceholderText("1080");

    //update the layout on startup so the stuff is disabled
    UpdateResolutionLayout();

    //connect the checkbox getting checked or unchecked to updating the layout
    connect(&limitResolutionBox, &QCheckBox::stateChanged, this, &Window::UpdateResolutionLayout);

    //----------------MISC OPTION LAYOUT----------------

    //add each item to its appropriate spot

    //left column
    miscOptionLayout.addWidget(&miscOptionLabel, 0, 0); //row 0 column 0
    miscOptionLayout.addWidget(&saveThumbnailBox, 1, 0);
    miscOptionLayout.addWidget(&embedThumbnailBox, 2, 0);
    miscOptionLayout.addWidget(&addMetadataBox, 3, 0);

    //right column
    miscOptionLayout.addWidget(&descriptionBox, 1, 1);
    miscOptionLayout.addWidget(&vp9Box, 2, 1);

    //add blank space on the right
    miscOptionLayout.setColumnStretch(2, 1);

    miscOptionLabel.setText("Other options:");

    saveThumbnailBox.setText("Save thumbnail as PNG");
    saveThumbnailBox.setToolTip("The image will have the same name as the downloaded video.");

    embedThumbnailBox.setText("Embed thumbnail");
    embedThumbnailBox.setToolTip("The video thumbnail in File Explorer will match that on YouTube.");

    descriptionBox.setText("Save description as TXT");
    descriptionBox.setToolTip("The text document will have the same name as the downloaded video.");

    vp9Box.setText("Allow VP9 codec");
    vp9Box.setToolTip("This codec has a better bitrate, but it is incompatible with some editing programs and video players.");

    addMetadataBox.setText("Add metadata");
    addMetadataBox.setToolTip("Any relevant data will be added to file properties.");

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

    //set the parameters of the process
    process.setWorkingDirectory (directory);
    process.setProgram("yt-dlp.exe");
    process.setArguments(arguments);

    //run the process
    process.startDetached();

}

void Window::SetArguments(QStringList& arguments)
{
    //file type and resolution limit
    if (mp4Button.isChecked())
    {
        //if limit resolution is checked and a number is in the field,
        //use the argument that limits resolution
        if(limitResolutionBox.isChecked() && !limitResolutionField.text().isEmpty())
        {
            arguments << "-f"
            << ("bestvideo[ext=mp4][height<=" + limitResolutionField.text() + "]+bestaudio[ext=m4a]/best[ext=mp4]/best");
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

    //get the url from the url field and add it as the last argument
    QString url = urlField.text();
    arguments << url;
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
    for (int i = 1; i < trimLayout.count(); i++)
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
    addMetadataBox.setChecked(settings["useUploadDate"].toBool());
    playlistIndexBox.setChecked(settings["usePlaylistIndex"].toBool());

    //limit resolution
    limitResolutionBox.setChecked(settings["limitResolution"].toBool());
    limitResolutionField.setText(settings["maxResolution"].toString());

    //other options (default is false)
    descriptionBox.setChecked(settings["saveDescription"].toBool());
    saveThumbnailBox.setChecked(settings["saveThumbnail"].toBool());
    vp9Box.setChecked(settings["allowVP9"].toBool());
    embedThumbnailBox.setChecked(settings["embedThumbnail"].toBool());
    addMetadataBox.setChecked(settings["addMetadata"].toBool());

    //i don't see why anyone would want the trim video option to be saved but maybe that's just me
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
    settings["maxResolution"] = limitResolutionField.text();

    //other settings
    settings["saveDescription"] = descriptionBox.isChecked();
    settings["saveThumbnail"] = saveThumbnailBox.isChecked();
    settings["allowVP9"] = vp9Box.isChecked();
    settings["embedThumbnail"] = embedThumbnailBox.isChecked();
    settings["addMetadata"] = addMetadataBox.isChecked();

}
