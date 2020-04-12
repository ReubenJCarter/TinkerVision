#include "NodeEditorWidget.h"

#include <QGridLayout.h>

#include <nodes/FlowScene>
#include <nodes/FlowView>
#include <nodes/FlowViewStyle>
#include <nodes/NodeStyle>
#include <nodes/ConnectionStyle>
#include <nodes/DataModelRegistry>
#include <nodes/Node>
#include <nodes/NodeData>

#include "Nodes/ImageNodes.h"
#include "Nodes/SourceNodes.h"

namespace Visi
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
		
	}
}

NodeEditorWidget::NodeEditorWidget()
{	
	auto RegisterDataModels = []() -> std::shared_ptr<QtNodes::DataModelRegistry> 
	{
		auto ret = std::make_shared<QtNodes::DataModelRegistry>();
		
		ret->registerModel<Nodes::IntSource>("Sources");
		/*
		ret->registerModel<TextSource>("Sources");
		
		ret->registerModel<TextDisplay>("Sinks");
		
		ret->registerModel<Add>("Arithmetic"); 
		ret->registerModel<Subtract>("Arithmetic"); 
		ret->registerModel<Multiply>("Arithmetic"); 
		ret->registerModel<Divide>("Arithmetic"); 
		
		ret->registerModel<Sqrt>("Maths"); 
		
		ret->registerModel<AdaptiveThreshold>("ImageProcess"); 
		ret->registerModel<Blend>("ImageProcess"); 
		ret->registerModel<BrightnessContrast>("ImageProcess"); 
		ret->registerModel<ChannelDemux>("ImageProcess"); 
		ret->registerModel<GaussianBlur>("ImageProcess"); 
		ret->registerModel<GrayScale>("ImageProcess"); 
		
		ret->registerTypeConverter(std::make_pair(NumberData().type(), TextData().type()), QtNodes::TypeConverter{NumberToTextConverter()});
		*/					 
		return ret; 
	};
	SetStyle(true);
	flowScene = new QtNodes::FlowScene(RegisterDataModels());

	flowView = new QtNodes::FlowView(flowScene);

	//flow view speed improvments
	flowView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	flowView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	
	layout = new QGridLayout(this); 
	layout->addWidget(flowView);
}

}
}
}