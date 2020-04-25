#include "HierarchyListModel.h"

#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>

#include <iostream> 

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
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDropEnabled;
    
}

bool HierarchyListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    std::cout << "removeRows:" << row << " " << count << "\n"; 
    if (row < 0 || row > items.size())
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
    return Qt::MoveAction; //Qt::CopyAction | 
}

bool HierarchyListModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    return false; 
}   

void HierarchyListModel::AddNew(QString name)
{
    Item it;
    it.name = name; 
    items.push_back(it); 
    emit dataChanged(QModelIndex(), QModelIndex(), { Qt::DisplayRole } ); 
}

void HierarchyListModel::ModifyData(int inx, QByteArray flowSceneData)
{
    items[inx].flowSceneData = flowSceneData; 
}

void HierarchyListModel::Remove(int inx)
{
    items.erase(items.begin() + inx);
}

void HierarchyListModel::Clear()
{
    items.clear(); 
}


}	
}
}