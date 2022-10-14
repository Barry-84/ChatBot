    #ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <vector>
#include <string>
#include "chatbot.h"

/* Task 4 */
#include <memory>


// forward declarations
class GraphEdge;

class GraphNode
{
private:
    //// STUDENT CODE
    ////

    // data handles (owned)
    /* Task 4: change the ownership of all instances of GraphEdge in a way
       such that each instance of GraphNode exclusively owns the outgoing
       GraphEdges ('outgoing' refers to the child edges) */
    //std::vector<GraphEdge *> _childEdges;  // edges to subsequent nodes
    std::vector<std::unique_ptr<GraphEdge>> _childEdges;  // edges to subsequent nodes

    // data handles (not owned)
    /* Task 4: ...and holds non-owning references to incoming GraphEdges
       ('incoming' refers to the parent edges)*/
    std::vector<GraphEdge *> _parentEdges; // edges to preceding nodes 
    //std::vector<std::weak_ptr<GraphEdge>> _parentEdges; // edges to preceding nodes 
    
    /* Task 5 */
    ChatBot _chatBot;
    //std::unique_ptr<ChatBot> _chatBot;
    //ChatBot *_chatBot;

    ////
    //// EOF STUDENT CODE

    // proprietary members
    int _id;
    std::vector<std::string> _answers;

public:
    // constructor / destructor
    GraphNode(int id);
    ~GraphNode();

    // getter / setter
    int GetID() { return _id; }
    int GetNumberOfChildEdges() { return _childEdges.size(); }
    GraphEdge *GetChildEdgeAtIndex(int index);
    std::vector<std::string> GetAnswers() { return _answers; }
    int GetNumberOfParents() { return _parentEdges.size(); }

    // proprietary functions
    void AddToken(std::string token); // add answers to list

    /* Task 4: This isn't marked for modification by the student, but it seems to me it must be modified. */
    //void AddEdgeToParentNode(std::weak_ptr<GraphEdge> edge);
    void AddEdgeToParentNode(GraphEdge *edge);
    void AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge);
    //void AddEdgeToChildNode(GraphEdge *edge);

    //// STUDENT CODE
    ////

	/* Task 5 */
    void MoveChatbotHere(ChatBot chatbot);
	//void MoveChatbotHere(std::unique_ptr<ChatBot> chatbot);
    //void MoveChatbotHere(ChatBot *chatbot);

    ////
    //// EOF STUDENT CODE

    void MoveChatbotToNewNode(GraphNode *newNode);
};

#endif /* GRAPHNODE_H_ */