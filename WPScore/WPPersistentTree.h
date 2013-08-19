#ifndef _WPPERSISTENTTREE_H
#define _WPPERSISTENTTREE_H

#include <iostream>
#include <fstream>
#include <vector>
#include "WPAllocator.h"
#include "WPMultinote.h"
#include "WPInterval.h"
#include "WPProperty.h"

class WPPersistentTreeNode
{
	public:
		WPPersistentTreeNode *Left, *Right;
		
	private:
		
};

class WPPersistentTree
{
	public:
		WPPersistentTree();
		~WPPersistentTree();
	private:
		WPPersistentTreeNode *Root;
};

/// Multinote
class WPMultinotePersistentTreeNode
{
	public:
		WPMultinotePersistentTreeNode *Left, *Right;
		WPMultinote Element;
		Fraction Sum;
		int Size;
		int Priority;
		WPMultinotePersistentTreeNode();
		~WPMultinotePersistentTreeNode();
		void Apply();
		void Update();
	private:
};

class WPMultinotePersistentTree
{
	public:
		WPMultinotePersistentTree(WPAllocator <WPMultinotePersistentTreeNode> *);
		~WPMultinotePersistentTree();
		void insert(const WPPosition &, const WPMultinote &);
		void remove(const WPInterval &);
		std::pair <Fraction, WPMultinote> query(const WPPosition &);
		std::vector <WPMultinote> traverse();
	private:
		WPMultinotePersistentTreeNode *Root;
		WPAllocator <WPMultinotePersistentTreeNode> *Alloc;
		WPMultinotePersistentTreeNode *Merge(WPMultinotePersistentTreeNode *&, WPMultinotePersistentTreeNode *&);
		void Split(WPMultinotePersistentTreeNode *, WPMultinotePersistentTreeNode *&, WPMultinotePersistentTreeNode *&, Fraction);
		WPMultinotePersistentTreeNode *SingleRotateWithLeft(WPMultinotePersistentTreeNode *&);
		WPMultinotePersistentTreeNode *SingleRotateWithRight(WPMultinotePersistentTreeNode *&);
		WPMultinotePersistentTreeNode *Insert(WPMultinotePersistentTreeNode *&, Fraction, const WPMultinote &);
		void DFS(WPMultinotePersistentTreeNode *, std::vector <WPMultinote> &);
};

/// Property
class WPPropertyPersistentTreeNode
{
	public:
		WPPropertyPersistentTreeNode *Left, *Right;
		WPProperty Element;
		int Size;
		int Priority;
		WPPropertyPersistentTreeNode();
		~WPPropertyPersistentTreeNode();
		void Apply();
		void Update();
	private:
};

class WPPropertyPersistentTree
{
	public:
		WPPropertyPersistentTree(WPAllocator <WPPropertyPersistentTreeNode> *);
		WPPropertyPersistentTree(WPAllocator <WPPropertyPersistentTreeNode> *, WPPropertyPersistentTreeNode *);
		~WPPropertyPersistentTree();
		void insert(const WPProperty &);
		void remove(const WPInterval &);
		bool remove(const WPProperty &);
		WPPropertyPersistentTree query(const WPInterval &);
		std::vector <WPProperty> traverse();
	private:
		WPPropertyPersistentTreeNode *Root;
		WPAllocator <WPPropertyPersistentTreeNode> *Alloc;
		WPPropertyPersistentTreeNode *Merge(WPPropertyPersistentTreeNode *&, WPPropertyPersistentTreeNode *&);
		void LessThanSplit(WPPropertyPersistentTreeNode *, WPPropertyPersistentTreeNode *&, WPPropertyPersistentTreeNode *&, WPPosition);
		WPPropertyPersistentTreeNode *SingleRotateWithLeft(WPPropertyPersistentTreeNode *&);
		WPPropertyPersistentTreeNode *SingleRotateWithRight(WPPropertyPersistentTreeNode *&);
		WPPropertyPersistentTreeNode *Insert(WPPropertyPersistentTreeNode *&, const WPProperty &);
		WPPropertyPersistentTreeNode *Remove(WPPropertyPersistentTreeNode *T, const WPProperty &P);
		void DFS(WPPropertyPersistentTreeNode *, std::vector <WPProperty> &);
		bool Compare(const WPProperty &, const WPProperty &);
};

#endif

