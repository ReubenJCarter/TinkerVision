#include "NodeEditor.h"

#include <QGridLayout.h>

#include <nodes/FlowScene>
#include <nodes/FlowView>
#include <nodes/FlowViewStyle>
#include <nodes/NodeStyle>
#include <nodes/ConnectionStyle>
#include <nodes/DataModelRegistry>
#include <nodes/Node>
#include <nodes/NodeData>

namespace Visi
{
namespace GUI
{
	
void NodeEditor::SetStyle()
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

NodeEditor::NodeEditor()
{	
	auto RegisterDataModels = []() -> std::shared_ptr<QtNodes::DataModelRegistry> 
	{
		auto ret = std::make_shared<QtNodes::DataModelRegistry>();
		/*
		ret->registerModel<NumberSource>("Sources");
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
	SetStyle();
	flowScene = new QtNodes::FlowScene(RegisterDataModels());
	flowView = new QtNodes::FlowView(flowScene);
	
	layout = new QGridLayout(this); 
	layout->addWidget(flowView);
}

}
}