#include "stdio.h"
#include "string.h"
#include "RedBlackTree.hpp"

enum COLOR{ TREECOLOR_BLACK, TREECOLOR_REN };

template<class T>
struct RedBlackNode
{
	T Key;
	RedBlackNode* pLeft, *pRight, *parent;
	COLOR color;
	RedBlackNode();
};

template<class K>
class RedBlackTree
{

public:
	RedBlackTree();
public:
	void Insert(RedBlackNode<K>* _RedBlackNode);
	bool Delete(RedBlackNode<K>* _Node);
	bool IsExists(RedBlackNode<K>* _RedBlackNode);
protected:
	void InsertFixUp(RedBlackNode<K>* _RedBlackNode);
	inline void RightRotate(RedBlackNode<K>* _RedBlackNode);
	inline void LeftRotate(RedBlackNode<K>* _RedBlackNode);
	void DeleteFixUp(RedBlackNode<K> *node, RedBlackNode<K> *parent);
private:
	RedBlackNode<K>* pRoot;
};

template<class T>
RedBlackNode<T>::RedBlackNode()
{
	memset(this, 0x00, sizeof(RedBlackNode));
}

template<class K>
RedBlackTree<K>::RedBlackTree()
{
	pRoot = 0;
}

template<class K>
void RedBlackTree<K>::Insert(RedBlackNode<K>* _RedBlackNode)
{
	RedBlackNode<K>* pRedBlackNode = pRoot;
	if (!pRedBlackNode)
	{
		pRoot = _RedBlackNode;
		pRoot->color = TREECOLOR_BLACK;
		return;
	}
	while (true)
	{
		if (pRedBlackNode->Key <= _RedBlackNode->Key)
		{
			if (pRedBlackNode->pRight)
				pRedBlackNode = pRedBlackNode->pRight;
			else
			{
				pRedBlackNode->pRight = _RedBlackNode;
				_RedBlackNode->parent = pRedBlackNode;
				break;
			}
		}
		else
		{
			if (pRedBlackNode->pLeft)
				pRedBlackNode = pRedBlackNode->pLeft;
			else
			{
				pRedBlackNode->pLeft = _RedBlackNode;
				_RedBlackNode->parent = pRedBlackNode;
				break;
			}
		}
	}

	InsertFixUp(_RedBlackNode);
}

template<class K>
void RedBlackTree<K>::InsertFixUp(RedBlackNode<K>* _RedBlackNode)
{
	while (_RedBlackNode->parent && _RedBlackNode->parent->color == TREECOLOR_REN)
	{
		if (_RedBlackNode->parent == _RedBlackNode->parent->parent->pLeft)   //    
		{
			RedBlackNode<K>* uncle = _RedBlackNode->parent->parent->pRight;
			if (uncle && uncle->color == TREECOLOR_REN)   //插入情况1，z的叔叔y是红色的。    
			{
				_RedBlackNode->parent->color = TREECOLOR_BLACK;
				uncle->color = TREECOLOR_BLACK;
				_RedBlackNode->parent->parent->color = TREECOLOR_REN;
				_RedBlackNode = _RedBlackNode->parent->parent;
			}
			else  //插入情况2：z的叔叔y是黑色的，。    
			{
				if (_RedBlackNode == _RedBlackNode->parent->pRight) //且z是右孩子    
				{
					_RedBlackNode = _RedBlackNode->parent;
					LeftRotate(_RedBlackNode);
				}
				//else                 //插入情况3：z的叔叔y是黑色的，但z是左孩子。    
				_RedBlackNode->parent->color = TREECOLOR_BLACK;
				_RedBlackNode->parent->parent->color = TREECOLOR_REN;
				RightRotate(_RedBlackNode->parent->parent);
			}
		}
		else //这部分是针对为插入情况1中，z的父亲现在作为祖父的右孩子了的情况，而写的。    
			//15 else (same as then clause with "pRight" and "pLeft" exchanged)    
		{
			RedBlackNode<K>* uncle = _RedBlackNode->parent->parent->pLeft;
			if (uncle && uncle->color == TREECOLOR_REN)
			{
				_RedBlackNode->parent->color = TREECOLOR_BLACK;
				uncle->color = TREECOLOR_BLACK;
				uncle->parent->color = TREECOLOR_REN;
				_RedBlackNode = _RedBlackNode->parent->parent;
			}
			else
			{
				if (_RedBlackNode == _RedBlackNode->parent->pLeft)
				{
					_RedBlackNode = _RedBlackNode->parent;
					RightRotate(_RedBlackNode);     //与上述代码相比，左旋改为右旋    
				}
				_RedBlackNode->parent->color = TREECOLOR_BLACK;
				_RedBlackNode->parent->parent->color = TREECOLOR_REN;
				LeftRotate(_RedBlackNode->parent->parent);   //右旋改为左旋，即可。    
			}
		}
	}
	pRoot->color = TREECOLOR_BLACK;
}

