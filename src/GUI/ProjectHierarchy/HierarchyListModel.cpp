#include "HierarchyListModel.h"

#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>

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

int HierarchyListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
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

QModelIndex HierarchyListModel::parent(const QModelIndex &index) const
{
    return QModelIndex(); 
}

void HierarchyListModel::Add(QString name, QByteArray flowSceneData)
{
    Item it;
    it.name = name; 
    it.flowSceneData = flowSceneData; 
    items.push_back(it); 
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