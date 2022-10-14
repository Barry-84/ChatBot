#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <tuple>
#include <algorithm>

#include "graphedge.h"
#include "graphnode.h"
#include "chatbot.h"
#include "chatlogic.h"


ChatLogic::ChatLogic()
{
    //// STUDENT CODE
    ////

    /* Task 5: set at the bottom of function LoadAnswerGraphFromFile */
    // create instance of chatbot
    //_chatBot = new ChatBot("../images/chatbot.png");

    // add pointer to chatlogic so that chatbot answers can be passed on to the GUI
    //_chatBot->SetChatLogicHandle(this);

    ////
    //// EOF STUDENT CODE
}

ChatLogic::~ChatLogic()
{
    //// STUDENT CODE
    ////

    // delete chatbot instance
    //delete _chatBot;

    /* Task 3 involved declaring vector _nodes as a vector of unique pointers.
       _nodes was originally a vector of raw pointers. The code below has been
       removed since the resources managed by smart pointers, unlike raw pointers,
       are automatically deleted once they go out of scope.
    */
    // delete all nodes
//   for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
//    {
//        delete *it;
//    }

  	/* _edges no used at all */
//    // delete all edges
//   for (auto it = std::begin(_edges); it != std::end(_edges); ++it)
//    {
//        delete *it;
//    }

    ////
    //// EOF STUDENT CODE
}

template <typename T>
void ChatLogic::AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element)
{
    // find all occurences for current node
    auto token = tokens.begin();
    while (true)
    {
        token = std::find_if(token, tokens.end(), [&tokenID](const std::pair<std::string, std::string> &pair) { return pair.first == tokenID;; });
        if (token != tokens.end())
        {
            element.AddToken(token->second); // add new keyword to edge
            token++;                         // increment iterator to next element
        }
        else
        {
            break; // quit infinite while-loop
        }
    }
}

