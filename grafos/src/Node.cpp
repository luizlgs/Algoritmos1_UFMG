#include "../include/Node.hpp"

Node::Node(int id){
    this->_id = id;
}

Node::~Node() {}

int Node::get_id() const{
    return _id;
}

std::vector<Node*> Node::get_adj() const{
    return _adj;
}

bool Node::get_discovered() const{
    return _discovered;
}

bool Node::get_ended() const{
    return _ended;
}

void Node::new_adj(Node *new_node){
    _adj.push_back(new_node);
}

void Node::remove_adj(int id){
    int j=0;
    for(auto i:_adj){
        if(i->get_id() == id){
            _adj.erase(_adj.begin() + j);
            return;
        }
        j++;
    }
}

void Node::set_discovered(){
    _discovered = true;
}

void Node::set_ended(){
    _ended = true;
}