template<class K>
void RedBlackTree<K>::RightRotate(RedBlackNode<K>* _RedBlackNode)
{
	if (!_RedBlackNode || !_RedBlackNode->pLeft)
		return;
	RedBlackNode<K>* lower_left = _RedBlackNode->pLeft;
	_RedBlackNode->pLeft = lower_left->pRight;
	lower_left->parent = _RedBlackNode->parent;
	if (lower_left->pRight)
	{
		lower_left->pRight->parent = _RedBlackNode;
	}
	if (!_RedBlackNode->parent)
	{
		pRoot = lower_left;
		pRoot->parent = 0;
	}
	else
	{
		if (_RedBlackNode == _RedBlackNode->parent->pRight)
			_RedBlackNode->parent->pRight = lower_left;
		else
			_RedBlackNode->parent->pLeft = lower_left;
	}
	_RedBlackNode->parent = lower_left;
	lower_left->pRight = _RedBlackNode;
}

template<class K>
void RedBlackTree<K>::LeftRotate(RedBlackNode<K>* _RedBlackNode)
{
	if (!_RedBlackNode || !_RedBlackNode->pRight)
		return;
	RedBlackNode<K>* lower_right = _RedBlackNode->pRight;
	lower_right->parent = _RedBlackNode->parent;
	_RedBlackNode->pRight = lower_right->pLeft;
	if (lower_right->pLeft)
		lower_right->pLeft->parent = _RedBlackNode;
	if (!_RedBlackNode->parent)
	{
		pRoot = lower_right;
		pRoot->parent = 0;
	}
	else
	{
		if (_RedBlackNode == _RedBlackNode->parent->pLeft)
		{
			_RedBlackNode->parent->pLeft = lower_right;
		}
		else
		{
			_RedBlackNode->parent->pLeft = lower_right;
		}
	}
	_RedBlackNode->parent = lower_right;
	lower_right->pLeft = _RedBlackNode;
}

template<class K>
bool RedBlackTree<K>::Delete(RedBlackNode<K>*  _Node)
{
	RedBlackNode<K> *child, *parent;
	COLOR color;

	// 被删除节点的"左右孩子都不为空"的情况。
	if ((_Node->pLeft != NULL) && (_Node->pRight != NULL))
	{
		// 被删节点的后继节点。(称为"取代节点")
		// 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
		RedBlackNode<K> *replace = _Node;

		// 获取后继节点
		replace = replace->pRight;
		while (replace->pLeft != NULL)
			replace = replace->pLeft;

		// "_Node节点"不是根节点(只有根节点不存在父节点)
		if (_Node->parent)
		{
			if (_Node->parent->pLeft == _Node)
				_Node->parent->pLeft = replace;
			else
				_Node->parent->pRight = replace;
		}
		else
			// "_Node节点"是根节点，更新根节点。
			pRoot = replace;

		// child是"取代节点"的右孩子，也是需要"调整的节点"。
		// "取代节点"肯定不存在左孩子！因为它是一个后继节点。
		child = replace->pRight;
		parent = replace->parent;
		// 保存"取代节点"的颜色
		color = replace->color;

		// "被删除节点"是"它的后继节点的父节点"
		if (parent == _Node)
		{
			parent = replace;
		}
		else
		{
			// child不为空
			if (child)
				child->parent = parent;
			parent->pLeft = child;

			replace->pRight = _Node->pRight;
			_Node->pRight->parent = replace;
		}

		replace->parent = _Node->parent;
		replace->color = _Node->color;
		replace->pLeft = _Node->pLeft;
		_Node->pLeft->parent = replace;

		if (color == BLACK)
			DeleteFixUp(child, parent);

		//		delete _Node;
		return true;
	}

	if (_Node->pLeft != NULL)
		child = _Node->pLeft;
	else
		child = _Node->pRight;

	parent = _Node->parent;
	// 保存"取代节点"的颜色
	color = _Node->color;

	if (child)
		child->parent = parent;

	// "_Node节点"不是根节点
	if (parent)
	{
		if (parent->pLeft == _Node)
			parent->pLeft = child;
		else
			parent->pRight = child;
	}
	else
		pRoot = child;

	if (color == BLACK)
		DeleteFixUp(child, parent);
	//	delete _Node;

	return true;
}

