#pragma once

#include "Bounds.h"
#include <vector>


// ���� ������ ���� ������
enum class NodeIndex
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	Straddling,
	OutOfArea
};


class ENGINE_API Node
{
public:
	Node(const Bounds& bounds, int depth = 0);
	~Node();

	// ����(Insert)
	void Insert(Node* node);

	// ����(Query)
	void Query(const Bounds& queryBounds, std::vector<Node*>& possibleNodes);

	// ����
	void Clear();

	// ��ġ ������Ʈ 
	void Update(float x, float y);

	// Getter
	// ���� ����� ����
	Bounds GetBounds() const { return bounds; }

	// �ڽ� ���
	inline Node* TopLeft() const { return topLeft; }
	inline Node* TopRight() const {return topRight;}
	inline Node* BottomLeft() const {return bottomLeft;}
	inline Node* BottomRight() const {return bottomRight;}

	inline std::vector<Node*> Points() const { return points; }
private:
	// ������ 4���� �Ҷ� ����� �Լ�
	bool Subdivide();

	// ������ ���ҵƴ��� Ȯ���ϴ� �Լ�
	bool IsDivided();

	// �ڽ� ��� ���� �Լ�
	void ClearChildren();

	// ������ ������ ��ġ�� ������ ��ȯ�ϴ� �Լ�
	NodeIndex TestRegion(const Bounds& bounds);

	// ������ ������ ���Եǰų� ��ġ�� ������ ��� ���� �� ����� �Լ�
	std::vector<NodeIndex> GetQuads(const Bounds& bounds);



private:
	// ���� ����� ����
	int depth = 0;

	// ���� ����� ����
	Bounds bounds;

	// ���� ������ ���Ե� ���
	std::vector<Node*> points;

	// �ڽ� ���
	Node* topLeft = nullptr;
	Node* topRight = nullptr;
	Node* bottomLeft = nullptr;
	Node* bottomRight = nullptr;

};
