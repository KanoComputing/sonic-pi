#include <fstream>
#include <iostream>
#include <ctime>
#include <string>

#include "share_dialog.h"

void ShareDialog::initialise() {
    this->setWindowTitle("Share");
    export_button->setText("SHARE");
    heading_label->setText("Share");
    subheading_label->setText("Share your project with the world");
}

/**
 * Public
 */

int ShareDialog::open_external_dialog() {
    this->setCursor(QCursor(Qt::WaitCursor));

    // Get a timestamp date, e.g. 2016-10-14--9:41
    QString date = getFormatedDate();

    // Set the date as the title of the share.
    title_input->setText(date);

    // Perform a save with the date as the filenames.
    if (save() == -1) {
        this->setCursor(QCursor(Qt::ArrowCursor));
        return -1;
    }

    // Share
    FILE *share_proc;

    std::string filename = title_input->text().toUtf8().constData();
    std::replace(filename.begin(), filename.end(), ' ', '_');
    std::string filepath = save_dir + filename + std::string(".spi");

    std::string cmd = std::string("/usr/bin/kano-share-container ") + filepath
                    + std::string(" --app-title make-music");
    if (!(share_proc = popen(cmd.c_str(), "r"))) {
        this->setCursor(QCursor(Qt::ArrowCursor));
        return -1;
    }

    // Read STDOUT
    char buff[512];
    std::string std_out;

    while (std::fgets(buff, sizeof(buff), share_proc) != NULL) {
        std_out += buff;
    }

    int rc = pclose(share_proc);
    int status = WEXITSTATUS(rc);


    if (status != 0) {
        // Sharing failed.
        std::cout << "\nShare failed: " << std_out;
        this->setCursor(QCursor(Qt::ArrowCursor));
        return -1;
    }

    /**
     * Increment share count in Kano Profile
     * Don't worry about if it succeeds
     */
    FILE *profile_proc;

    cmd = std::string("/usr/bin/kano-profile-cli "
                      "increment_app_state_variable make-music shared 1");
    profile_proc = popen(cmd.c_str(), "r");
    pclose(profile_proc);

    this->accept();
    return 0;
}

/**
 * Slots
 */

int ShareDialog::export_file() {
    // Moved to open_external_dialog.
    return 0;
}

/**
 * Private
 */

QString ShareDialog::getFormatedDate() const {
    time_t t = time(0);
    struct tm *now = localtime(&t);

    QString date = QString::number(now->tm_year + 1900) + "-"
                 + QString::number(now->tm_mon + 1) + "-"
                 + QString::number(now->tm_mday) + "--"
                 + QString::number(now->tm_hour) + ":"
                 + QString::number(now->tm_min) + ":"
                 + QString::number(now->tm_sec);

    return date;
}
