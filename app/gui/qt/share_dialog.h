#ifndef SHARE_DIALOG_H
#define SHARE_DIALOG_H

#include "export_dialog.h"

class ShareDialog : public ExportDialog
{
	Q_OBJECT

	public:
        ShareDialog(QWidget * parent = 0,
                    Qt::WindowFlags f = 0) :
            ExportDialog(parent, f) {
				initialise();
            };

        int open_external_dialog();

    protected slots:
	    int export_file();

	protected:
		void initialise();

    private:
        QString getFormatedDate() const;
};

#endif
