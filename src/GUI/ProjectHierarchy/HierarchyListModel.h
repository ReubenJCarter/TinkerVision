#pragma once

#include <QAbstractListModel>


class QScrollArea;
class QWidget;
class QVBoxLayout; 

namespace Visi
{
namespace GUI
{
namespace ProjectHierarchy
{
	
class HierarchyListModel: public QAbstractListModel
{
	Q_OBJECT 
	
    public:
        struct Item
        {
            QString name;
            QByteArray flowSceneData; 
        }; 

	private:
        std::vector<Item> items; 
		
	public:
        HierarchyListModel(); 
		int rowCount(const QModelIndex& parent = QModelIndex()) const; 
        int columnCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const; 
        QModelIndex parent(const QModelIndex &index) const;
        
        void Add(QString name, QByteArray flowSceneData);
        void ModifyData(int inx, QByteArray flowSceneData); 
        void Remove(int inx);
        void Clear(); 
};

}	
}
}