void ChatLogic::LoadAnswerGraphFromFile(std::string filename)
{
    // load file with answer graph elements
    std::ifstream file(filename);

    // check for file availability and process it line by line
    if (file)
    {
        // loop over all lines in the file
        std::string lineStr;
        while (getline(file, lineStr))
        {
            // extract all tokens from current line
            tokenlist tokens;
            while (lineStr.size() > 0)
            {
                // extract next token
                int posTokenFront = lineStr.find("<");
                int posTokenBack = lineStr.find(">");
                if (posTokenFront < 0 || posTokenBack < 0)
                    break; // quit loop if no complete token has been found
                std::string tokenStr = lineStr.substr(posTokenFront + 1, posTokenBack - 1);

                // extract token type and info
                int posTokenInfo = tokenStr.find(":");
                if (posTokenInfo != std::string::npos)
                {
                    std::string tokenType = tokenStr.substr(0, posTokenInfo);
                    std::string tokenInfo = tokenStr.substr(posTokenInfo + 1, tokenStr.size() - 1);

                    // add token to vector
                    tokens.push_back(std::make_pair(tokenType, tokenInfo));
                }

                // remove token from current line
                lineStr = lineStr.substr(posTokenBack + 1, lineStr.size());
            }

            // process tokens for current line
            auto type = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "TYPE"; });
            if (type != tokens.end())
            {
                // check for id
                auto idToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "ID"; });
                if (idToken != tokens.end())
                {
                    // extract id from token
                    int id = std::stoi(idToken->second);

                    // node-based processing
                    if (type->second == "NODE")
                    {
                        //// STUDENT CODE
                        ////

                        // check if node with this ID exists already
                        /* Task 3 involved declaring vector _nodes as a vector of unique pointers.
                           _nodes was originally a vector of raw pointers. The code below has been
                           modified since _nodes no longer contains raw pointers.
                        */
                      //auto newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](GraphNode *node) { return node->GetID() == id; });
                      auto newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](std::unique_ptr<GraphNode>& node) { return node->GetID() == id; });

                        // create new element if ID does not yet exist
                        if (newNode == _nodes.end())
                        {
                            //_nodes.emplace_back(new GraphNode(id)); // ToDO: should be a unique pointer
                            _nodes.emplace_back(std::make_unique<GraphNode>(id));
                            newNode = _nodes.end() - 1; // get iterator to last element

                            // add all answers to current node
                            AddAllTokensToElement("ANSWER", tokens, **newNode);
                        }

                        ////
                        //// EOF STUDENT CODE
                    }

                    // edge-based processing
                    if (type->second == "EDGE")
                    {
                        //// STUDENT CODE
                        ////

                        // find tokens for incoming (parent) and outgoing (child) node
                        auto parentToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "PARENT"; });
                        auto childToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "CHILD"; });

                        if (parentToken != tokens.end() && childToken != tokens.end())
                        {
                            // get iterator on incoming and outgoing node via ID search
                            /* Task 3 involved declaring vector _nodes as a vector of unique pointers.
                               _nodes was originally a vector of raw pointers. The code below has been
                               modified since _nodes no longer contains raw pointers.
                            */
                            //auto parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](GraphNode *node) { return node->GetID() == std::stoi(parentToken->second); });
                          	auto parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](std::unique_ptr<GraphNode>& node) { return node->GetID() == std::stoi(parentToken->second); });
                            //auto childNode = std::find_if(_nodes.begin(), _nodes.end(), [&childToken](GraphNode *node) { return node->GetID() == std::stoi(childToken->second); });
                            auto childNode = std::find_if(_nodes.begin(), _nodes.end(), [&childToken](std::unique_ptr<GraphNode>& node) { return node->GetID() == std::stoi(childToken->second); });
                            /* Task 4: change the ownership of all instances of GraphEdge in a way such that each instance of GraphNode exclusively
                               owns the outgoing GraphEdges and holds non-owning references to incoming GraphEdges. But how do we know whether this
                               will be incoming or outgoing? I've just chosen a uniue pointer here, which would be for outgoing. */
                            // create new edge
                            // GraphEdge *edge = new GraphEdge(id);
                            std::unique_ptr<GraphEdge> edge = std::make_unique<GraphEdge>(id);
                          
                            /* Task 3 involved declaring vector _nodes as a vector of unique pointers.
                               _nodes was originally a vector of raw pointers. The code below has been
                              modified since _nodes no longer contains raw pointers.
                              childnode is an iterator of type std::vector<std::unique_ptr<GraphNode>>::iterator. In other words, I think it's
                              a raw pointer to a unique_pointer and that's why it's first dereferenced here.
                            */
                            //edge->SetChildNode(*childNode);
                            edge->SetChildNode((*childNode).get()); // Task 3: function doesn't modify the pointer, so just pass it the raw pointer
                            //edge->SetParentNode(*parentNode);
                            edge->SetParentNode((*parentNode).get()); // Task 3: function doesn't modify the pointer, so just pass it the raw pointer
                            /* Task 4: edge is a unique pointer. We can't call push_back with it as this would amount to copying it, which is not allowed. */  
                          
                            /* _edges no used at all */
							//_edges.push_back(edge);
                            //_edges.push_back(edge.get());

                            // find all keywords for current node
                            AddAllTokensToElement("KEYWORD", tokens, *edge);

                            /* Task 4 */
                            // store reference in child node and parent node
                            //(*childNode)->AddEdgeToParentNode(edge); // ToDO: Task 4
                            //(*parentNode)->AddEdgeToChildNode(edge); // ToDO: Task 4
                            (*childNode)->AddEdgeToParentNode(edge.get());                         
                            (*parentNode)->AddEdgeToChildNode(std::move(edge));
                        }

                        ////
                        //// EOF STUDENT CODE
                    }
                }
                else
                {
                    std::cout << "Error: ID missing. Line is ignored!" << std::endl;
                }
            }
        } // eof loop over all lines in the file

        file.close();

    } // eof check for file availability
    else
    {
        std::cout << "File could not be opened!" << std::endl;
        return;
    }

    //// STUDENT CODE
    ////

    // identify root node
    GraphNode *rootNode = nullptr;
    for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    {
        // search for nodes which have no incoming edges
        if ((*it)->GetNumberOfParents() == 0)
        {

            if (rootNode == nullptr)
            {
                /* Task 3 involved declaring vector _nodes as a vector of unique pointers.
                   _nodes was originally a vector of raw pointers. The code below has been
                   modified since the variable 'it' is now a pointer to smart (unique) pointer. 
                */
                //rootNode = *it; // assign current node to root
            	rootNode = (*it).get(); // assign current node to root
            }
            else
            {
                std::cout << "ERROR : Multiple root nodes detected" << std::endl;
            }
        }
    }

	/* Task 5: create a local ChatBot instance on the stack at the bottom of function LoadAnswerGraphFromFile.
       Then, use move semantics to pass the ChatBot instance into the root node. Make sure that ChatLogic has
       no ownership relation to the ChatBot instance and thus is no longer responsible for memory allocation
       and deallocation. 
    */
  
	ChatBot localchatbot = ChatBot("../images/chatbot.png");
  	
    // add chatbot to graph root node
  	localchatbot.SetRootNode(rootNode);
    //_chatBot->SetRootNode(rootNode);
  
    localchatbot.SetChatLogicHandle(this);
  
	_chatBot = &localchatbot;
  
  	rootNode->MoveChatbotHere(std::move(localchatbot));
    //rootNode->MoveChatbotHere(_chatBot);
    
    ////
    //// EOF STUDENT CODE
}

void ChatLogic::SetPanelDialogHandle(ChatBotPanelDialog *panelDialog)
{
    _panelDialog = panelDialog;
}

void ChatLogic::SetChatbotHandle(ChatBot *chatbot)
{
    _chatBot = chatbot;
}

void ChatLogic::SendMessageToChatbot(std::string message)
{
    _chatBot->ReceiveMessageFromUser(message);
}

void ChatLogic::SendMessageToUser(std::string message)
{
    _panelDialog->PrintChatbotResponse(message);
}

wxBitmap *ChatLogic::GetImageFromChatbot()
{
    return _chatBot->GetImageHandle();
}
