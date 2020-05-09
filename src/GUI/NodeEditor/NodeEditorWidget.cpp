#include "NodeEditorWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include <QOpenGLWidget>
#include <QtOpenGL/QGL>


#include <nodes/FlowScene>
#include <nodes/FlowView>
#include <nodes/FlowViewStyle>
#include <nodes/NodeStyle>
#include <nodes/ConnectionStyle>
#include <nodes/DataModelRegistry>
#include <nodes/Node>
#include <nodes/NodeData>

#include "BaseNode.h"

#include "Nodes/ImageNodes.h"
#include "Nodes/SourceNodes.h"
#include "Nodes/MiscNodes.h"
#include "Nodes/ProcessNodes.h"

#include "ComputeGraph/Graph.h"
#include "ComputeGraph/Node.h"


namespace Viso
{
namespace GUI
{
namespace NodeEditor
{
	
void NodeEditorWidget::SetStyle(bool darkMode)
{
	if(!darkMode)
	{
		QtNodes::FlowViewStyle::setStyle(
		R"(
		{
		"FlowViewStyle": {
		"BackgroundColor": [255, 255, 240],
		"FineGridColor": [245, 245, 230],
		"CoarseGridColor": [235, 235, 220]
		}
		}
		)");

		QtNodes::NodeStyle::setNodeStyle(
		R"(
		{
		"NodeStyle": {
		"NormalBoundaryColor": "darkgray",
		"SelectedBoundaryColor": "deepskyblue",
		"GradientColor0": "mintcream",
		"GradientColor1": "mintcream",
		"GradientColor2": "mintcream",
		"GradientColor3": "mintcream",
		"ShadowColor": [255, 255, 255],
		"FontColor": [10, 10, 10],
		"FontColorFaded": [100, 100, 100],
		"ConnectionPointColor": "white",
		"ErrorColor": "white",
		"PenWidth": 2.0,
		"HoveredPenWidth": 2.5,
		"ConnectionPointDiameter": 10.0,
		"Opacity": 1.0
		}
		}
		)");

		QtNodes::ConnectionStyle::setConnectionStyle(
		R"(
		{
		"ConnectionStyle": {
		"ConstructionColor": "gray",
		"NormalColor": "black",
		"SelectedColor": "gray",
		"SelectedHaloColor": "deepskyblue",
		"HoveredColor": "deepskyblue",

		"LineWidth": 3.0,
		"ConstructionLineWidth": 2.0,
		"PointDiameter": 10.0,

		"UseDataDefinedColors": true
		}
		}
		)");
	}
	else
	{
		//Darkest 19232d [7, 17, 26] //put this in paleton...
		//Geryish 2b343c [43, 52, 60]
		//CoarseGridColor0: [43, 52, 60]
		//CoarseGridColor1: [50, 65, 70]
		//CoarseGridColor2: [25, 35, 45]
		QtNodes::FlowViewStyle::setStyle(
		R"(
		{
		"FlowViewStyle": {
		"BackgroundColor": [7, 17, 26],
		"FineGridColor": [9, 22, 33],
		"CoarseGridColor": [25, 35, 45]
		}
		}
		)");

		QtNodes::NodeStyle::setNodeStyle(
		R"(
		{
		"NodeStyle": {
		"NormalBoundaryColor": [50, 65, 75],
		"SelectedBoundaryColor": "deepskyblue",
		"GradientColor0": [50, 65, 75],
		"GradientColor1": [50, 65, 75],
		"GradientColor2": [50, 65, 75],
		"GradientColor3": [50, 65, 75],
		"ShadowColor": [0, 0, 0, 0],
		"FontColor": "white",
		"FontColorFaded": "gray",
		"ConnectionPointColor": [169, 169, 169],
		"FilledConnectionPointColor": "cyan",
		"ErrorColor": [232, 76, 61],
		"WarningColor": [128, 128, 0],
		"PenWidth": 1.0,
		"HoveredPenWidth": 1.5,
		"ConnectionPointDiameter": 8.0,
		"Opacity": 1.0
		}
		}
		)");

		QtNodes::ConnectionStyle::setConnectionStyle(
		R"(
		{
		"ConnectionStyle": {
		"ConstructionColor": "gray",
		"NormalColor": "darkcyan",
		"SelectedColor": [100, 100, 100],
		"SelectedHaloColor": "orange",
		"HoveredColor": "lightcyan",

		"LineWidth": 3.0,
		"ConstructionLineWidth": 2.0,
		"PointDiameter": 10.0,

		"UseDataDefinedColors": false
		}
		}
		)");
		
	}
}

NodeEditorWidget::NodeEditorWidget()
{	
	auto RegisterDataModels = []() -> std::shared_ptr<QtNodes::DataModelRegistry> 
	{
		auto ret = std::make_shared<QtNodes::DataModelRegistry>();
		
		//Register Nodes
		ret->registerModel<Nodes::GraphInputNode>("Graph"); 
		ret->registerModel<Nodes::GraphOutputNode>("Graph");

		ret->registerModel<Nodes::IntSource>("Sources");
		ret->registerModel<Nodes::FloatSource>("Sources");
		ret->registerModel<Nodes::BoolSource>("Sources");
		ret->registerModel<Nodes::StringSource>("Sources");
		ret->registerModel<Nodes::ImageSource>("Sources");
		ret->registerModel<Nodes::ImageGPUSource>("Sources");
		ret->registerModel<Nodes::ImageTypeSource>("Sources");
			 
		ret->registerModel<Nodes::ImageAllocate>("Image");
		ret->registerModel<Nodes::ImageDeallocate>("Image");
		ret->registerModel<Nodes::ImageGetDims>("Image");

		ret->registerModel<Nodes::AdaptiveThreshold>("Processes"); 
		ret->registerModel<Nodes::ApproxDistanceTransform>("Processes"); 
		ret->registerModel<Nodes::AverageFilter>("Processes"); 
		ret->registerModel<Nodes::Blend>("Processes"); 
		ret->registerModel<Nodes::BrightnessContrast>("Processes"); 
		ret->registerModel<Nodes::CameraDistortion>("Processes"); 
		ret->registerModel<Nodes::ChannelMapper>("Processes"); 
		ret->registerModel<Nodes::ClearColor>("Processes"); 
		ret->registerModel<Nodes::CopyImage>("Processes"); 

		return ret; 
	};
	SetStyle(true);
	flowScene = new QtNodes::FlowScene(RegisterDataModels());

	flowView = new QtNodes::FlowView(flowScene);

	//flow view update and rendering modes
	//flowView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	//flowView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

	//remove shadows on all nodes created to speed up node editor 
	connect(flowScene, &QtNodes::FlowScene::nodeCreated, [](QtNodes::Node &n){ n.nodeGraphicsObject().setGraphicsEffect(nullptr); }); 
	
	QFont font; 
	font.setPointSize(16); 
	font.setBold(true); 
	nameLabel = new QLabel("Graph Name"); 
  	nameLabel->setFont( font );

	layout = new QVBoxLayout(this); 

	topBarLayout = new QHBoxLayout; 
	layout->addLayout(topBarLayout); 
	topBarLayout->addWidget(nameLabel); 
	
	layout->addWidget(flowView);
}

void NodeEditorWidget::Load(QByteArray d, QString name)
{
	flowScene->clearScene();
	flowScene->loadFromMemory(d); 
	nameLabel->setText(name); 
}

QByteArray NodeEditorWidget::Save()
{
	return flowScene->saveToMemory();
}

void NodeEditorWidget::Clear()
{
	flowScene->clearScene();
}

void NodeEditorWidget::SerializeToComputeGraph(SerializedObject* so)
{
	//retrieve all the connection an nodes info from the scene 
	std::vector<QtNodes::Node*> sceneNodes = flowScene->allNodes(); 
	std::vector<QtNodes::Connection*> sceneConnections;
	for( auto itt = flowScene->connections().begin(); itt !=  flowScene->connections().end(); itt++ )
	{
		std::shared_ptr<QtNodes::Connection> c = itt->second; 
		sceneConnections.push_back(c.get()); 
	}

	//Create the compute graph
	ComputeGraph::Graph graph; 

	//Add all the nodes to the compute graph and create Map gui IDs for nodes to compute nodes
	std::map<QUuid, ComputeGraph::Node*> guiIdToComputeNode; 
	for(int i = 0; i < sceneNodes.size(); i++)
	{
		//Get the base node, uid, compute node 
		BaseNode* bn = (BaseNode*)(sceneNodes[i]->nodeDataModel());
		QUuid uid = sceneNodes[i]->id(); 
		ComputeGraph::Node* cn = bn->GetComputeNode(); 

		//Test is this node a graph input node
		if( bn->name() != QString("GraphInput") &&
			bn->name() != QString("GraphOutput") &&
			cn != NULL)
		{
			guiIdToComputeNode[uid] = cn; 
			graph.AddNode( cn );
		}
	}

	//cycle over all the connections
	

	
}

}
}
}