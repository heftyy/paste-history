#include <QApplication>
#include <QDebug>
#include <QFile>

#include <Config/PasteHistoryConfig.h>

#include <PasteHistory/PasteHistoryWindow.h>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	QApplication::setApplicationName("Paste history");
	QApplication::setApplicationVersion(QString("%1.%2.%3").arg(PasteHistory_VERSION_MAJOR).arg(PasteHistory_VERSION_MINOR).arg(PasteHistory_VERSION_PATCH));

	QFile style_sheet(":style/style.qss");
	if (!style_sheet.exists())
	{
		qWarning() << "Unable to set stylesheet, file not found\n";
	}
	else
	{
		style_sheet.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&style_sheet);
		QString output = ts.readAll();
		app.setStyleSheet(output);
	}

	PasteHistoryWindow paste_history_window(app.activeWindow());
	paste_history_window.Start();

	return app.exec();
}