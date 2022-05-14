#include "BinarySearchTree.h"
#include <iostream>
#include <string>
using namespace std;

BinarySearchTree::~BinarySearchTree()
{
	release(m_root); // recursive function to release
}

void BinarySearchTree::release(TreeNode* root)
{
	if (root == NULL) // no data
		return;
	if (root->m_left != NULL) // left node exists, release
		release(root->m_left);
	if (root->m_right != NULL) // right node exists, release
		release(root->m_right);
	root->m_left = NULL; // release current node's left node
	root->m_right = NULL; // release current node's right node
	if (root != NULL) // root node exists, release
		delete root;
}

void BinarySearchTree::insert(SoccerPlayerData& data)
{
	TreeNode* newnode = new TreeNode(data); // inserting data
	if (m_root == NULL) // root node is empty, insert to root node
		m_root = newnode;
	else { // root node is filled
		TreeNode* current = new TreeNode(); // current node at position to insert
		current = m_root; // start from root node to determine where to insert
		while (1) {
			if (data.m_ability < current->m_data.m_ability) { // inserting data is smaller than current node's data, insert to left node
				if (current->m_left == NULL) { // insert to empty node
					current->m_left = newnode;
					return;
				}
				current = current->m_left; // find where is empty
			}
			else if (data.m_ability > current->m_data.m_ability) { // inserting data is bigger than current node's data, insert to right node
				if (current->m_right == NULL) { // insert to empty node
					current->m_right = newnode;
					return;
				}
				current = current->m_right; // find where is empty
			}
		}
	}
}

void BinarySearchTree::deletion(int ability)
{
	TreeNode* c_node = new TreeNode(); // removing node
	TreeNode* p_node = new TreeNode(); // parent node
	TreeNode* r_node = new TreeNode(); // temporary node
	TreeNode* ch_node = new TreeNode(); // child node
	TreeNode* temp = new TreeNode(); // left side node
	c_node = m_root; // start from root node

	while (c_node->m_data.m_ability != ability) { // find node which is having same ability
		if (ability < c_node->m_data.m_ability) { // ability is smaller than current ability, goto left
			p_node = c_node;
			c_node = c_node->m_left; // parent node's left node is current node
		}
		else if (ability > c_node->m_data.m_ability) { // ability is bigger than current ability, goto right
			p_node = c_node;
			c_node = c_node->m_right; // parent node's right node is current node
		}
	}
	if (m_root->m_data.m_ability == ability) { // if removing data is root node
		if (c_node->m_left == NULL && c_node->m_right == NULL) { // root node is terminal node
			delete c_node; // release root node
			m_root = NULL; // reset to NULL
		}
		else if (c_node->m_left == NULL || c_node->m_right == NULL) { // root node has one child node
			if (c_node->m_left == NULL) // has right node
				r_node = c_node->m_right; // take root node's right node
			if (c_node->m_right == NULL) // has left node
				r_node = c_node->m_left; // take root node's left node
			delete c_node; // release root node
			m_root = r_node; // new root node is root node's left or right node
		}
		else if (c_node->m_left != NULL && c_node->m_right != NULL) { // root node has two child node
			r_node = c_node->m_right; // move to right node
			if (r_node->m_left == NULL) { // no left child node
				temp = c_node->m_left; // root node's left child node
				ch_node = c_node->m_right; // right child node
				m_root = r_node; // new root node
				r_node->m_left = temp;
				delete c_node;
			}
			else { // no right child node
				while (r_node->m_left != NULL) { // goto leftmost node
					p_node = r_node;
					r_node = r_node->m_left;
				}
				p_node->m_left = NULL; // remove node to import to root node
				m_root->m_data = r_node->m_data; // new root node
				delete r_node;
			}
		}
	}
	else if (c_node->m_left == NULL && c_node->m_right == NULL) { // removing node is terminal node
		if (p_node->m_left == c_node) // removing node is parent node's left node
			p_node->m_left = NULL; // reset to NULL
		if (p_node->m_right == c_node) // removing node is parent node's right node
			p_node->m_right = NULL; // reset to NULL
		delete c_node;
	}
	else if (c_node->m_left == NULL || c_node->m_right == NULL) { // has one child node
		if (c_node->m_left == NULL) // has right child node
			r_node = c_node->m_right; // goto removing node's right node
		if (c_node->m_right == NULL) // has left child node
			r_node = c_node->m_left; // goto removing node's left node

		if (p_node->m_left == c_node) // removing node is parent node's left node
			p_node->m_left = r_node; // parent node's new left node is removing node's left node
		if (p_node->m_right == c_node) // removing node is parent node's right node
			p_node->m_right = r_node; // parent node's new right node is removing node's right node
		delete c_node;
	}
	else if (c_node->m_left != NULL && c_node->m_right != NULL) { // has two child node
		r_node = c_node->m_right; // move to right child node
		if (r_node->m_left == NULL) { // right child node has only right child node
			temp = c_node->m_left; // removing node's left node
			ch_node = r_node; // removing node's right node
			// remove corresponding node and fill with new left node and right node
			if (p_node->m_left == c_node) { // removing node is left
				p_node->m_left = ch_node;
				ch_node->m_left = temp;
			}
			if (p_node->m_right == c_node) { // removing node is right
				p_node->m_right = ch_node;
				ch_node->m_left = temp;
			}
			delete c_node;
		}
		else { // right child node has only left child node
			while (r_node->m_left != NULL) { // goto node which has next higher data
				p_node = r_node;
				r_node = r_node->m_left; // parent node's left node is removing node, next higher node
			}
			p_node->m_left = NULL; // removing importing node
			c_node->m_data = r_node->m_data; // new data
			delete r_node;
		}
	}
}