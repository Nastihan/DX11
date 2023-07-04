#pragma once
class ModelProbe
{
public:
	virtual bool RenderNodeTree(class Node& node) = 0;
	virtual void PopTreeNode(class Node& node) = 0;
};