#pragma once

#include <QAbstractListModel>

#include "ComputeGraph/SerializedObject.h"


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
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const; 
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole); 
        QModelIndex parent(const QModelIndex &index) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;

        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
        bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

        Qt::DropActions supportedDragActions() const; 
        bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

        
        void AddNew(QString name);
        void ModifyData(int inx, QByteArray flowSceneData); 
        void Remove(int inx);
        void Clear(); 
};

}	
}
}
