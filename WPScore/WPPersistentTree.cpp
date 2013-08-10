#include "WPPersistentTree.h"

WPPersistentTree::WPPersistentTree()
{
}

WPPersistentTree::~WPPersistentTree()
{
}

/// Multinote

WPMultinotePersistentTreeNode::WPMultinotePersistentTreeNode()
{
	Priority = ran();
	Left = Right = NULL;
	Sum = Fraction (0, 1);
	Size = 1;
}

WPMultinotePersistentTreeNode::~WPMultinotePersistentTreeNode()
{
}

void WPMultinotePersistentTreeNode::Update()
{
	Sum = Element.getLength();
	Size = 1;
	if (Left)
	{
		Sum += Left->Sum;
		Size += Left->Size;
	}
	if (Right)
	{
		Sum += Right->Sum;
		Size += Right->Size;
	}
}

WPMultinotePersistentTree::WPMultinotePersistentTree(WPAllocator <WPMultinotePersistentTreeNode> *A)
{
	Root = NULL;
	Alloc = A;
}

WPMultinotePersistentTree::~WPMultinotePersistentTree()
{
}

WPMultinotePersistentTreeNode *WPMultinotePersistentTree::Merge(WPMultinotePersistentTreeNode *&A, WPMultinotePersistentTreeNode *&B)
{
	if (!A || !B)
		return A ? A : B;
	WPMultinotePersistentTreeNode *T = Alloc->Allocate();
	if (A->Priority > B->Priority)
	{
		*T = *A;
		T->Right = Merge(A->Right, B);
	}
	else
	{
		*T = *B;
		T->Left = Merge(A, B->Left);
	}
	T->Update();
	return T;
}

void WPMultinotePersistentTree::Split(WPMultinotePersistentTreeNode *T, WPMultinotePersistentTreeNode *&X, WPMultinotePersistentTreeNode *&Y, Fraction K)
{
	if (T == NULL)
	{
		X = NULL;
		Y = NULL;
		return;
	}
	Fraction Before = T->Element.getLength();
	if (T->Left)
		Before += T->Left->Sum;
	WPMultinotePersistentTreeNode *D = Alloc->Allocate();
	*D = *T;
	if (Before < K)
	{
		X = D;
		Split(T->Right, X->Right, Y, K - Before);
		X->Update();
	}
	else
	{
		Y = D;
		Split(T->Left, X, Y->Left, K);
		Y->Update();
	}
}

WPMultinotePersistentTreeNode *WPMultinotePersistentTree::SingleRotateWithLeft(WPMultinotePersistentTreeNode *&T)
{
	WPMultinotePersistentTreeNode *L = Alloc->Allocate();
	*L = *T->Left;
	WPMultinotePersistentTreeNode *LR = L->Right;
	WPMultinotePersistentTreeNode *D = Alloc->Allocate();
	*D = *T;
	D->Left = LR;
	D->Update();
	L->Right = D;
	L->Update();
	return L;
}

WPMultinotePersistentTreeNode *WPMultinotePersistentTree::SingleRotateWithRight(WPMultinotePersistentTreeNode *&T)
{
	WPMultinotePersistentTreeNode *R = Alloc->Allocate();
	*R = *T->Right;
	WPMultinotePersistentTreeNode *RL = R->Left;
	WPMultinotePersistentTreeNode *D = Alloc->Allocate();
	*D = *T;
	D->Right = RL;
	D->Update();
	R->Left = D;
	R->Update();
	return R;
}

WPMultinotePersistentTreeNode *WPMultinotePersistentTree::Insert(WPMultinotePersistentTreeNode *&T, Fraction K, const WPMultinote &Item)
{
	WPMultinotePersistentTreeNode *D = Alloc->Allocate();
	if (T == NULL)
	{
		D->Element = Item;
		D->Update();
		return D;
	}
	*D = *T;
	if (K <= (T->Left ? T->Left->Sum : Fraction (0, 1)))
	{
		D->Left = Insert(T->Left, K, Item);
		D->Update();
		if (D->Left->Priority > D->Priority)
			return SingleRotateWithLeft(D);
	}
	else
	{
		D->Right = Insert(T->Right, K - T->Element.getLength() - (T->Left ? T->Left->Sum : Fraction (0, 1)), Item);
		D->Update();
		if (D->Right->Priority > D->Priority)
			return SingleRotateWithRight(D);
	}
	return D;
}

void WPMultinotePersistentTree::DFS(WPMultinotePersistentTreeNode *T, std::vector <WPMultinote> &V)
{
	if (!T)
		return;
	DFS(T->Left, V);
	V.push_back(T->Element);
	DFS(T->Right, V);
}

void WPMultinotePersistentTree::insert(const WPPosition &P, const WPMultinote &Nt)
{
	Root = Insert(Root, P.getValue(), Nt);
}

void WPMultinotePersistentTree::remove(const WPInterval &I)
{
	WPMultinotePersistentTreeNode *A, *B, *C, *D;
	Split(Root, D, C, I.end().getValue());
	Split(D, A, B, I.begin().getValue());
	Root = Merge(A, C);
}

