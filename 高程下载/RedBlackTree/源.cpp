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
			if (uncle && uncle->color == TREECOLOR_REN)   //�������1��z������y�Ǻ�ɫ�ġ�    
			{
				_RedBlackNode->parent->color = TREECOLOR_BLACK;
				uncle->color = TREECOLOR_BLACK;
				_RedBlackNode->parent->parent->color = TREECOLOR_REN;
				_RedBlackNode = _RedBlackNode->parent->parent;
			}
			else  //�������2��z������y�Ǻ�ɫ�ģ���    
			{
				if (_RedBlackNode == _RedBlackNode->parent->pRight) //��z���Һ���    
				{
					_RedBlackNode = _RedBlackNode->parent;
					LeftRotate(_RedBlackNode);
				}
				//else                 //�������3��z������y�Ǻ�ɫ�ģ���z�����ӡ�    
				_RedBlackNode->parent->color = TREECOLOR_BLACK;
				_RedBlackNode->parent->parent->color = TREECOLOR_REN;
				RightRotate(_RedBlackNode->parent->parent);
			}
		}
		else //�ⲿ�������Ϊ�������1�У�z�ĸ���������Ϊ�游���Һ����˵��������д�ġ�    
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
					RightRotate(_RedBlackNode);     //������������ȣ�������Ϊ����    
				}
				_RedBlackNode->parent->color = TREECOLOR_BLACK;
				_RedBlackNode->parent->parent->color = TREECOLOR_REN;
				LeftRotate(_RedBlackNode->parent->parent);   //������Ϊ���������ɡ�    
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

	// ��ɾ���ڵ��"���Һ��Ӷ���Ϊ��"�������
	if ((_Node->pLeft != NULL) && (_Node->pRight != NULL))
	{
		// ��ɾ�ڵ�ĺ�̽ڵ㡣(��Ϊ"ȡ���ڵ�")
		// ������ȡ��"��ɾ�ڵ�"��λ�ã�Ȼ���ٽ�"��ɾ�ڵ�"ȥ����
		RedBlackNode<K> *replace = _Node;

		// ��ȡ��̽ڵ�
		replace = replace->pRight;
		while (replace->pLeft != NULL)
			replace = replace->pLeft;

		// "_Node�ڵ�"���Ǹ��ڵ�(ֻ�и��ڵ㲻���ڸ��ڵ�)
		if (_Node->parent)
		{
			if (_Node->parent->pLeft == _Node)
				_Node->parent->pLeft = replace;
			else
				_Node->parent->pRight = replace;
		}
		else
			// "_Node�ڵ�"�Ǹ��ڵ㣬���¸��ڵ㡣
			pRoot = replace;

		// child��"ȡ���ڵ�"���Һ��ӣ�Ҳ����Ҫ"�����Ľڵ�"��
		// "ȡ���ڵ�"�϶����������ӣ���Ϊ����һ����̽ڵ㡣
		child = replace->pRight;
		parent = replace->parent;
		// ����"ȡ���ڵ�"����ɫ
		color = replace->color;

		// "��ɾ���ڵ�"��"���ĺ�̽ڵ�ĸ��ڵ�"
		if (parent == _Node)
		{
			parent = replace;
		}
		else
		{
			// child��Ϊ��
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
	// ����"ȡ���ڵ�"����ɫ
	color = _Node->color;

	if (child)
		child->parent = parent;

	// "_Node�ڵ�"���Ǹ��ڵ�
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
				// Case 1: x���ֵ�w�Ǻ�ɫ��  
				other->color = TREECOLOR_BLACK;
				parent->color = TREECOLOR_REN;
				LeftRotate(parent);
				other = parent->pRight;
			}
			if ((!other->pLeft || other->pLeft->color == TREECOLOR_BLACK) &&
				(!other->pRight || other->pRight->color == TREECOLOR_BLACK))
			{
				// Case 2: x���ֵ�w�Ǻ�ɫ����w����������Ҳ���Ǻ�ɫ��  
				other->color = TREECOLOR_REN;
				node = parent;
				parent = node->parent;
			}
			else
			{
				if (!other->pRight || rb_is_black(other->pRight))
				{
					// Case 3: x���ֵ�w�Ǻ�ɫ�ģ�����w�������Ǻ�ɫ���Һ���Ϊ��ɫ��  
					other->pLeft->color = TREECOLOR_BLACK;
					other->color = TREECOLOR_REN;
					RightRotate(other);
					other = parent->pRight;
				}
				// Case 4: x���ֵ�w�Ǻ�ɫ�ģ�����w���Һ����Ǻ�ɫ�ģ�����������ɫ��
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
				// Case 1: x���ֵ�w�Ǻ�ɫ��  
				other->color = TREECOLOR_BLACK;
				parent->color = TREECOLOR_REN;
				RightRotate(parent);
				other = parent->pLeft;
			}
			if ((!other->pLeft || other->pLeft->color == TREECOLOR_BLACK) &&
				(!other->pRight || other->pRight->color == TREECOLOR_BLACK))
			{
				// Case 2: x���ֵ�w�Ǻ�ɫ����w����������Ҳ���Ǻ�ɫ��  
				other->color = TREECOLOR_REN;
				node = parent;
				parent = node->parent;
			}
			else
			{
				if (!other->pLeft || other->pLeft->color == TREECOLOR_BLACK)
				{
					// Case 3: x���ֵ�w�Ǻ�ɫ�ģ�����w�������Ǻ�ɫ���Һ���Ϊ��ɫ��  
					other->pRight->color = TREECOLOR_BLACK;
					other->color = TREECOLOR_REN;
					LeftRotate(other);
					other = parent->pLeft;
				}
				// Case 4: x���ֵ�w�Ǻ�ɫ�ģ�����w���Һ����Ǻ�ɫ�ģ�����������ɫ��
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