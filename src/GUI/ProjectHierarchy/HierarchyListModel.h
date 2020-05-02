#pragma once

#include <QAbstractListModel>

class QScrollArea;
class QWidget;
class QVBoxLayout; 

namespace Visi
{

class SerializedObject; 

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
        Qt::DropActions supportedDropActions() const;
        bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
        QMimeData* mimeData(const QModelIndexList &indexes) const;
        QStringList mimeTypes() const;

        
        int AddNew(QString name);
        void ModifyData(int inx, QByteArray flowSceneData); 
        int GetInxFromName(QString name); 
        QByteArray GetData(int inx); 
        QString GetName(int inx); 
        void Remove(int inx);
        void Remove(std::vector<int>& inxs);
        int GetCount(); 
        void Clear(); 

        void Serialize(SerializedObject* so); 
		void Deserialize(SerializedObject* so); 
};

}	
}
}