std::pair <Fraction, WPMultinote> WPMultinotePersistentTree::query(const WPPosition &P)
{
	Fraction F = P.getValue();
	WPMultinotePersistentTreeNode *T = Root;
	while (T)
	{
		if (F < (T->Left ? T->Left->Sum : Fraction (0, 1)))
		{
			T = T->Left;
			continue;
		}
		F -= (T->Left ? T->Left->Sum : Fraction (0, 1));
		if (F >= T->Element.getLength())
		{
			F -= T->Element.getLength();
			T = T->Right;
			continue;
		}
		break;
	}
	if (!T)
		return std::make_pair(Fraction (- 1, 1), WPMultinote (WPNote (WPNote::Rest, Fraction (- 1, 1))));
	return std::make_pair(F, T->Element);
}

std::vector <WPMultinote> WPMultinotePersistentTree::traverse()
{
	std::vector <WPMultinote> Result;
	DFS(Root, Result);
	return Result;
}


/// Property

WPPropertyPersistentTreeNode::WPPropertyPersistentTreeNode()
{
	Priority = ran();
	Left = Right = NULL;
	Size = 1;
}

WPPropertyPersistentTreeNode::~WPPropertyPersistentTreeNode()
{
}

void WPPropertyPersistentTreeNode::Update()
{
	Size = 1;
	if (Left)
		Size += Left->Size;
	if (Right)
		Size += Right->Size;
}

WPPropertyPersistentTree::WPPropertyPersistentTree(WPAllocator <WPPropertyPersistentTreeNode> *A)
{
	Root = NULL;
	Alloc = A;
}

WPPropertyPersistentTree::WPPropertyPersistentTree(WPAllocator <WPPropertyPersistentTreeNode> *A, WPPropertyPersistentTreeNode *R)
{
	Root = R;
	Alloc = A;
}

WPPropertyPersistentTree::~WPPropertyPersistentTree()
{
}

WPPropertyPersistentTreeNode *WPPropertyPersistentTree::Merge(WPPropertyPersistentTreeNode *&A, WPPropertyPersistentTreeNode *&B)
{
	if (!A || !B)
		return A ? A : B;
	WPPropertyPersistentTreeNode *T = Alloc->Allocate();
	if (A->Priority > B->Priority)
	{
		*T = *A;
		T->Right = Merge(A->Right, B);
	}
	else
	{
		*T = *B;
		T->Left = Merge(A, B->Left);
	}
	T->Update();
	return T;
}

void WPPropertyPersistentTree::LessThanSplit(WPPropertyPersistentTreeNode *T, WPPropertyPersistentTreeNode *&X, WPPropertyPersistentTreeNode *&Y, WPPosition K)
{
	if (T == NULL)
	{
		X = NULL;
		Y = NULL;
		return;
	}
	WPPropertyPersistentTreeNode *D = Alloc->Allocate();
	*D = *T;
	if (T->Element.getInterval().begin() < K)
	{
		X = D;
		LessThanSplit(T->Right, X->Right, Y, K);
		X->Update();
	}
	else
	{
		Y = D;
		LessThanSplit(T->Left, X, Y->Left, K);
		Y->Update();
	}
}

WPPropertyPersistentTreeNode *WPPropertyPersistentTree::SingleRotateWithLeft(WPPropertyPersistentTreeNode *&T)
{
	WPPropertyPersistentTreeNode *L = Alloc->Allocate();
	*L = *T->Left;
	WPPropertyPersistentTreeNode *LR = L->Right;
	WPPropertyPersistentTreeNode *D = Alloc->Allocate();
	*D = *T;
	D->Left = LR;
	D->Update();
	L->Right = D;
	L->Update();
	return L;
}

WPPropertyPersistentTreeNode *WPPropertyPersistentTree::SingleRotateWithRight(WPPropertyPersistentTreeNode *&T)
{
	WPPropertyPersistentTreeNode *R = Alloc->Allocate();
	*R = *T->Right;
	WPPropertyPersistentTreeNode *RL = R->Left;
	WPPropertyPersistentTreeNode *D = Alloc->Allocate();
	*D = *T;
	D->Right = RL;
	D->Update();
	R->Left = D;
	R->Update();
	return R;
}

WPPropertyPersistentTreeNode *WPPropertyPersistentTree::Insert(WPPropertyPersistentTreeNode *&T, const WPProperty &Item)
{
	WPPropertyPersistentTreeNode *D = Alloc->Allocate();
	if (T == NULL)
	{
		D->Element = Item;
		D->Update();
		return D;
	}
	*D = *T;
	if (Item.getInterval().begin() < T->Element.getInterval().begin())
	{
		D->Left = Insert(T->Left, Item);
		D->Update();
		if (D->Left->Priority > D->Priority)
			return SingleRotateWithLeft(D);
	}
	else
	{
		D->Right = Insert(T->Right, Item);
		D->Update();
		if (D->Right->Priority > D->Priority)
			return SingleRotateWithRight(D);
	}
	return D;
}

void WPPropertyPersistentTree::DFS(WPPropertyPersistentTreeNode *T, std::vector <WPProperty> &V)
{
	if (!T)
		return;
	DFS(T->Left, V);
	V.push_back(T->Element);
	DFS(T->Right, V);
}

void WPPropertyPersistentTree::insert(const WPProperty &P)
{
	Root = Insert(Root, P);
}

WPPropertyPersistentTree WPPropertyPersistentTree::query(const WPInterval &I)
{
	WPPropertyPersistentTreeNode *A, *B, *C, *D;
	LessThanSplit(Root, D, C, I.end());
	LessThanSplit(D, A, B, I.begin());
	return WPPropertyPersistentTree (Alloc, B);
}

std::vector <WPProperty> WPPropertyPersistentTree::traverse()
{
	std::vector <WPProperty> Result;
	DFS(Root, Result);
	return Result;
}
