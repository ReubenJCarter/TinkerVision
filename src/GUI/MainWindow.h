#pragma once

#include <QtWidgets/QMainWindow>

class QMenu;
class QAction; 

namespace Visi
{
namespace GUI
{

class NodeEditor;

class MainWindow: public QMainWindow
{
	Q_OBJECT
	public:
		QMenu* fileMenu;
		QAction* saveAction;
		QAction* loadAction;
		NodeEditor* nodeEditor; 
		
		MainWindow();
		void ExpandToFitScreen();
		
	public slots:
		void Save();
		void Load();
};

}
}