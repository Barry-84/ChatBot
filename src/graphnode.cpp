#include "graphedge.h"
#include "graphnode.h"

GraphNode::GraphNode(int id)
{
    _id = id;
}

GraphNode::~GraphNode()
{
    //// STUDENT CODE
    ////

  	/* Task 0 */
    // delete _chatBot; 

    ////
    //// EOF STUDENT CODE
}

void GraphNode::AddToken(std::string token)
{
    _answers.push_back(token);
}

/* Task 4: This isn't marked for modification by the student, but it seems to me it must be modified. */
//void GraphNode::AddEdgeToParentNode(std::weak_ptr<GraphEdge> edge)
void GraphNode::AddEdgeToParentNode(GraphEdge *edge)
{
    _parentEdges.push_back(edge);
}

/* Task 4: This isn't marked for modification by the student, but it seems to me it must be modified. */
void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge)
//void GraphNode::AddEdgeToChildNode(GraphEdge *edge)
{
    _childEdges.push_back(std::move(edge));// use std::move as push_back amounts to a copy which is not allowed with unique pointers
    //_childEdges.push_back(edge);
}

//// STUDENT CODE
////
void GraphNode::MoveChatbotHere(ChatBot *chatbot)
{
    _chatBot = chatbot;
    _chatBot->SetCurrentNode(this);
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    newNode->MoveChatbotHere(_chatBot);
    _chatBot = nullptr; // invalidate pointer at source
}
////
//// EOF STUDENT CODE

GraphEdge *GraphNode::GetChildEdgeAtIndex(int index)
{
    //// STUDENT CODE
    ////
	/* Task 4 */
    //return _childEdges[index];
    return _childEdges[index].get();

    ////
    //// EOF STUDENT CODE
}