template<class K>
bool RedBlackTree<K>::IsExists(RedBlackNode<K>* _RedBlackNode)
{
	if (!pRoot)
		return false;
	if (_RedBlackNode == pRoot)
		return true;

	RedBlackNode<K>* _NowNode = pRoot;
	while (true)
	{
		if (_NowNode->Key < _RedBlackNode->Key)
		{
			if (_NowNode->pRight)
				_NowNode = _NowNode->pRight;
			else
				return false;
		}
		else if (_NowNode->Key > _RedBlackNode->Key)
		{
			if (_NowNode->pLeft)
				_NowNode = _NowNode->pLeft;
			else
				return false;
		}
		else if (_NowNode->Key == _RedBlackNode->Key)
			return true;
	}

	return true;
}

template<class K>
void RedBlackTree<K>::DeleteFixUp(RedBlackNode<K> *node, RedBlackNode<K> *parent)
{
	RedBlackNode<K> *other;

	while ((!node || node->color == TREECOLOR_BLACK) && node != pRoot)
	{
		if (parent->pLeft == node)
		{
			other = parent->pRight;
			if (other->color == TREECOLOR_REN)
			{
				// Case 1: x的兄弟w是红色的  
				other->color = TREECOLOR_BLACK;
				parent->color = TREECOLOR_REN;
				LeftRotate(parent);
				other = parent->pRight;
			}
			if ((!other->pLeft || other->pLeft->color == TREECOLOR_BLACK) &&
				(!other->pRight || other->pRight->color == TREECOLOR_BLACK))
			{
				// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
				other->color = TREECOLOR_REN;
				node = parent;
				parent = node->parent;
			}
			else
			{
				if (!other->pRight || rb_is_black(other->pRight))
				{
					// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
					other->pLeft->color = TREECOLOR_BLACK;
					other->color = TREECOLOR_REN;
					RightRotate(other);
					other = parent->pRight;
				}
				// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
				other->color = parent->color;
				parent->color = TREECOLOR_BLACK;
				other->pRight->color = TREECOLOR_BLACK;
				LeftRotate(parent);
				node = pRoot;
				break;
			}
		}
		else
		{
			other = parent->pLeft;
			if (other->color == TREECOLOR_REN)
			{
				// Case 1: x的兄弟w是红色的  
				other->color = TREECOLOR_BLACK;
				parent->color = TREECOLOR_REN;
				RightRotate(parent);
				other = parent->pLeft;
			}
			if ((!other->pLeft || other->pLeft->color == TREECOLOR_BLACK) &&
				(!other->pRight || other->pRight->color == TREECOLOR_BLACK))
			{
				// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
				other->color = TREECOLOR_REN;
				node = parent;
				parent = node->parent;
			}
			else
			{
				if (!other->pLeft || other->pLeft->color == TREECOLOR_BLACK)
				{
					// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
					other->pRight->color = TREECOLOR_BLACK;
					other->color = TREECOLOR_REN;
					LeftRotate(other);
					other = parent->pLeft;
				}
				// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
				other->color = parent->color;
				parent->color = TREECOLOR_BLACK;
				other->pLeft->color = TREECOLOR_BLACK;
				RightRotate(parent);
				node = pRoot;
				break;
			}
		}
	}
	if (node)
		node->color = TREECOLOR_BLACK;
}

int main()
{
	RedBlackTree<int> m_RedBlackTree;
	RedBlackNode<int> m_RedBlackNode[10];
	RBTree<int> m_RBTNode;

	m_RedBlackNode[0].Key = 100;
	m_RedBlackNode[1].Key = 90;
	m_RedBlackNode[2].Key = 110;
	m_RedBlackNode[3].Key = 80;
	m_RedBlackNode[4].Key = 85;
	m_RedBlackNode[5].Key = 82;
	m_RedBlackNode[6].Key = 81;
	m_RedBlackNode[7].Key = 120;
	m_RedBlackNode[8].Key = 109;
	m_RedBlackNode[9].Key = 105;

	m_RBTNode.insert(100);
	m_RBTNode.insert(90);
	m_RBTNode.insert(110);
	m_RBTNode.insert(80);
	m_RBTNode.insert(85);
	m_RBTNode.insert(82);
	m_RBTNode.insert(81);
	m_RBTNode.insert(120);
	m_RBTNode.insert(105);

	for (int a = 0; a < 10; a++)
	{
		m_RedBlackNode[a].color = TREECOLOR_REN;
		m_RedBlackTree.Insert(&m_RedBlackNode[a]);
	}


	for (int a = 0; a < 10; a++)
	{
		m_RBTNode.remove(m_RedBlackNode[a].Key);
		m_RedBlackTree.Delete(&m_RedBlackNode[a]);
	}

	return true;
}