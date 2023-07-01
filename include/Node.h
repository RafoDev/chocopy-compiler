#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Node
{
public:
	string value;
	Node *parent;
	vector<Node *> children;

	Node &operator=(const Node &other)
	{
		value = other.value;
		parent = other.parent;

		for (Node *child : children)
			delete child;
		children.clear();

		for (Node *child : other.children)
		{
			addChild(child);
		}

		return *this;
	}
	Node()
	{
		value = "[empty]";
	}

	string getValue()
	{
		return value;
	}
	vector<Node *> getChildren()
	{
		return children;
	}
	size_t getChildSize()
	{
		return children.size();
	}
	Node(string _value)
	{
		value = _value;
	}
	Node(string _value, Node *_parent)
	{
		value = _value;
		parent = _parent;
		parent->addChild(this);
	}

	void addChild(Node *child)
	{
		if (child->value != "[empty]")
			children.push_back(child);
	}
	void setParent(Node *_parent)
	{
		parent = _parent;
		parent->addChild(this);
	}

	void printPadding(int size)
	{
		for (int i = 0; i < size; i++)
			cout << " ";
	}

	void printUtil(Node *node, int lv)
	{
		vector<Node *> nodeChildren = node->getChildren();
		string value = node->getValue();
		int padding = value.size() / 2 + (3 * lv);
		int childSize = node->getChildSize();

		cout << node->getValue() << '\n';

		if (childSize == 0)
			return;

		for (int i = 0; i < childSize - 1; i++)
		{
			printPadding(padding);
			cout << "├──";
			printUtil(nodeChildren[i], lv + 1);
		}
		printPadding(padding);
		cout << "└──";
		printUtil(nodeChildren[childSize - 1], lv + 1);
	}

	void print()
	{
		int childSize = children.size();
		int padding = value.size() / 2 - 1;
		cout << value << '\n';
		if (childSize == 0)
			return;
		int i;
		for (i = 0; i < childSize - 1; i++)
		{
			printPadding(padding);
			cout << "├──";
			printUtil(children[i], 1);
		}
		printPadding(padding);
		cout << "└──";
		printUtil(children[childSize - 1], 1);
		cout<<"\n";
	}
	// void printUtil(Node *node, int padding)
	// {
	// 	string value = node->value;
	// 	int childSize = node->children.size();

	// 	cout << node->getValue() << " " << childSize << '\n';
	// 	padding++;
	// 	if(childSize == 0) return;
	// 	for (int i = 0; i < childSize; i++)
	// 	{
	// 		printPadding(padding);
	// 		printUtil(node->children[i], padding);
	// 	}
	// }

	// void print()
	// {
	// 	int childSize = children.size();
	// 	int padding = 1;
	// 	cout << value << " " << childSize << '\n';

	// 	for (int i = 0; i < childSize; i++)
	// 	{
	// 		printPadding(padding);
	// 		printUtil(children[i], 1);
	// 	}
	// 	cout << "termine de imprimir";
	// }
	// void printTreeUtil(Node *node, int level)
	// {
	// 	if (node == nullptr)
	// 		return;

	// 	for (int i = 0; i < level; i++)
	// 		cout << "  ";

	// 	cout << "|-- " << node->value << endl;

	// 	int size = node->children.size();
	// 	for (int i = 0; i < size; i++)
	// 	{

	// 		printTreeUtil(node->children[i], level + 2);
	// 	}
	// 	// for (auto *child : node->children)
	// 	// {
	// 	// }
	// }

	// void print()
	// {
	// 	printTreeUtil(this, 0);
	// }
};

class Nodes
{
private:
public:
	vector<Node *> nodes;

	Nodes() {} // Constructor vacío

	Nodes(const Nodes &other)
	{
		for (Node *node : other.nodes)
		{
			Node *newNode = new Node(*node); // Constructor copia de Node
			nodes.push_back(newNode);
		}
	}

	void add(Node *node)
	{

		if(node->value != "[empty]"){
			nodes.push_back(node);
		}
			
	}

	void add(Nodes nodeList)
	{
		if (!nodeList.empty())
		{
			for (Node *node : nodeList.nodes)
			{
				add(node);
			}
		}
	}

	void setParent(Node *parent)
	{
		for (Node *node : nodes)
		{
			node->setParent(parent);
		}
	}

	Nodes &operator=(const Nodes &other)
	{
		if (this == &other || other.nodes.empty())
			return *this;

		// Limpiar los nodos existentes
		for (Node *node : nodes)
		{
			delete node;
		}
		nodes.clear();

		// Crear nuevos nodos
		for (Node *node : other.nodes)
		{
			Node *newNode = new Node(*node); // Constructor copia de Node
			nodes.push_back(newNode);
		}

		return *this;
	}
	bool empty()
	{
		return nodes.empty();
	}

	void print()
	{
		for (Node *node : nodes)
		{
			node->print();
		}
	}
};