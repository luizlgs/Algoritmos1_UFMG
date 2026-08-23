#ifndef NODE_HPP
#define NODE_HPP

#include "core.hpp"
class Node
{
private:
    int _id;
    std::vector<Node*> _adj;

    bool _discovered = false;
    bool _ended = false;
public:
    Node(int id);
    ~Node();
    int get_id() const;
    std::vector<Node*> get_adj() const;
    bool get_discovered() const;
    bool get_ended() const;


    void new_adj(Node *new_node);
    void remove_adj(int id);
    void set_discovered();
    void set_ended();
};

#endif
