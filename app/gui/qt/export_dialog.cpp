#include <fstream>

#include "export_dialog.h"
#include "audio_tools.h"

ExportDialog::~ExportDialog()
{
    delete heading_label;
    delete subheading_label;
    delete cancel_button;
    delete export_button;
	delete title_input;
	delete desc_input;
}

void ExportDialog::initialise() {
    heading_label = new QLabel();
    subheading_label = new QLabel();
    cancel_button = new QPushButton("&CANCEL", this);
    export_button = new QPushButton(this);

	title_input = new QLineEdit();
	title_input->setPlaceholderText("Title");
	desc_input = new QTextEdit();
	// desc_input->setPlaceholderText("Description");

    layout->addWidget(heading_label, 0, 0, 1, 2);
    layout->addWidget(subheading_label, 1, 0, 1, 2);
	layout->addWidget(title_input, 2, 0, 1, 2);
	layout->addWidget(desc_input, 3, 0, 1, 2);
    layout->addWidget(cancel_button, 4, 0);
    layout->addWidget(export_button, 4, 1);
}

void ExportDialog::connect_listeners() {
    connect(cancel_button, SIGNAL(clicked()), this, SLOT(reject()));
    connect(export_button, SIGNAL(clicked()), this, SLOT(export_file()));
}

int ExportDialog::save() {
    if (title_input->text().isEmpty()) {
        return -1;
    }

    std::string title = title_input->text().toUtf8().constData();
    std::string filename = title;
    std::replace(filename.begin(), filename.end(), ' ', '_');

    std::string desc = desc_input->toPlainText().toUtf8().constData();
    std::string filepath = save_dir + filename;

    std::string json_data = "{"
        "\"title\": \"" + title + "\","
        "\"description\": \"" + desc + "\""
    "}";

    save_to_file(filepath + std::string(".spi"), file_contents);
    save_to_file(filepath + std::string(".json"), json_data);
    AudioTools::convert_wav_to_ogg(SAMPLE_TMP_PATH,
                                   filepath + std::string(".ogg"));

	return 0;
}

int ExportDialog::save_to_file(std::string filepath, std::string file_contents) {
    std::ofstream file(filepath.c_str());
    file << file_contents;
    file.close();

    return 0;
}

/**
 * Slots
 */

int ExportDialog::export_file() {
	return 0;
};

/**
 * Public methods
 */

void ExportDialog::set_file_contents(std::string contents) {
    file_contents = contents;
}
