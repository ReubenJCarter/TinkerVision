#include "HierarchyListModel.h"

#include "Core/SerializedObject.h"

#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>
#include <QMimeData>

#include <iostream> 

//https://forum.qt.io/topic/104450/how-to-implement-simple-internal-drag-drop-for-reordering-items-in-qlistview-using-a-custom-model/6
//https://forum.qt.io/topic/90560/qtreeview-qabstractitemmodel-and-internal-move/5
//https://stackoverflow.com/questions/56819085/qt-how-to-implement-simple-internal-dragdrop-for-reordering-items-in-qlistview

namespace Visi
{
namespace GUI
{
namespace ProjectHierarchy
{
	
HierarchyListModel::HierarchyListModel()
{

}

int HierarchyListModel::rowCount(const QModelIndex& parent) const
{
    return items.size(); 
}

QVariant HierarchyListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= items.size())
        return QVariant();

    if(role == Qt::DisplayRole)
        return QVariant(items[ index.row() ].name);
    else 
        return QVariant();
    
}

bool HierarchyListModel::setData(const QModelIndex &index, const QVariant &value, int role )
{

    if (index.row() < 0 || index.row() >= items.size())
        return false;

    if(role != Qt::EditRole)
        return false; 

    if(value.toString().length() == 0)
        return false;

    items[ index.row() ].name = value.toString();  

    emit dataChanged(index, index, { role } );
    return true; 
}

QModelIndex HierarchyListModel::parent(const QModelIndex &index) const
{
    return QModelIndex(); 
}

Qt::ItemFlags HierarchyListModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDropEnabled;
    
}

bool HierarchyListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    std::cout << "removeRows:" << row << " " << count << "\n"; 
    if (row < 0 || (row + count) > items.size())
        return false;

    beginRemoveRows(parent, row, row+count-1);

    
    items.erase (items.begin() + row, items.begin() + row + count);

    endRemoveRows(); 

    //emit dataChanged(QModelIndex(), QModelIndex(), { Qt::DisplayRole } ); 
    
    return true; 
}

bool HierarchyListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    std::cout << "insertRows:" << row << " " << count << "\n"; 
    if (row < 0 || row > items.size())
        return false;
    

    beginInsertRows(parent, row, row+count-1);

    Item item;
    items.insert(items.begin() + row, count, item); 
    
    endInsertRows(); 

    return true; 
}


Qt::DropActions HierarchyListModel::supportedDragActions() const
{
    return Qt::MoveAction;// | Qt::CopyAction; 
}

Qt::DropActions HierarchyListModel::supportedDropActions() const
{
    return Qt::MoveAction;// | Qt::CopyAction;
}

bool HierarchyListModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;
    else if (action  != Qt::MoveAction)
        return false;

    QByteArray encodedData = data->data("EditorGraphMimeData");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    std::vector<Item> newItems; 
    while (!stream.atEnd()) {
        
        Item it;
        stream >> it.name;
        stream >> it.flowSceneData;
        newItems.push_back(it); 
    }

    insertRows(row, newItems.size(), QModelIndex());
    for(int i = 0; i < newItems.size(); i++)
    {
        items[row + i] = newItems[i]; 
    }
    emit dataChanged(QModelIndex(), QModelIndex(), { Qt::DisplayRole } ); 
    return true;
}   

QMimeData* HierarchyListModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData* mimeData = new QMimeData;
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    for (const QModelIndex &index : indexes) 
    {
        if (index.isValid()) 
        {
            stream << items[ index.row() ].name << items[ index.row() ].flowSceneData;
        }
    }
    mimeData->setData("EditorGraphMimeData", encodedData);
    return mimeData;

}

QStringList HierarchyListModel::mimeTypes() const
{
    QStringList types;
    types << "EditorGraphMimeData";
    return types;
}





int HierarchyListModel::AddNew(QString name)
{
    Item it;
    it.name = name; 
    items.push_back(it); 
    emit dataChanged(QModelIndex(), QModelIndex(), { Qt::DisplayRole } ); 
    return items.size()-1; 
}

void HierarchyListModel::ModifyData(int inx, QByteArray flowSceneData)
{
    if(inx < 0 || inx >= items.size())
        return; 
    items[inx].flowSceneData = flowSceneData; 
}

int HierarchyListModel::GetInxFromName(QString name)
{
    for(int i = 0; i < items.size(); i++)
    {
        if(items[i].name == name)
            return i;
    }
    return -1;
}

QByteArray HierarchyListModel::GetData(int inx)
{
    if(inx < 0 || inx >= items.size())
        return QByteArray(); 
    return items[inx].flowSceneData;
}

QString HierarchyListModel::GetName(int inx)
{
    if(inx < 0 || inx >= items.size())
        return ""; 
    return items[inx].name;
}

void HierarchyListModel::Remove(int inx)
{
    if(inx < 0 || inx >= items.size())
        return; 

    items.erase(items.begin() + inx);
    emit dataChanged(QModelIndex(), QModelIndex(), { Qt::DisplayRole } ); 
}

void HierarchyListModel::Remove(std::vector<int>& inxs)
{
    std::vector<int> inxCopy = inxs; 
    std::sort(inxCopy.begin(), inxCopy.end()); 
    bool dataDidChange = false; 
    for(int i = inxCopy.size()-1; i>=0; i--)
    {
        if(i < 0 || i >= items.size())
            continue; 
        int inx = inxCopy[i]; 
        items.erase(items.begin() + inx);
        dataDidChange = true; 
    }

    if(dataDidChange)
        emit dataChanged(QModelIndex(), QModelIndex(), { Qt::DisplayRole } ); 
}

int HierarchyListModel::GetCount()
{
    return items.size(); 
}

void HierarchyListModel::Clear()
{
    items.clear(); 

    emit dataChanged(QModelIndex(), QModelIndex(), { Qt::DisplayRole } ); 
}

void HierarchyListModel::Serialize(SerializedObject* so)
{
    std::vector<SerializedObject*> graphsSOs; 
    for(int i = 0 ; i < items.size(); i++)
    {
        std::string name = items[i].name.toStdString(); 
        std::string flowSceneData = QString(items[i].flowSceneData).toStdString(); 

        SerializedObject* so = new SerializedObject; 
        so->SetString("name", name); 
        so->SetString("graph", flowSceneData); 
        graphsSOs.push_back(so); 
    }
    so->SetSerializedObjectArray("graphItems", graphsSOs);
}

void HierarchyListModel::Deserialize(SerializedObject* so)
{
    items.clear(); 

    std::vector<SerializedObject*> graphsSOs; 
    so->GetSerializedObjectArray("graphItems", graphsSOs); 
    for(int i = 0; i < graphsSOs.size(); i++)
    {
        Item it; 
        it.name = QString( graphsSOs[i]->GetString("name").c_str() ); 
        it.flowSceneData = QByteArray( graphsSOs[i]->GetString("graph").c_str() ); 
        items.push_back(it); 
    }

    emit dataChanged(QModelIndex(), QModelIndex(), { Qt::DisplayRole } ); 
}

}	
}
}