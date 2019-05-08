#include "treeelement.h"

TreeElement::TreeElement(Token token)
{
    this->token = token;
    this->type = tokenType;
}

TreeElement::TreeElement(treeElementTypes type){
    this->type = type;
}

void TreeElement::setAsChild(TreeElement* child){
    (this->children).push_back(child);
    child->parent = this;
}
