#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <fstream>
#include <iostream>
#include <QDebug>

#include "load_source_dialog.h"

LoadSourceDialog::~LoadSourceDialog()
{
    delete heading_label;
    delete subheading_label;
    delete local_button;
    delete remote_button;
}

void LoadSourceDialog::initialise() {
    this->setWindowTitle("Load");
    heading_label = new QLabel("Load from where?");
    subheading_label = new QLabel("Do you want to load from your Kano or the Internet?");
    local_button = new QPushButton("&YOUR KANO", this);
    remote_button = new QPushButton("&INTERNET", this);

    layout->addWidget(heading_label, 0, 0, 1, 2);
    layout->addWidget(subheading_label, 1, 0, 1, 2);
    layout->addWidget(local_button, 2, 0);
    layout->addWidget(remote_button, 2, 1);
}

void LoadSourceDialog::connect_listeners() {
    connect(local_button, SIGNAL(clicked()), this, SLOT(load_from_local()));
    connect(remote_button, SIGNAL(clicked()), this, SLOT(load_from_internet()));
}

int LoadSourceDialog::read_file(std::string filename) {
    file_contents = "";

    QFile file(QString::fromStdString(filename));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return -1;
    }

    QTextStream stream(&file);
    stream.setCodec("UTF-8");

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        file_contents += line.toStdString();
        file_contents += '\n';
    }

    file.close();

    return 0;
}

/**
 * Slots
 */

int LoadSourceDialog::load_from_local() {
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Load from your Kano"),
        QString::fromStdString(save_dir),
        tr("Sonic Pi Files (*.spi)"));

    if (filename.isEmpty()) {
        return -1;
    }

    read_file(filename.toStdString());

    this->accept();
    return 0;
}

int LoadSourceDialog::load_from_internet() {
    this->setCursor(QCursor(Qt::WaitCursor));

    FILE * share_proc;
    char buff[512];

    if (!(share_proc = popen("/usr/bin/kano-share make-music", "r"))) {
        return -1;
    }

    // Read STDOUT
    std::string std_out;

    while (std::fgets(buff, sizeof(buff), share_proc) != NULL) {
        std_out += buff;
    }

    pclose(share_proc);

    // Find the returned file path
    std::string file_path_str = "File Path: ";
    int start = std_out.find(file_path_str) + file_path_str.length();

    int end = std_out.find('\n', start);

    if (start == -1
        || end == -1) {

        // No file returned
        this->setCursor(QCursor(Qt::ArrowCursor));
        return -1;
    }

    std::string file_path = std_out.substr(start, end - start);
    std::cout << file_path;

    // Read file and clean up
    read_file(file_path);

    this->accept();
    return 0;
}

/**
 * Public methods
 */

std::string LoadSourceDialog::get_file_contents() {
    return file_contents;
}
