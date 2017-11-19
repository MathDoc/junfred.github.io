#ifndef BINTREE_H_
#define BINTREE_H_

#include <iostream>
namespace mstd
{

template <class T> class BST
{
	class BT
	{
	      public:
		T value;
		BT *left;
		BT *right;

		inline BT()
		{
			this->left = nullptr;
			this->right = nullptr;
		}

		inline ~BT()
		{
			if (this->left != nullptr)
				delete this->left;
			if (this->right != nullptr)
				delete this->right;
			delete this;
		}

		inline BT *make_new_node(const T value)
		{
			BT *new_tree;
			new_tree = new BT[sizeof(BT)];

			new_tree->left = nullptr;
			new_tree->right = nullptr;
			new_tree->value = value;

			return new_tree;
		}

		inline void addLeft(const T value)
		{
			BT *new_tree = make_new_node(value);
			this->left = new_tree;
		}

		inline void addRight(const T value)
		{
			BT *new_tree = make_new_node(value);
			this->right = new_tree;
		}

		inline void insert(const T value)
		{

			if (this->value > value) {
				if (this->left != nullptr)
					this->left->insert(value);
				else {
					this->addLeft(value);
				}
			} else {
				if (this->right != nullptr)
					this->right->insert(value);
				else
					this->addRight(value);
			}
			return;
		}

		inline bool find(const T value)
		{
			if (this->value > value) {
				if (this->left == nullptr)
					return 0;
				return this->left->find(value);
			}

			if (this->value < value) {
				if (this->right == nullptr)
					return 0;
				return this->right->find(value);
			}

			return 1;
		}

		inline void preorder()
		{
			std::cout << this->value << " ";
			if (this->left != nullptr)
				this->left->preorder();
			if (this->right != nullptr)
				this->right->preorder();
		}
		inline void inorder()
		{
			if (this->left != nullptr)
				this->left->inorder();
			std::cout << this->value << " ";
			if (this->right != nullptr)
				this->right->inorder();
		}
		inline void postorder()
		{
			if (this->left != nullptr)
				this->left->postorder();
			if (this->right != nullptr)
				this->right->postorder();
			std::cout << this->value << " ";
		}
	};

      private:
	BT *root;

      public:
	BST() { this->root = nullptr; }
	BST(T value) { this->root = this->root->make_new_node(value); }
	~BST() { delete this->root; }

	inline void insert(const T value)
	{
		if (this->root == nullptr) {
			this->root = this->root->make_new_node(value);
			return;
		}
		this->root->insert(value);
		return;
	}

	inline bool find(const T value) { return this->root->find(value); }

	inline void preorder() { this->root->preorder(); }
	inline void inorder() { this->root->inorder(); }
	inline void postorder() { this->root->postorder(); }
};
}

#endif

