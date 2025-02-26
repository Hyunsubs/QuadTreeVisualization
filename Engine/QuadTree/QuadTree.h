#pragma once

#include <vector>
#include "Node.h"


class BoundingBox;
// ���� Ʈ�� Ŭ����
class ENGINE_API QuadTree
{
public:
	QuadTree(const Bounds& bounds);
	~QuadTree();
	

	// Ʈ���� ��带 �߰��ϴ� �Լ�
	void Insert(Node* node);

	// ������ ���� ��ġ�� ��带 ��ȯ�ϴ� �Լ�
	std::vector<Node*> Query(Node* queryNode);
	void Clear(const Bounds& bounds);

public:
	// ���� �� �ִ� Ʈ���� �ִ� ���� ��
	static const int maxDepth = 15;

private:
	Node* root = nullptr;
};

