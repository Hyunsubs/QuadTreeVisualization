#include "PreCompiledHeader.h"
#include "Bounds.h"

bool Bounds::Intersects(const Bounds& other) const
{
    // �ٸ� ������ ���� ������ ������ �ٱ��� �ִ� ���.
    if (other.x - other.width / 2 > MaxX()) return false;

    // �ٸ� ������ ���� ������ ���� �ٱ��� �ִ� ���.
    if (other.MaxX() < x - width / 2) return false;

    // �ٸ� ������ ���� ������ �Ʒ��� �ٱ��� �ִ� ���.
    if (other.y - other.height / 2 > MaxY()) return false;

    // �ٸ� ������ ���� ������ ���� �ٱ��� �ִ� ���.
    if (other.MaxY() < y - height / 2) return false;

    return true;
}