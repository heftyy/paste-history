#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileSystemWatcher>

#include <Config/PasteHistoryConfig.h>

#include <PasteHistoryWindow/PasteHistoryWindow.h>

static void LoadStylesheet(QApplication& app, const QString& filepath)
{
	QFile stylesheet_file(filepath);
	if (!stylesheet_file.exists())
	{
		qWarning() << "Unable to set stylesheet, file not found\n";
	}
	else
	{
		stylesheet_file.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&stylesheet_file);
		QString output = ts.readAll();
		app.setStyleSheet(output);
	}
}

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	QApplication::setApplicationName("Paste history");
	QApplication::setApplicationVersion(
	    QString("%1.%2.%3").arg(PasteHistory_VERSION_MAJOR).arg(PasteHistory_VERSION_MINOR).arg(PasteHistory_VERSION_PATCH));

	QFileSystemWatcher file_watcher({"src/resources/dark_theme.qss"}, &app);
	app.connect(&file_watcher, &QFileSystemWatcher::fileChanged, [&app](const QString& path) { LoadStylesheet(app, path); });

	LoadStylesheet(app, ":/styles/dark_theme.qss");

	PasteHistoryWindow paste_history_window(app.activeWindow());
	paste_history_window.Start();

	return app.exec();
}